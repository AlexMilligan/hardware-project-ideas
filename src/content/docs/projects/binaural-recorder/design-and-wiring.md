---
title: Design and wiring
description: Pin table, preamp values, clock, and power rule for the binaural recorder. Not a build log.
sidebar:
  label: Design and wiring
  badge:
    text: Unverified
    variant: caution
---

Reference for the [binaural recorder](../). The figures in the Oct 30, 2026 plan are not redrawn here. Where a number lived only in a figure, this page says so instead of filling it in. Nothing on this page has been built.

## Signal path

| Stage | What happens | Numbers the plan took from datasheets |
| --- | --- | --- |
| Ear mics | Electret capsules turn sound pressure into a small voltage. They need a DC bias through a resistor | AOM-5024L-HD-R: −24 ±3 dB (about 63 mV at 94 dB SPL), SNR 80 dB(A), max 110 dB SPL, 2.2 kΩ load, 1–10 V |
| Preamp | Boosts the mic to a level the ADC can use. LOW is ×2 (+6 dB). HIGH is ×12 (+21.6 dB) | MCP6022: rail-to-rail, 2.5–5.5 V, 8.7 nV/√Hz, about 1 mA per amplifier |
| ADC | The PCM1808 samples both channels and sends 24-bit data on a 3-wire I²S bus | 99 dB dynamic range (typical). Full scale about 3 V peak-to-peak at 5 V. 60 kΩ input |
| Pico | Makes the ADC master clock, receives I²S, buffers in RAM, writes WAV files | RP2040 at 110.571 MHz, clock out 12.2857 MHz, 47,991 Hz sample rate |
| Files | Stereo 24-bit WAV on microSD, a status LED, and a USB serial log on the bench | About 288 KB/s, about 1.04 GB per hour |

## Why this preamp

The plan compared three options and picked the MCP6022.

