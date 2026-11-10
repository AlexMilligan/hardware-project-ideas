---
title: Binaural ear-mounted stereo recorder
description: Draft design for two ear mics and a pocket WAV recorder. Not built. Hardware and firmware untested.
sidebar:
  order: 2
  label: Overview
  badge:
    text: Draft design
    variant: caution
---

Two tiny microphones at the entrances of your ear canals, and a pocket recorder that writes a stereo file of what those ears heard.

## Status

| | |
| --- | --- |
| Status | draft design |
| Evidence | unbuilt; hardware untested; firmware untested |

This page condenses a design plan dated Oct 30, 2026. Prices on this page were checked that day. Nothing in the plan was built, ordered, or run. The [design and wiring](./design-and-wiring/) page holds the pin table, the preamp values, and the power rule. The six steps below are the plan's milestones, shortened.

:::note
Specs below come from the manufacturers' datasheets cited on the design page, as the plan recorded them. Anything the plan could not verify is marked unverified here too.
:::

## Concept image

No image yet. The plan's block diagrams are not reproduced on this site.

If you add a generated picture, caption it exactly:

> AI-generated concept — not built; not a wiring reference

## Why

Played back on headphones, the recording should sound like the listener is standing where you stood. Mics at your ears pick up the three clues your brain already uses: a sound arrives at the near ear first, your head quiets and dulls the far ear, and the shape of your outer ear changes the tone a little depending on direction. You do not add EQ. On speakers the effect is weaker, because both ears hear both speakers.

Each mic gets an MCP6022 op-amp preamp with a switch. LOW gain is ×2 (+6 dB), for loud music. HIGH gain is ×12 (+21.6 dB), for ambient sound and speech. A PCM1808 24-bit stereo ADC sends I²S to a Raspberry Pi Pico. The Pico makes the ADC's master clock, listens as an I²S slave, and writes 24-bit WAV files to a microSD card. A LiPo and an Adafruit PowerBoost 1000C supply power. The analog parts get an extra RC filter on their 5 V supply so the booster's switching noise stays out of the audio. The plan's estimated parts total is about $158.85 before shipping and tax. Basic tools, if you need them, add about $97.30.

The plan also keeps the raw file if you later high-pass filter rumble (it suggests around 80 Hz as an example, in firmware or in an editor). A later "bunny ears" hat, with the mics spaced apart, is a different technique. It is a wide stereo pair, not this head-shaped effect. The same electronics can feed either one.

:::caution[About 110 dB SPL]
The AOM-5024L is rated to about 110 dB SPL (THD under 3%). Right next to a PA or a drum kit the sound can be louder than that, and the mic itself distorts no matter which gain you pick. Stand back from the stacks, and listen to a test recording on headphones before the set.
:::

