---
title: Generative mini synth
description: Idea for a Pico that composes and plays notes through an I2S DAC. Not planned as a build.
sidebar:
  order: 4
  badge:
    text: Idea
    variant: note
---

A small synth, built around a Raspberry Pi Pico, that makes up music as it plays. Knobs and buttons steer it. Sound comes out of an I2S DAC module, then a small amp or a headphone jack.

The author does not plan to build this. It is written down so someone else can try it. Nothing on this page has been wired, coded, or heard.

## Status

| | |
| --- | --- |
| Status | idea |
| Evidence | unbuilt |

There is no schematic, no chosen DAC, and no firmware in this repository. The evidence line stays at unbuilt.

## Concept image

No image.

## Why

The interesting part is the composing, not a fixed tune. You turn knobs for tempo, scale or key, density, and randomness, and you press buttons to mutate the pattern or freeze it. The Pico keeps playing while you steer.

Do the work in stages. A tone through a DAC comes before any "smart" generator. A rules-based tune comes before a model trained on other people's MIDI.

Numbers below are from Raspberry Pi's own pages, checked Nov 10, 2026. They are chip and board limits, not a claim that a particular synth fits.

A [Raspberry Pi Pico](https://www.raspberrypi.com/products/raspberry-pi-pico/) uses the RP2040: two Arm Cortex-M0+ cores, clock up to 133 MHz, 264 kB of on-chip SRAM, and 2 MB of on-board QSPI flash. The RP2040 datasheet's Cortex-M0+ configuration (section 2.4.1.2) lists a hardware multiplier and does not list an FPU. The Pico product page calls floating point "accelerated integer and floating-point libraries on-chip," which is software, not a hardware floating-point unit. The published Pico peripheral list includes SPI, I2C, PWM, USB, and PIO. It does not list an I2S controller, so a DAC that wants I2S clocks is a PIO or bit-bang problem. This page does not pick pins.

Note-level generation is the realistic target: a few numbers per note (pitch, time, velocity), like MIDI. Raw-audio neural generation, inventing the waveform sample by sample, is not. At audio rates that is tens of thousands of samples a second, and the 264 kB SRAM is the whole working memory for the program, the audio buffer, and the model.

[Pico 2](https://www.raspberrypi.com/products/raspberry-pi-pico-2/) is a reasonable later board, not the starting point. Raspberry Pi lists it from $5, with RP2350, 520 kB SRAM, 4 MB flash, and either dual Cortex-M33 cores or dual Hazard3 cores at up to 150 MHz. The RP2350 datasheet says the Cortex-M33 cores include an FPU. That helps float-heavy inference. It does not make waveform-rate neural audio a small project.

:::caution[Headphones and amps]
Start with the volume down, and listen in the room before you put on headphones. A synth that gets stuck on a loud tone can hurt your hearing. This page has no measured output level, because nothing has been built.
:::

## Tools

Nothing here has been priced. These are not a shopping list.

| Tool | Purpose | Price | Link |
| --- | --- | --- | --- |
| A computer | Later stages train or count MIDI on a PC, then copy a small table to the Pico | not checked | TBD |
| Headphones or a small speaker | Hear the DAC only after the volume is down | not checked | TBD |
| A desk way to wire the first DAC | Breadboard and jumpers, if you do not already have a way to connect modules | not checked | TBD |

## Parts

No module has been chosen beyond the kind of board. Example links are filled in only where the product page was opened on Nov 10, 2026. Every row is illustrative. Nothing is verified-compatible.

| Part | Purpose | Qty | Example link | Price | Compatibility |
| --- | --- | --- | --- | --- | --- |
| Raspberry Pi Pico (RP2040) | Runs the player and, later, the generator | 1 | [Raspberry Pi Pico](https://www.raspberrypi.com/products/raspberry-pi-pico/) | From $4 on Raspberry Pi's page, checked Nov 10, 2026. Seller prices differ | Illustrative |
| I2S DAC module | Turns the digital stream into an analog audio signal | 1 | TBD | not checked | Illustrative |
| Small amp or headphone jack | Lets you hear that signal | 1 | TBD | not checked | Illustrative |
| Knobs (potentiometers) | Tempo, scale or key, density, randomness | as needed | TBD | not checked | Illustrative |
| Buttons | Mutate, freeze, and anything else you add | as needed | TBD | not checked | Illustrative |
| Raspberry Pi Pico 2 (RP2350), optional | More RAM, more flash, and an FPU on the Cortex-M33 cores | 1, only if you outgrow the Pico | [Raspberry Pi Pico 2](https://www.raspberrypi.com/products/raspberry-pi-pico-2/) | From $5 on Raspberry Pi's page, checked Nov 10, 2026 | Illustrative |

## Build steps

These are milestones for whoever picks the project up. None of them have been done. There is no wiring diagram.

### 1. The Pico plays tones through an I2S DAC

Connect one DAC module and make the Pico play a steady tone, then a short scale. No generator yet.

**Pass:** you hear the tones you asked for, at a volume you chose, with no surprise blast when it starts.

**Fail:** silence or a stuck harsh tone. Stop, lower the volume, and recheck the DAC clocks and data pin before you add knobs.

### 2. Knobs and buttons

Add the controls: tempo, scale or key, density, randomness, mutate, and freeze. They may still drive a trivial pattern.

**Pass:** each control changes one thing you can hear, and freeze holds the current pattern until you let it go.

**Fail:** a knob changes the wrong thing, or mutate cannot be undone. Fix that mapping before the generator gets more complicated.

### 3. A rules-based generator

Stay inside a scale. A random walk or a simple arpeggiator picks the next note. Euclidean rhythms are one way to place the hits. Density and randomness should do what the knobs say.

**Pass:** the notes stay in the scale you selected, and turning density or randomness changes the music in the direction you expected.

**Fail:** notes leave the scale, or a knob does nothing. The bug is in this step, not in a neural net you have not trained.

### 4. A Markov chain from MIDI, trained on a PC

On a computer, read MIDI files and count which note tends to follow which. Export a small table. The Pico only plays that table. It does not parse MIDI files itself in this step.

**Pass:** the Pico plays from the table, and a second export (after you change the MIDI set) changes what you hear.

**Fail:** the table is too big for the 2 MB flash or the 264 kB SRAM, or the Pico never leaves the old tune. Shrink the table before you try a neural model.

### 5. Optional: a tiny neural next-note model

Train a very small next-note predictor on a PC. An int8 quantized RNN, LSTM, or MLP is the sort of model this means. Run it on the RP2040 with TensorFlow Lite Micro or a short inference loop you write yourself. The output is still notes, not audio samples.

**Pass:** the Pico produces notes from the model without freezing the audio stream, and you can still freeze or mutate from the buttons.

**Fail:** it does not fit, or the audio clicks every time the model runs. Stop. A bigger model is a Pico 2 question, not a reason to generate raw audio on the RP2040.

## Software side

Training happens on a PC, in Python. The Pico only receives a table or a quantized model you already built.

MIDI datasets are not automatically yours to train on or to ship. A file you downloaded can be all rights reserved, non-commercial, or restricted to research. Read the license before you train, and again before you put the result in a device you give away. This page does not name a dataset, because none has been chosen.

## Scripts

No scripts and no firmware in this repository. Nothing here should be flashed.

Manual fallback: use the blink or tone example that comes with the board you already have, on the desk, and write the board name in the test log. Do not start with stage 5.

## Open questions and risks

| Item | What is still unknown |
| --- | --- |
| DAC and amp | No module is chosen, so there is no pin map and no measured volume |
| I2S on the RP2040 | Feasible in principle with PIO. No sketch here has been compiled or heard |
| Markov table size | Has to be measured against 264 kB SRAM and 2 MB flash. No table exists yet |
| Neural model | Which architecture, and whether int8 weights leave room for an audio buffer, is untested |
| Pico 2 | A reasonable upgrade for hardware floating point. Not required for stages 1–4, and not tried |
| Dataset license | Unresolved until someone picks files and reads their terms |

## Next tiny step

Someone else can do stage 1 only: one Pico, one I2S DAC, one audible tone, volume down first. The author is not planning that build.

## Test log

No tests yet. Add a row only after you try something.

| Date | What you tried | Result |
| --- | --- | --- |
| | | |