| Option | Why it looked good | Why it lost, or won | Verdict in the plan |
| --- | --- | --- | --- |
| MAX9814 board | One cheap board. Mic bias built in. AGC can reportedly be disabled by tying TH to MICBIAS | Gain is fixed at 40, 50, or 60 dB. Even at 40 dB the plan says the ADC would clip at about 78.5 dB SPL, far below a live set. AGC pumping would also move the level | Not used |
| MCP6022, both channels | You choose the gain. Fixed gain keeps levels steady. Rail-to-rail on one 5 V supply. DIP fits a breadboard. Resistors and caps come from the kits | About 20 parts to place. Layout and grounding take care | Chosen |
| PCM1863 | One chip: PGA from −12 to +32 dB, mic bias, PLL. Single 3.3 V supply | TSSOP-30, no common hobby breakout (only TI's EVM), needs I²C setup | A possible later version on a custom PCB. Not this build |

:::note
The plan says the MAX9814 figures were not re-checked against the analog.com datasheet, because that site timed out the day the plan was written. The part is not in this design.
:::

## Level budget

ADC full scale in the plan is about 1.06 V RMS at a 5 V supply. The mic gives about 63 mV at 94 dB SPL. The mic's ±3 dB tolerance moves both clip points by up to 3 dB. At LOW gain the plan puts the noise floor at about 17 dB(A) SPL equivalent, with mic noise and ADC noise about equal, and the op-amp adding almost nothing.

| Gain | Switch | Preamp | ADC clips at (sine, typical mic) | Use |
| --- | --- | --- | --- | --- |
| LOW | open | ×2 (+6 dB) | about 112.5 dB SPL, near the mic's own 110 dB limit | Music, loud rooms |
| HIGH | closed | ×12 (+21.6 dB) | about 96.9 dB SPL | Ambient sound, speech, quiet rooms |

:::caution[About 110 dB SPL]
The capsule is rated to 110 dB SPL (THD under 3%). Next to a PA or a drum kit it can distort on its own, at either gain. Stand back, and check a test recording on headphones before the set.
:::

## Clock

The PCM1808 wants a system clock (SCKI) of exactly 256, 384, or 512 times the sample rate, with a 40–60% duty cycle. The plan says an exact 12.288 MHz (256 × 48 kHz) cannot be made from the Pico's 12 MHz crystal while staying inside the RP2040's rated 133 MHz. The closest clean option it uses:

- System clock = 12 MHz × 129 / 7 / 2 = 110.571 MHz (inside the rating).
- GP21 (GPOUT0) = that clock ÷ 9 = 12.2857 MHz, which is SCKI.
- Turn on the 50% duty-cycle correction (DC50). Without it, divide-by-9 is 44%, which the plan says is still inside the 40–60% window.
- Run the PCM1808 as master, 256 fs, so MD0 and MD1 are HIGH. It makes BCK and LRCK. The Pico is the I²S slave.
- Sample rate: 47,991 Hz. Put `47991` in the WAV header.

:::note[47,991 Hz]
That rate is not 48 kHz. The plan calls the error inaudible and says audio apps will read the header. Resample later if a tool wants 48 kHz or 16 kHz.
:::

The plan rejects letting the Arduino I²S library make the master clock: its integer dividers land near 12.8 MHz instead of 12.288 MHz at 48 kHz, which is not a valid ratio for the PCM1808. Exact 48,000 Hz would mean a 12.288 MHz crystal oscillator on SCKI and no GP21 clock. That oscillator was not priced.

## Power

On the bench (steps 1–5), power everything from USB through the Pico. Pico VBUS (physical pin 40) is 5V_RAW. In step 6, move 5V_RAW to the PowerBoost output instead.

The PowerBoost 1000C charges the LiPo from USB and outputs 5.2 V. The plan says its 1 A charge rate suits a 2000 mAh cell. Analog parts (ADC analog side, op-amp, mic bias) run from an RC-filtered rail the plan calls A5V: 10 Ω, then 1000 μF, then 0.1 μF. The point is to keep the booster's 700 kHz switching ripple out of the audio. The PCM1808 datasheet, as the plan cites it, notes that supply variation moves the reference, so a clean A5V matters.

To charge, plug USB into the PowerBoost, not the Pico.

:::danger[PowerBoost and the Pico's USB]
Do not connect the PowerBoost output to VSYS while the Pico's USB is plugged in, unless you add a Schottky diode between them as the Pico datasheet describes. The plan does not name that diode, and this page does not pick one. Unplug power before you change wiring. The LiPo rules on the [project page](../#parts) apply as soon as a cell is connected.
:::

:::note[Battery life is unknown]
Not measured. Do not treat the ADC's datasheet current as a runtime. Measure a full charge in step 6.
:::

## Preamp parts

Build this twice, left and right. The left channel uses MCP6022 pins 1–3. The right uses pins 5–7. The notch or dot marks pin 1.

| Part | Value | Job, as the plan states it |
| --- | --- | --- |
| R1 + C1 | 1 kΩ + 100 μF | Cleans the supply that feeds the mic. One filter per channel |
| R2 | 2.2 kΩ | Mic bias. The mic's datasheet is specified with a 2.2 kΩ load. The mic sees about 3.4 V, inside its 1–10 V range |
| C2 | 10 μF, + toward the mic | Passes audio and blocks the mic's DC |
| R3 | 100 kΩ | Holds the op-amp input at VREF (2.5 V, mid-supply) so audio can swing both ways |
| Rf, Rg, Rh, Cg | 10 kΩ, 10 kΩ, 1 kΩ, 100 μF | Gain is 1 + Rf/Rg = 2 with the switch open. Switch closed, Rg in parallel with Rh is about 909 Ω, so 1 + 10 kΩ/909 Ω is about 12. Cg keeps DC gain at 1 |
| Cf | 100 pF | Rolls off far-ultrasonic gain (about 160 kHz) for stability |
| C3, R4, C4 | 10 μF, 100 Ω, 10 nF | DC block and anti-alias into the ADC, in the range the plan says the PCM1808 datasheet suggests (100 Ω / 0.01 μF) |

One VREF divider is shared by both channels. The plan's figure shows it, and shows op-amp supply decoupling. The written part list does not give the divider's two resistor values, or that decoupling capacitor's value. Those values are an open question on the [project page](../#open-questions-and-risks). Do not invent them from a typical schematic.

:::caution
The plan's text does not number the op-amp supply pins. Read them on the MCP6022 datasheet (the plan cites Microchip document 20001685E) before you connect power. Electrolytic capacitors are marked with a stripe on the minus side. C2's plus end points at the mic.
:::

## Pin table

Physical pin numbers are the ones in parentheses in the plan (GP10 is Pico pin 14, and so on).

| From | To | Signal | Notes from the plan |
| --- | --- | --- | --- |
| PCM1808 OUT (DOUT) | Pico GP10 (pin 14) | I²S data | Slave input needs DIN, BCLK, and LRCLK on consecutive GPIOs |
| PCM1808 BCK | Pico GP11 (pin 15) | Bit clock, from the ADC | 3.072 MHz (64 × fs) |
| PCM1808 LRC (LRCK) | Pico GP12 (pin 16) | Word clock, from the ADC | 47,991 Hz |
| Pico GP21 (pin 27) | PCM1808 SCK (SCKI) | Master clock, 12.2857 MHz | Series 47 Ω (or 33–100 Ω) near the Pico. Keep the wire short and away from the analog inputs |
| PCM1808 MD0, MD1 | 3V3 | Master, 256 fs | Check whether your module has solder jumpers for these |
| PCM1808 FMT (may be labeled FMY) | GND | Format = I²S | |
| PCM1808 +5V | A5V | Analog supply | 4.5–5.5 V allowed |
| PCM1808 3.3 | Pico 3V3 OUT (pin 36) | Digital supply | 2.7–3.6 V allowed |
| Preamp OUT_L / OUT_R | PCM1808 VINL / VINR | Analog audio | If the module already has input capacitors, keep C3 anyway. The plan says that is harmless |
| Jack tip / ring / sleeve | Preamp MIC_L / MIC_R / GND | Mic signals | Tip = L, ring = R, sleeve = both grounds and both shields. Shields are grounded at the plug end only |
| microSD CLK / SI / SO / CS | GP18 (pin 24) / GP19 (pin 25) / GP16 (pin 21) / GP17 (pin 22) | SPI0 | SD 3V from Pico 3V3 OUT. GND to GND |
| Record button | GP15 (pin 20) and GND | Start / stop | Internal pull-up. Debounce in firmware |
| Battery sense | BAT, then 100 kΩ, to GP26 (pin 31), then 100 kΩ, to GND | ADC0 | Reads half the battery voltage. Add 0.1 μF from GP26 to GND |
| PowerBoost 5V | Pico VSYS (pin 39) and the A5V filter | 5V_RAW | Only after step 6. See the power rule above |
| PowerBoost EN | Power switch, then GND | On / off | EN tied to GND means off |
| Status LED | GP25, onboard | Status | Slow blink = idle, solid = recording, fast blink = error |

Join every ground (mic sleeve, preamp, PCM1808, Pico, SD, PowerBoost) at one point: the PCM1808 module's GND. Run the digital wires (SCKI, BCK, SPI) on the opposite side of the board from the mic inputs.

:::caution[Unverified module pinout]
Check this table against your module with a meter before you power it. The names came from the VVatashi/Pico-I2S example, not from a check of the Amazon listing.
:::

## Files and storage

24-bit stereo at 47,991 Hz is about 288 KB/s, or about 1.04 GB per hour, so a 32 GB card holds about 30 hours. FAT32 limits a file to 4 GB, so the plan starts a new file every 30 minutes. SD cards sometimes pause while writing, which is why audio is buffered in RAM.

The plan's notes for the recorder that does not exist yet: use Arduino-Pico's SDFS (based on SdFat 2), which its docs recommend over the old SD library, on SPI0 (GP16, GP17, GP18, GP19). Use a ring buffer of 32–64 KB. Update the WAV header about every 10 seconds so a dead battery still leaves a playable file. Optional: core 1 reads I²S while core 0 writes the card.

## Wearing the mics

Put each capsule at the entrance of the ear canal, in the bowl of the outer ear (the concha). The sound hole faces outward, away from your head. Do not push it into the canal. If that is uncomfortable, just in front of the canal entrance also works. Make left and right match in depth and angle. Keep hair, hat brims, and collars off the mics. Do not wear headphones over them while recording. Label the plugs and cables L and R. The plan suggests colored heat-shrink, for example blue = L and red = R.

Cable path in the plan:

1. Solder the cable to the capsule's two pads. Slide a short piece of heat-shrink over the joint and the back of the capsule. Leave the front sound hole open. The heat-shrink gives the furry windscreen something to grip.
2. Run the cable up behind each ear, like in-ear monitors. Leave a small slack loop so turning your head never pulls the mic.
3. Fasten the cable to the headband every few centimeters. Route both cables to the back of the head, then down the neck to one 3.5 mm TRS plug.
4. At the box, clamp the cable to the enclosure before the jack, so a yank hits the clamp and not the solder.

Wind is the main enemy of a body-worn mic. The plan says foam alone helps only a little, and uses a furry windscreen over each mic. Indoors you can take them off. Cables that rub on clothing make thumps. Secure every loose run. Keep a short fixed cable at the ear, and let the longer run to your pocket hang under your shirt.

## Soldering

You solder in three places: headers on the PCM1808 (step 3), the mic cable joints and the TRS plug (steps 4 and 6), and the Perma-Proto (step 6). The Pico H already has headers. Everything before step 6 can stay on a breadboard.

Practice first: solder 20 header pins onto a leftover strip on a spare Perma-Proto. A good joint is shiny and shaped like a small volcano, with no blobs and no gaps.

Heat the pad and the pin together for 1–2 seconds, feed solder into the joint (not onto the iron), pull the solder away, then pull the iron away. Wipe the tip often. The plan's starting point for lead-free solder is about 330–350 °C.

Shielded cable: strip about 10 mm, twist the shield braid into a lead, and cover it with heat-shrink. Slide the plug shell and the heat-shrink onto the cable before you solder the plug.

Before power-up, check polarity and use a multimeter in continuity mode to look for shorts between 5 V and GND.

:::caution
The iron is about 300–350 °C. Use a stand, ventilate the fumes, wash your hands after handling solder, and wear eye protection when you clip leads. The mic capsule is heat-sensitive. Tin the wire and the pad, then join them in about 2 seconds, and let it cool before you try again.
:::

## Sources

Datasheets and guides named in the plan:

- [TI PCM1808](https://www.ti.com/lit/ds/symlink/pcm1808.pdf)
- [TI PCM1863](https://www.ti.com/lit/ds/symlink/pcm1863.pdf)
- [RP2040](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf)
- [Raspberry Pi Pico](https://datasheets.raspberrypi.com/pico/pico-datasheet.pdf)
- [PowerBoost 1000C guide](https://learn.adafruit.com/adafruit-powerboost-1000c-load-share-usb-charge-boost)
- PUI AOM-5024L-HD-R datasheet: the plan's `api.puiaudio.com` link was truncated. [PUI's product page](https://puiaudio.com/product/microphones/aom-5024l-hd-r) is the manufacturer page, not that datasheet URL.
- MCP6022: the plan cites `ww1.microchip.com/…/20001685E.pdf` (path truncated). Document 20001685E.

Firmware and libraries named in the plan:

- [Arduino-Pico](https://github.com/earlephilhower/arduino-pico), including its README for the board-manager URL
- [I²S](https://arduino-pico.readthedocs.io/en/latest/i2s.html) and [filesystems / SDFS](https://arduino-pico.readthedocs.io/en/latest/fs.html)
- [SdFat](https://github.com/greiman/SdFat)
- [Pico SDK](https://github.com/raspberrypi/pico-sdk) and [pico-extras](https://github.com/raspberrypi/pico-extras)
- [C SDK I²S slave example](https://github.com/malacalypse/rp2040_i2s_example)
- [FatFS on the Pico](https://github.com/carlk3/no-OS-FatFS-SD-SDIO-SPI-RPi-Pico)
- [PCM1808 + Pico wiring example](https://github.com/VVatashi/Pico-I2S)
- [PCM1808 master / Pico slave example](https://github.com/monpaf/pico-pcm1808-usb-audio)

Legal (Texas, not advice):

- [Penal Code §16.02](https://statutes.capitol.texas.gov/GetStatute.aspx?Code=PE&Value=16.02)
- [Business & Commerce Code §503.001](https://statutes.capitol.texas.gov/GetStatute.aspx?Code=BC&Value=503.001)
- [Texas Attorney General, Biometric Identifier Act](https://www.texasattorneygeneral.gov/consumer-protection/file-consumer-complaint/consumer-privacy-rights/biometric-identifier-act)
