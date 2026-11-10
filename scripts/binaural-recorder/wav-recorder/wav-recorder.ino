// STATUS: compiles; untested on hardware.
//
// Stereo 24-bit WAV recorder for the binaural-recorder plan (Oct 30, 2026).
// The PCM1808 is the I2S master. This Pico is the slave. Nothing here was
// run on a Pico, an ADC, or a microSD card.
//
// Pinned tools this file was compiled with:
//   arduino-cli 1.5.1
//   Arduino-Pico core rp2040:rp2040 6.3.2 (Earle Philhower)
//   Board: Raspberry Pi Pico (rp2040:rp2040:rpipico)
// OS for that compile: Linux. The plan names no OS.
// No other libraries. I2S and SDFS ship inside Arduino-Pico 6.3.2.
//
// This file does not upload itself. check-setup.sh does not upload it.
// CI only compiles it. Upload from the Arduino IDE when you mean to.
// Manual fallback: type the same behavior into the IDE yourself, or open
// this file and upload it by hand.
//
// The sketch never formats the card and never opens a file that already
// exists. Recording starts only after the GP15 button or serial "r".

#include <I2S.h>
#include <SDFS.h>
#include <hardware/clocks.h>

static const uint32_t SAMPLE_RATE = 47991;  // 12.2857 MHz SCKI / 256
static const uint32_t DISCARD_FRAMES = 12000;  // plan: throw away the first 0.25 s
static const uint32_t FRAMES_PER_FILE = SAMPLE_RATE * 30u * 60u;
static const size_t CHUNK_BYTES = 1536;  // 256 frames * 6 bytes, a multiple of 512
static const uint8_t PIN_BUTTON = 15;
static const uint8_t PIN_SD_MISO = 16;
static const uint8_t PIN_SD_CS = 17;
static const uint8_t PIN_SD_SCK = 18;
static const uint8_t PIN_SD_MOSI = 19;

static I2S i2s(INPUT);
static File rec;
static bool recording = false;
static bool sdReady = false;
static bool recordError = false;
static uint32_t overflows = 0;
static uint32_t dataBytes = 0;
static uint32_t framesThisFile = 0;
static uint32_t discardLeft = 0;
static uint32_t headerRewriteFrames = 0;
static uint8_t chunk[CHUNK_BYTES];
static size_t chunkUsed = 0;
static bool buttonDown = false;
static uint32_t buttonChangedAt = 0;

static void putU16(uint8_t *p, uint16_t v) {
  p[0] = (uint8_t)(v & 0xff);
  p[1] = (uint8_t)(v >> 8);
}

static void putU32(uint8_t *p, uint32_t v) {
  p[0] = (uint8_t)(v & 0xff);
  p[1] = (uint8_t)((v >> 8) & 0xff);
  p[2] = (uint8_t)((v >> 16) & 0xff);
  p[3] = (uint8_t)((v >> 24) & 0xff);
}

// 44-byte PCM WAV header. Rate is the clock plan's 47,991 Hz, not a measured rate.
static void writeHeader() {
  uint8_t h[44];
  memcpy(h, "RIFF", 4);
  putU32(h + 4, 36u + dataBytes);
  memcpy(h + 8, "WAVE", 4);
  memcpy(h + 12, "fmt ", 4);
  putU32(h + 16, 16);
  putU16(h + 20, 1);
  putU16(h + 22, 2);
  putU32(h + 24, SAMPLE_RATE);
  putU32(h + 28, SAMPLE_RATE * 2u * 3u);
  putU16(h + 32, 6);
  putU16(h + 34, 24);
  memcpy(h + 36, "data", 4);
  putU32(h + 40, dataBytes);
  rec.seek(0);
  rec.write(h, sizeof(h));
  rec.seek(44u + dataBytes);
}

static void pack24(uint8_t *dst, int32_t sample) {
  // Arduino-Pico 6.3.2 I2S::read() returns the 24-bit sample right-aligned.
  // I2S::read24() then shifts left by 8 (see I2S.cpp). Store those same 24 bits.
  uint32_t u = (uint32_t)sample & 0x00FFFFFFu;
  dst[0] = (uint8_t)(u & 0xff);
  dst[1] = (uint8_t)((u >> 8) & 0xff);
  dst[2] = (uint8_t)((u >> 16) & 0xff);
}

static bool mountCard() {
  if (sdReady) {
    return true;
  }
  SPI.setRX(PIN_SD_MISO);
  SPI.setTX(PIN_SD_MOSI);
  SPI.setSCK(PIN_SD_SCK);
  SDFSConfig cfg;
  cfg.setCSPin(PIN_SD_CS);
  cfg.setAutoFormat(false);
  if (!SDFS.setConfig(cfg) || !SDFS.begin()) {
    Serial.println("microSD did not mount. Nothing was formatted or erased.");
    return false;
  }
  sdReady = true;
  return true;
}

