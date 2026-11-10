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

The op-amp is the MCP6022-I/P already in the parts list: an 8-lead PDIP, so it fits the breadboard. Table 3-1 of [Microchip DS20001685F](https://ww1.microchip.com/downloads/aemDocuments/documents/MSLD/ProductDocuments/DataSheets/MCP6021-Data-Sheet-DS20001685.pdf) gives the same pin numbers for PDIP, SOIC, and TSSOP. The notch or dot marks pin 1.

| Pin | Name | This circuit |
| --- | --- | --- |
| 1 | VOUTA | Left preamp output |
| 2 | VINA− | Left inverting input (gain network) |
| 3 | VINA+ | Left non-inverting input, biased through R3 |
| 4 | VSS | Analog ground |
| 5 | VINB+ | Right non-inverting input, biased through its own R3 |
| 6 | VINB− | Right inverting input (gain network) |
| 7 | VOUTB | Right preamp output |
| 8 | VDD | A5V |

The MCP6022 has no VREF pin. Microchip's mid-supply pin is only on the MCP6021 and MCP6023 (Table 3-1 and section 4.6). Figure 4-8 draws that internal reference as 50 kΩ from VDD to the pin and 50 kΩ from the pin to VSS.

Build the signal parts twice, left and right. One bias divider is shared.

| Part | Value | Job |
| --- | --- | --- |
| R1 + C1 | 1 kΩ + 100 μF | Cleans the supply that feeds the mic. One filter per channel. From the plan |
| R2 | 2.2 kΩ | Mic bias. The mic datasheet is specified with a 2.2 kΩ load. The plan says the mic sees about 3.4 V, inside its 1–10 V range |
| C2 | 10 μF, + toward the mic | Passes audio and blocks the mic's DC. From the plan |
| Ra, Rb | 100 kΩ, 100 kΩ | Shared bias divider from A5V to ground. The midpoint is the bias node |
| Cb | 10 μF | From the bias node to ground |
| R3 | 100 kΩ, one per channel | From the bias node to the + input. From the plan |
| Rf, Rg, Rh, Cg | 10 kΩ, 10 kΩ, 1 kΩ, 100 μF | Gain is 1 + Rf/Rg = 2 with the switch open. Switch closed, Rg in parallel with Rh is about 909 Ω, so 1 + 10 kΩ/909 Ω is about 12. Cg keeps DC gain at 1. From the plan |
| Cf | 100 pF | Rolls off far-ultrasonic gain (about 160 kHz) for stability. From the plan |
| C3, R4, C4 | 10 μF, 100 Ω, 10 nF | DC block and anti-alias into the ADC. From the plan |
| Cvd | 0.1 μF | From MCP6022 pin 8 to pin 4 |

### Why 100 kΩ and 10 μF

The kit has 100 kΩ and 10 kΩ. It does not have 50 kΩ, so this divider cannot copy Figure 4-8 exactly.

Two 100 kΩ resistors draw 25 μA at 5.0 V and 26 μA at 5.2 V (`V / 200 kΩ`). Microchip's on-chip 50 kΩ + 50 kΩ divider draws 50 μA at 5 V. Two 10 kΩ resistors would draw 250 μA. The ADC's analog supply is already 8.6 mA typical, so 25 μA is the smaller of the kit choices.

The divider's Thevenin resistance is 50 kΩ. At 25 °C its open-circuit noise density is about 29 nV/√Hz (`sqrt(4kT × 50 kΩ)`), above the op-amp's 8.7 nV/√Hz at 10 kHz (DS20001685F). The 10 μF cap across the bottom resistor puts the pole at about 0.32 Hz (`1 / (2π × 50 kΩ × 10 μF)`). From 20 Hz up, that cap is under 800 Ω, and the noise of 800 Ω is about 3.6 nV/√Hz, under the op-amp. R3 is 100 kΩ, about 41 nV/√Hz, and was already in the plan. The cap does not change that.

Input bias current is 1 pA typical at 25 °C, and 150 pA maximum at 85 °C for the industrial part (DS20001685F). 150 pA through 50 kΩ is 7.5 μV, and through R3 is 15 μV. That does not move a bias of about 2.5 V.

DC gain is 1 because of Cg, so each output sits at the bias-node voltage. C2 keeps mic DC off the + input, so the only DC load on the divider is the bias current, not R3 as a resistor to ground.

### How that meets the PCM1808

PCM1808 electrical characteristics, typical, at VCC = 5 V: input voltage 0.6 × VCC (3 Vp-p), center voltage 0.5 × VCC, input impedance 60 kΩ. Recommended VCC is 4.5 V to 5.5 V, so the PowerBoost's 5.2 V output is inside that range. The features list states a 3 Vp-p single-ended input.

The ADC's center is its own reference. Chip pin 1 is named VREF and the pin table calls it "reference-voltage decoupling (= 0.5 VCC)". TI's typical application (figure 26, note 3) bypasses that pin with 0.1 μF ceramic and 10 μF electrolytic. TI says that applications section is not part of the component specification. The center voltage itself is in the electrical characteristics. Do not drive the pin, and do not tie it to the op-amp bias node. The two midpoints are both half of their own supply. They are not the same wire.

C3 is the plan's 10 μF series capacitor into the 60 kΩ input. That high-pass is about 0.27 Hz (`1 / (2π × 60 kΩ × 10 μF)`). TI's typical circuit uses 1 μF and calls the corner 2.7 Hz. Either cap removes the op-amp's DC. The PCM1808 also has a digital high-pass, 0.91 Hz at 48 kHz. The signal the ADC converts is centered on 0.5 × VCC by the ADC, not by our divider.

A 3 Vp-p sine around 2.5 V uses 1.0 V to 4.0 V. Section 4.2 says the output can reach within 20 mV of either rail with a 10 kΩ load. The ADC load is lighter than that (100 Ω in series with 60 kΩ). The 3 Vp-p swing fits. At 5.2 V the typical full-scale is 0.6 × 5.2 V = 3.12 Vp-p around 2.6 V, which still fits. A5V is the filtered rail, not a guaranteed 5.00 V, so measure it. The bias node should be half of that reading. The plan's 2.4–2.6 V pass window matches a supply of about 5 V.

:::caution
Electrolytic capacitors are marked with a stripe on the minus side. C2's plus end points at the mic. Cb's plus end points at the bias node if you use an electrolytic. On a breadboard the 0.1 μF from pin 8 to pin 4 cannot meet the datasheet's "within 2 mm" rule (section 4.7). Put it on the socket pins anyway. The A5V 1000 μF is the shared bulk capacitor (1 μF or larger, within 100 mm) from that same section. Keep it on the same board.
:::

:::caution[Two different midpoints]
The op-amp bias node and the PCM1808 VREF pin are both about half the analog supply. Connect them together and you are driving a pin the datasheet uses for decoupling. Leave the ADC pin on its own caps. The Amazon module's silkscreen is still unverified, so find VREF with the module in front of you before you add those caps.
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
| A5V | 100 kΩ Ra, then the bias node, then 100 kΩ Rb, to GND | Op-amp bias | Midpoint is half of A5V. One divider for both channels |
| Bias node | 10 μF to GND | Bias bypass | Pole about 0.32 Hz. See the preamp section |
| Bias node | R3, 100 kΩ, to MCP6022 pins 3 and 5 | + input bias | One R3 per channel. From the plan |
| MCP6022 pin 8 | A5V | VDD | Pin 4 (VSS) to GND. 0.1 μF from pin 8 to pin 4 |
| MCP6022 pin 1 / pin 7 | PCM1808 VINL / VINR | Analog audio | Through C3, R4, and C4. If the module already has input capacitors, keep C3 anyway. The plan says that is harmless |
| PCM1808 VREF (chip pin 1) | 0.1 μF and 10 μF to analog ground | ADC reference decoupling | Do not connect this pin to the op-amp bias node. Skip the caps if the module already has them |
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

`wav-recorder.ino` follows those notes and compiles. It is untested on hardware. It uses SDFS (the core's SdFat 2 wrapper; the docs prefer it to the old SD library) on SPI0: MISO GP16, CS GP17, SCK GP18, MOSI GP19. The I²S DMA buffer is `setBuffers(32, 256)`, which is 32 KB, the low end of the plan's 32–64 KB. The header is rewritten every 10 seconds of samples, and a new file starts every 30 minutes of samples. The optional split, core 1 reading I²S while core 0 writes the card, is not in this sketch.

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
- [PUI AOM-5024L-HD-R datasheet](https://api.puiaudio.com/filename/AOM-5024L-HD-R.pdf). Checked Nov 10, 2026: the URL returns a PDF (`%PDF-1.7`).
- [MCP6021/1R/2/3/4 datasheet, DS20001685F](https://ww1.microchip.com/downloads/aemDocuments/documents/MSLD/ProductDocuments/DataSheets/MCP6021-Data-Sheet-DS20001685.pdf). Checked Nov 10, 2026. The plan's older file, [20001685E](https://ww1.microchip.com/downloads/en/devicedoc/20001685e.pdf), also still downloads. Pin numbers on this page are from revision F.

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