:::note[47,991 Hz]
The sample rate is 47,991 Hz, not 48,000 Hz. The plan calls that inaudible (about 0.02% slow, −186 ppm). Write `47991` into the WAV header so playback speed is exact. The reason for the odd rate is on the [design page](./design-and-wiring/#clock).
:::

:::note[Not legal advice. Recording laws vary.]
The plan's legal note is written for Texas. Texas is a one-party consent state (Penal Code §16.02): you may record a conversation you are part of. Do not record conversations you are not part of. Do not leave the recorder running in a room you have left.

Texas Business & Commerce Code §503.001 treats a voiceprint as a biometric identifier. Capturing one for a commercial purpose means telling the person and getting consent, then protecting the data and deleting it within a set time. The Attorney General enforces it, with penalties up to $25,000 per violation. The plan says personal hobby use is mainly outside that focus, and that a voice-ID use which becomes commercial needs consent from the people you enroll.

Recording a performance also involves the artist's rights and the venue's rules, even with a media pass. Get the band's OK before you share recordings.

Statute links and the Attorney General page are on the [design page](./design-and-wiring/#sources).
:::

## Tools

Optional. The plan priced these at SparkFun on Oct 30, 2026, for someone who does not already own them. Shipping and tax are not included. Tools are not in the $158.85 parts total.

| Tool | Purpose | Price | Link |
| --- | --- | --- | --- |
| Soldering iron, 60 W adjustable (TOL-14456) | Headers, mic joints, the final board | $25.95 | [SparkFun](https://www.sparkfun.com/soldering-iron-60w-adjustable-temperature.html) |
| Lead-free solder, 100 g (TOL-09325) | Those joints | $24.95 | [SparkFun](https://www.sparkfun.com/solder-lead-free-100-gram-spool.html) |
| Digital multimeter, basic (TOL-12966) | The pass/fail voltage checks | $21.50 | [SparkFun](https://www.sparkfun.com/digital-multimeter-basic.html) |
| Soldering stand with brass sponge (TOL-24061) | Park the iron | $13.95 | [SparkFun](https://www.sparkfun.com/soldering-stand-with-brass-sponge.html) |
| Solder wick (TOL-29531) | Lift a bad joint | $10.95 | [SparkFun](https://www.sparkfun.com/solder-wick-2-5ft-techspray.html) |

Tools subtotal in the plan: **$97.30**.

Upgrade the plan mentions, instead of the 60 W iron: Pinecil smart iron, SparkFun TOL-23913, $68.50. The plan did not include a URL for it, so this page does not invent one.

## Parts

Every price is US dollars, checked Oct 30, 2026, from the linked page that day. Shipping and tax are not included. Ordering from five sellers (Newark, Amazon, SparkFun, Adafruit, Performance Audio) adds several shipping charges. Newark pages in the plan loaded in a browser but blocked command-line checkers.

**Compatibility:** verified-compatible is not used on this page. Nothing has been bench-tested. A row can still be the plan's chosen part and stay illustrative until a check passes.

| Part | Purpose | Qty | Example link | Price | Compatibility |
| --- | --- | --- | --- | --- | --- |
| PUI AOM-5024L-HD-R electret mic | Ear mics | 2 | [Newark 02AM8398](https://www.newark.com/pui-audio/aom-5024l-hd-r/microphone-ecm-omnidirectional/dp/02AM8398) | $4.49 each ($8.98) | Illustrative |
| PCM1808 ADC module | Stereo 24-bit ADC | 1 | [Amazon B0D9LNGBD1](https://www.amazon.com/dp/B0D9LNGBD1) (GODIYMODULES) | $6.99 | Illustrative; pinout unverified |
| Raspberry Pi Pico H (with headers) | Controller and clock source | 1 | [SparkFun DEV-20172](https://www.sparkfun.com/raspberry-pi-pico-h.html) | $5.50 | Illustrative |
| MCP6022-I/P dual op-amp (DIP-8) | Preamp, both channels | 1 | [Newark 96K3737](https://www.newark.com/microchip/mcp6022-i-p/ic-op-amp-10mhz-7v-us-dip-8/dp/96K3737) | $1.86 | Illustrative |
| 8-pin DIP socket | Lets you swap the op-amp | 1 | [SparkFun PRT-07937](https://www.sparkfun.com/dip-sockets-solder-tail-8-pin.html) | $0.60 | Illustrative |
| microSD breakout | Storage interface | 1 | [Adafruit 4682](https://www.adafruit.com/product/4682) | $3.50 | Illustrative |
| SanDisk Ultra 32 GB microSD | Storage (about 30 hours) | 1 | [Amazon B08GY9NYRM](https://www.amazon.com/dp/B08GY9NYRM) | $21.33 | Illustrative |
| PowerBoost 1000C | LiPo charger and 5.2 V boost | 1 | [Adafruit 2465](https://www.adafruit.com/product/2465) | $19.95 | Illustrative |
| LiPo 3.7 V 2000 mAh (protected) | Battery | 1 | [Adafruit 2011](https://www.adafruit.com/product/2011) | $12.50 | Illustrative |
| Resistor kit, 500 pcs (COM-10969) | All resistors (10 Ω, 47 Ω, 100 Ω, 1 kΩ, 2.2 kΩ, 10 kΩ, 100 kΩ) | 1 | [SparkFun](https://www.sparkfun.com/resistor-kit-1-4w-500-total.html) | $9.95 | Illustrative |
| Capacitor kit (KIT-13698) | All caps (100 pF, 10 nF, 0.1 μF, 10 μF, 100 μF, 1000 μF) | 1 | [SparkFun](https://www.sparkfun.com/sparkfun-capacitor-kit.html) | $11.50 | Illustrative |
| SPDT slide switch | Gain L, gain R, and power | 3 | [Adafruit 805](https://www.adafruit.com/product/805) | $0.95 each ($2.85) | Illustrative |
| Tactile buttons, 20-pack | Record button | 1 | [Adafruit 367](https://www.adafruit.com/product/367) | $2.50 | Illustrative |
| 3.5 mm stereo jack, breadboard-friendly | Mic harness socket | 1 | [Adafruit 1699](https://www.adafruit.com/product/1699) | $0.95 | Illustrative |
| 3.5 mm TRS plug | End of the mic harness | 1 | [SparkFun COM-11143](https://www.sparkfun.com/audio-plug-3-5mm.html) | $0.75 | Illustrative |
| Mogami W2697 shielded cable | Mic cables, 2.5 mm outside diameter | 10 ft | [Performance Audio](https://www.performanceaudio.com/products/mogami-w2697-miniature-balanced-mic-cable-by-the-foot) | $0.39/ft ($3.90) | Illustrative |
| Furry windscreens, 0.4 inch, 5-pack | Wind protection | 1 | [Amazon B08DJ2L8N1](https://www.amazon.com/dp/B08DJ2L8N1) (BornFeel) | $9.99 | Illustrative; fit unverified |
| Full-size breadboard | Prototyping for steps 2–5 | 1 | [Adafruit 239](https://www.adafruit.com/product/239) | $5.95 | Illustrative |
| Jumper wires, male-to-male, 20 × 6 inch | Breadboard wiring | 2 | [Adafruit 1957](https://www.adafruit.com/product/1957) | $1.95 each ($3.90) | Illustrative |
| Perma-Proto half-size, 3-pack | Final soldered build (step 6) | 1 | [Adafruit 571](https://www.adafruit.com/product/571) | $12.50 | Illustrative |
| Heat-shrink kit | Mic pods, strain relief, insulation | 1 | [SparkFun PRT-09353](https://www.sparkfun.com/heat-shrink-kit.html) | $10.95 | Illustrative |
| Break-away male headers, 40-pin | Modules that arrive without headers | 1 | [SparkFun PRT-00116](https://www.sparkfun.com/break-away-headers-straight.html) | $1.95 | Illustrative |

Estimated parts total in the plan, before shipping and tax: **$158.85**.

Not in that total:

- The headband or cap. Use one you have. Not priced.
- Optional foam windscreens. The plan's Amazon pages were blocked by a captcha, so they were not priced.
- An enclosure. Not priced.
- Optional JST-PH extension, [Adafruit 1131](https://www.adafruit.com/product/1131), $1.95.

Other price notes from that day:

- The mic's DigiKey price showed about $4.58 in a search result. That page was blocked to the plan's checker, so it is not linked here.
- The resistor-kit page showed both "in stock" and "backorder".
- The microSD price used is $21.33 from the Amazon seller "LET'S BUY". An Amazon-shipped offer the same day was $26.99.
- The PCM1808 listing claims "105 dB". The TI datasheet says 99 dB typical. Trust the datasheet.
- Whether the 0.4 inch windscreens fit a 9.7 mm capsule plus heat-shrink is unverified. The plan says a small rubber band or a dab of fabric glue can hold them.

:::caution[Unverified PCM1808 module]
The Amazon module's pin labels, onboard input caps, MD/FMT jumpers, and whether the 3.3 V pin powers VDD directly were not verified for this listing. Signal names in the pin table come from a published wiring example ([VVatashi/Pico-I2S](https://github.com/VVatashi/Pico-I2S)). Check your board with a meter and read the silkscreen before you power it.
:::

:::danger[LiPo batteries]
A LiPo can catch fire if it is punctured, crushed, shorted, or over-charged. The plan says Adafruit's packs include a protection circuit (it cuts out at 3.0 V and on an output short). Treat the cell with care anyway.

Charge only with the PowerBoost or another proper LiPo charger, on a non-flammable surface, and not unattended or overnight at first. Never charge a puffy or damaged battery. Take it to a battery recycling drop-off.

Do not let the red and black wires touch. Cover exposed metal with heat-shrink, and check the JST polarity before you plug in. Adafruit-to-Adafruit matches.

On your body, keep the battery in a pocket or pouch. Not against your skin, not under a tight headband, and not in a hot car.

This design stays at 5.2 V or below. Do not connect any part of it to mains power. Unplug power before you change wiring.
:::

## Build steps

Six milestones. Do them in order. The pass line is the plan's test, not a result anyone has recorded.

:::tip
If a test fails, the problem is somewhere in the last thing you changed.
:::

### 1. Tools and hello Pico

Install the Arduino IDE. Add the Arduino-Pico board URL from that core's README ([earlephilhower/arduino-pico](https://github.com/earlephilhower/arduino-pico)). Select "Raspberry Pi Pico". Upload a Blink sketch that also prints `hello` every second. No wiring and no soldering.

**Pass:** the onboard LED (GP25) blinks once per second, and `hello` scrolls in the Serial Monitor.

**Fail:** no COM port. Hold BOOTSEL while you plug in, then upload again.

:::caution
The plan describes that sketch. It does not paste one. Use the Arduino IDE Blink example and add a line that prints `hello` once a second. This notebook has not run it.
:::

### 2. Make the ADC clock

Set the system clock to 110.571 MHz and put a divided-by-9 clock on GP21. The calls are in [`scripts/binaural-recorder/m2-adc-clock/m2-adc-clock.ino`](https://github.com/AlexMilligan/hardware-project-ideas/blob/main/scripts/binaural-recorder/m2-adc-clock/m2-adc-clock.ino) and again under [Scripts](#scripts). They are untested.

**Pass:** Serial shows about 110571428. A multimeter on DC volts at GP21 reads roughly half of 3.3 V (about 1.5–1.8 V), which means the pin is toggling fast.

**Fail:** 0 V or 3.3 V means there is no clock. Check the pin number and the code.

:::caution[Untested clock sketch]
These are Pico SDK calls the plan expects to find inside Arduino-Pico. Nobody has run them for this notebook. The plan says to run them before USB-dependent features if Serial misbehaves, and that changing `clk_sys` also changes SPI clock math (SdFat handles that).
:::

### 3. ADC talks to the Pico, no mics yet

Solder headers onto the PCM1808 module if it needs them. This is the plan's first soldering; the [soldering notes](./design-and-wiring/#soldering) are the reference. Wire the module from the [pin table](./design-and-wiring/#pin-table). Take A5V from the Pico's VBUS through the 10 Ω and 1000 μF filter described on that page. Then read samples. The plan's lines are an incomplete fragment, saved as `scripts/binaural-recorder/m3-i2s-fragment.txt`, not as a sketch you can upload.

**Pass:** the sample count is about 47,991 per second. With the inputs unconnected, levels stay very low and steady. Write that number down as your noise reference. Touching the VINL pin with a finger raises only the left value (hum).

**Fail:** zero samples means check SCKI, the MD pins, and the BCK and LRCK wiring. Values that look shifted or like garbage: try `setBitsPerSample(32)` with `read32()` and shift right by 8. If that still fails, the plan's fallback is the C SDK path named on the design page.

The plan says the PCM1808 output becomes valid about 187 ms after the clock starts (8960/fs), so throw away the first 0.25 s.

:::caution[Untested I²S slave firmware]
Arduino-Pico's docs say slave input is supported. The plan has not seen that path tested with this PCM1808. Step 3 is that test. Do not treat the fragment as working firmware.
:::

:::caution[Check the module with a meter]
Confirm the Amazon module's silkscreen against the pin table before you apply power. The plan did not verify this listing's pinout.
:::

### 4. Preamp and mics on the breadboard

Build both preamp channels, VREF, and the mic bias, using the [preamp table](./design-and-wiring/#preamp-parts). Solder 30 cm test leads to two mics. The wearable harness comes later.

**Pass (meter, power on, before you connect the ADC):** VREF reads 2.4–2.6 V. Both op-amp outputs (pins 1 and 7) read about 2.5 V. Each mic node reads about 3–3.7 V.

**Pass (sound):** a finger snap by the left mic shows a much bigger left level than right, and the reverse for the right mic. Flipping a gain switch raises that channel by about 15.6 dB (×6).

**Fail:** an op-amp output stuck near 0 V or 5 V usually means a wiring error or a backwards electrolytic capacitor. Power off and recheck.

The plan's level print is `20·log10(peak / 8388608)` in dBFS. That line is a note, not a shipped sketch.

### 5. Record WAV files to microSD

Add the SD breakout. Start and stop with the button. Write a 44-byte WAV header (PCM, 2 channels, 24-bit, 47991 Hz) and then packed 3-byte samples.

**Pass:** a 10-minute recording opens in Audacity, plays at the correct speed, has no clicks or gaps, and is about 173 MB. The firmware reports 0 I²S overflows (`i2s.getOverflow()`) and logs its longest SD write.

**Fail:** clicks or overflows. The plan says to enlarge the RAM buffer, write in 512-byte multiples, pre-allocate the file, or try a different card.

:::note
The plan does not include a WAV-writer sketch, so this repository does not include one. Steps 5 and 6 are behavior to implement later, not code that works today. The plan's implementation notes (SDFS on SPI0, a 32–64 KB ring buffer, a header rewrite about every 10 seconds, a new file every 30 minutes) are on the [design page](./design-and-wiring/#files-and-storage).
:::

### 6. Battery, harness, and wearing it

Move the circuit to the Perma-Proto and solder it. Add the PowerBoost, the LiPo, the power switch, and the battery sense. Follow the [power rule](./design-and-wiring/#power) before you connect the booster. Build the headband harness with strain relief and the furry windscreens. Placement is on the [design page](./design-and-wiring/#wearing-the-mics).

**Pass:** record 60 minutes on battery with 0 overflows. At HIGH gain in a quiet room, the spectrum (Audacity, Analyze, Plot Spectrum) shows no tall narrow tones from the booster. On headphones, a friend walking around you while talking sounds like they are in the right place, left and right and front and back. A firm tug on the cable at the ear does not move the mic or break audio. Write down how long a full battery lasts.

**Fail:** whine or hum. The plan says a bigger A5V capacitor, shorter ground wires, or moving the PowerBoost farther from the preamp.

:::note[Battery life is unknown]
The plan did not estimate runtime. Pico and SD current depend on firmware that has not been written or run. The ADC alone, from its datasheet as the plan quotes it, is 8.6 mA analog plus 5.9 mA digital, typical at 48 kHz. Measure the real runtime in this step. The plan says to read the divider on GP26, multiply by 2, and stop recording cleanly below about 3.4 V, before the PowerBoost's low-battery point (3.2 V).
:::

:::danger
Step 6 is the first time a LiPo is in the circuit. The battery rules in the parts section apply before you plug it in.
:::

:::caution[Soldering]
The iron is about 300–350 °C. Use a stand, ventilate the fumes, wash your hands after handling solder, and wear eye protection when you clip leads. The mic capsule is heat-sensitive: the plan says to tin the wire and the pad first, then join them in about 2 seconds.
:::

## Scripts

These helpers are for the clock and the I²S fragment, so steps 1–6 stay a short list. None of them upload, flash, or overwrite a board. The manual fallback is the Arduino IDE, which is also what the plan tells you to use.

### `scripts/binaural-recorder/check-setup.sh`

| | |
| --- | --- |
| Status | Print-only. Run once on Linux with no Arduino tools installed. Not run against the Arduino IDE or a Pico. It does not install software or open a port |
| OS | The plan names none. The script is POSIX `sh` and prints `uname` for whatever machine you are on |
| Tools | Looks for `arduino` and `arduino-cli` on `PATH` and prints a version only if `arduino-cli` is present. Arduino-Pico's version is not pinned, because the plan did not pin one |
| Before it changes anything | It changes nothing. It checks, prints, and exits |
| Flash / overwrite | There is no upload flag. Extra arguments are refused |
| Manual fallback | Install the Arduino IDE, add the board URL from the Arduino-Pico README, select "Raspberry Pi Pico", and type or open the sketch yourself |

### `scripts/binaural-recorder/m2-adc-clock/m2-adc-clock.ino`

Untested. Copied from the plan. OS and Arduino-Pico version: not pinned. The file does not upload itself. Open it in the Arduino IDE and upload only when you mean to. Manual fallback: type the same sketch by hand.

```cpp
#include "hardware/clocks.h"

void setup() {
  set_sys_clock_pll(1548000000, 7, 2);  // 12 MHz*129/7/2 = 110.571 MHz
  clock_gpio_init(21, CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLK_SYS, 9); // 12.2857 MHz
  hw_set_bits(&clocks_hw->clk[clk_gpout0].ctrl, CLOCKS_CLK_GPOUT0_CTRL_DC50_BITS);
  Serial.begin(115200);
}

void loop() {
  Serial.println(clock_get_hz(clk_sys));
  delay(1000);
}
```

:::caution
Do not treat this sketch as working. The plan lists it as untested standard SDK calls inside Arduino-Pico.
:::

### `scripts/binaural-recorder/m3-i2s-fragment.txt`

Untested and incomplete. The plan only wrote these lines. The file is text, not an `.ino`, so the Arduino IDE will not treat the folder as a sketch and upload a stub over a board. Manual fallback: after the clock code is running, add these calls yourself.

```cpp
#include <I2S.h>
I2S i2s(INPUT);
// in setup(), after the M2 clock code:
i2s.setSlave(); i2s.setDATA(10); // GP10=DIN, GP11=BCLK, GP12=LRCLK
i2s.setBitsPerSample(24); i2s.setBuffers(16, 256);
i2s.begin(47991);
// in loop(): int32_t l, r; i2s.read24(&l, &r); -> track peaks, count samples/second
```

If slave input never behaves with this chip, the plan's fallback is the Pico C SDK, using the archived [malacalypse/rp2040_i2s_example](https://github.com/malacalypse/rp2040_i2s_example) (I²S slave input) and [carlk3's FatFS library](https://github.com/carlk3/no-OS-FatFS-SD-SDIO-SPI-RPi-Pico). That fallback is also untested here.

## Open questions and risks

| Item | Status in the plan |
| --- | --- |
| Mic max SPL, 110 dB | May distort at the stage. Stand back, test first, and consider a higher-SPL capsule in a later version |
| 47,991 Hz sample rate | Fine for listening, and for ML if you resample to 48 kHz or 16 kHz in software. Exact 48 kHz would need a 12.288 MHz oscillator on SCKI. That part was not priced |
| Arduino-Pico I²S slave with the PCM1808 | Docs say slave input is supported. Not seen tested with this chip. Step 3 is the test. The C SDK example is the fallback |
| PCM1808 module details | Pin labels, onboard input caps, MD/FMT jumpers, and the 3.3 V pin are unverified for this Amazon module. Check with a meter and the silkscreen. Trust the TI datasheet's 99 dB typical, not the listing's "105 dB" |
| Battery runtime | Not estimated. Measure it in step 6 |
| Prices | Checked Oct 30, 2026. Shipping and tax not included. Amazon third-party prices change often |
| Windscreen fit | 0.4 inch muffs on a 9.7 mm capsule plus heat-shrink: unverified |
| MAX9814 datasheet | The plan could not fetch it (analog.com timed out). That part is not used. The comparison is on the design page, with that limit |
| VREF divider values | The plan's text says a mid-supply 2.5 V divider shared by both channels. It does not write down the two resistor values. They are not guessed on this site |
| Op-amp supply pins and its decoupling capacitor | The plan's text assigns op-amp pins 1–3 and 5–7 only. Supply-pin numbers and the decoupling value are not copied here, because they were not in the text |

## Next tiny step

Do step 1 only. Install the Arduino IDE, point it at the Arduino-Pico core using the URL in that project's README, select "Raspberry Pi Pico", and get a one-second blink plus `hello` on the Serial Monitor. No soldering, and no parts order, for that step.

## Test log

No tests yet. Hardware has not been assembled. The sketches have not been compiled or uploaded for this notebook.

| Date | What you tried | Result |
| --- | --- | --- |
| | | |