static bool openNewFile() {
  char path[20];
  for (int n = 1; n <= 9999; n++) {
    snprintf(path, sizeof(path), "/REC%04d.WAV", n);
    if (SDFS.exists(path)) {
      continue;
    }
    rec = SDFS.open(path, "w+");
    if (!rec) {
      Serial.println("Could not create a new WAV. No existing file was opened.");
      return false;
    }
    dataBytes = 0;
    framesThisFile = 0;
    headerRewriteFrames = 0;
    chunkUsed = 0;
    discardLeft = DISCARD_FRAMES;
    writeHeader();
    Serial.print("Recording ");
    Serial.println(path);
    return true;
  }
  Serial.println("REC0001.WAV through REC9999.WAV already exist. Nothing overwritten.");
  return false;
}

static void stopRecord() {
  if (!recording) {
    return;
  }
  if (chunkUsed > 0 && rec) {
    rec.write(chunk, chunkUsed);
    dataBytes += chunkUsed;
    framesThisFile += chunkUsed / 6u;
    chunkUsed = 0;
  }
  if (rec) {
    writeHeader();
    rec.flush();
    rec.close();
  }
  recording = false;
  Serial.print("Stopped. I2S overflow events this session: ");
  Serial.println(overflows);
}

static void startRecord() {
  if (recording) {
    return;
  }
  if (!mountCard() || !openNewFile()) {
    recordError = true;
    return;
  }
  recordError = false;
  recording = true;
}

static void toggleRecord() {
  if (recording) {
    stopRecord();
  } else {
    startRecord();
  }
}

static void pollButton() {
  bool down = digitalRead(PIN_BUTTON) == LOW;
  uint32_t now = millis();
  if (down != buttonDown && (uint32_t)(now - buttonChangedAt) > 25u) {
    buttonDown = down;
    buttonChangedAt = now;
    if (down) {
      toggleRecord();
    }
  }
}

static void pollSerial() {
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    if (c == 'r' || c == 'R') {
      startRecord();
    } else if (c == 's' || c == 'S') {
      stopRecord();
    }
  }
}

static void updateLed() {
  uint32_t now = millis();
  if (recordError) {
    digitalWrite(LED_BUILTIN, ((now / 100u) & 1u) ? HIGH : LOW);
  } else if (recording) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, ((now / 500u) & 1u) ? HIGH : LOW);
  }
}

static void pumpAudio() {
  int32_t frame[2];
  size_t n = i2s.read((uint8_t *)frame, sizeof(frame));
  if (n != sizeof(frame)) {
    return;
  }
  if (discardLeft > 0) {
    discardLeft--;
    return;
  }
  pack24(chunk + chunkUsed, frame[0]);
  pack24(chunk + chunkUsed + 3, frame[1]);
  chunkUsed += 6;
  if (chunkUsed < CHUNK_BYTES) {
    return;
  }
  size_t wrote = rec.write(chunk, CHUNK_BYTES);
  chunkUsed = 0;
  if (wrote != CHUNK_BYTES) {
    Serial.println("microSD write failed. Stopping without deleting the file.");
    recordError = true;
    stopRecord();
    return;
  }
  dataBytes += CHUNK_BYTES;
  framesThisFile += CHUNK_BYTES / 6u;
  headerRewriteFrames += CHUNK_BYTES / 6u;
  if (headerRewriteFrames >= SAMPLE_RATE * 10u) {
    writeHeader();
    rec.flush();
    headerRewriteFrames = 0;
  }
  if (framesThisFile >= FRAMES_PER_FILE) {
    Serial.println("30 minutes. Starting the next file.");
    stopRecord();
    startRecord();
  }
}

static void startClock() {
  // Same calls as m2-adc-clock.ino and the Oct 30, 2026 plan.
  set_sys_clock_pll(1548000000, 7, 2);  // 12 MHz * 129 / 7 / 2 = 110.571 MHz
  clock_gpio_init(21, CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLK_SYS, 9);  // 12.2857 MHz
  hw_set_bits(&clocks_hw->clk[clk_gpout0].ctrl, CLOCKS_CLK_GPOUT0_CTRL_DC50_BITS);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  startClock();
  Serial.begin(115200);
  delay(300);
  Serial.println("STATUS: compiles; untested on hardware.");
  Serial.println("Arduino-Pico core 6.3.2. WAV rate field is 47991.");
  Serial.println("Button GP15, or serial r = start, s = stop.");
  Serial.println("This sketch will not format the card or overwrite a REC file.");

  i2s.setSlave();
  i2s.setDATA(10);  // GP10 DIN, GP11 BCLK, GP12 LRCLK, consecutive, slave input
  i2s.setBitsPerSample(24);
  i2s.setBuffers(32, 256);  // 32 * 256 * 4 = 32 KB DMA buffer
  if (!i2s.begin(SAMPLE_RATE)) {
    recordError = true;
    Serial.println("I2S begin() returned false. Not recording.");
  }
}

void loop() {
  pollButton();
  pollSerial();
  updateLed();
  if (recording) {
    pumpAudio();
    if (i2s.getOverflow()) {
      overflows++;
    }
  }
}
