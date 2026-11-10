---
title: Social battery patch
description: Idea for a backpack-sized felt battery patch with five lights and one button. Not built.
sidebar:
  order: 3
  badge:
    text: Idea
    variant: note
---

A soft patch, shaped like a battery, that shows how much social battery you have left.

## Status

| | |
| --- | --- |
| Status | idea |
| Evidence | unbuilt |

There is no schematic, no chosen board, and no firmware. Hardware and code are both still ahead of this page, so the evidence line stays at unbuilt rather than pretending a design exists to test.

## Concept image

No image yet.

If you add a generated picture, caption it exactly:

> AI-generated concept — not built; not a wiring reference

## Why

The patch is a budget wearable for a hat or a backpack. It is felt or craft foam, flexible, and battery-shaped. Five windows each have a light behind them. Those five lights are individually controllable. Foam dividers sit between the windows so one light does not bleed into the next. A translucent fabric, or a frosted sheet, diffuses the windows.

One push button cycles the display: full, then 4, then 3, then 2, then 1, then empty. The next press after empty returns to full. That cycle is the idea; it has not been built. The first version uses steady lights, not an animation.

The microcontroller and a USB power bank live in a pocket. A cable runs to the patch and is secured, so a tug pulls on the strain relief and not on the board. Hook-and-loop mounts the patch. Make it backpack-sized first. Shrink it for a hat only after the large one works.

:::tip
The first milestone is smaller than the patch: on a desk, one LED changes when you press the button. Do that before you cut fabric.
:::

:::caution
Do not bend a circuit board or its solder joints. A hat curves. If the board would have to flex to fit, it does not fit there yet.
:::

:::caution
Take the electronics out before you wash the patch. The fabric can go through a wash. The board, the joints, and the battery pack should not.
:::

## Tools

Nothing here has been priced, and nothing is a required brand. If you do not already have a way to try one LED and a button on a desk, that gap belongs in the test log. It is not a shopping list yet.

| Tool | Purpose | Price | Link |
| --- | --- | --- | --- |
| Scissors | Cut the felt or foam | not checked | TBD |
| Needle and thread, or another way you already attach fabric | Hold the patch together | not checked | TBD |
| A desk setup for one LED and one button | First milestone only | not checked | TBD |
| Multimeter, if you already have one | Look at a connection before you apply power | not checked | TBD |

## Parts

No part has been chosen. Example links are `TBD` on purpose. Do not fill them with a search result that has not been checked for this project. Prices are `not checked`. Every row is illustrative.

| Part | Purpose | Qty | Example link | Price | Compatibility |
| --- | --- | --- | --- | --- | --- |
| Felt or craft foam | Flexible, battery-shaped patch | 1 patch | TBD | not checked | Illustrative |
| LED segments | One light behind each window, each controllable on its own | 5 | TBD | not checked | Illustrative |
| Foam dividers | Stop light leaking into the next window | enough to separate the five windows | TBD | not checked | Illustrative |
| Translucent fabric or frosted sheet | Diffuse the windows | enough to cover the windows | TBD | not checked | Illustrative |
| Small microcontroller | Read the button and drive the five segments | 1 | TBD | not checked | Illustrative |
| Push button | Each press moves to the next level | 1 | TBD | not checked | Illustrative |
| USB power bank | Power, carried in a pocket | 1 | TBD | not checked | Illustrative |
| Cable | Join the pocket to the patch. Secure it so a tug misses the board | 1 | TBD | not checked | Illustrative |
| Hook-and-loop | Mount the patch on a backpack, and later a hat | 1 set | TBD | not checked | Illustrative |

## Build steps

These steps are the intended order. None of them have been done.

### 1. One LED and one button, on the desk

Use whatever board you already have, or one you can name in the test log. Wire one LED and one button. No fabric yet.

**Pass:** each press changes that LED in the way you wrote down before you powered it.

**Fail:** the LED never changes. Unplug, and recheck the button wiring before you add a second LED.

### 2. Five lights, one button, still on the desk

The button cycles full, 4, 3, 2, 1, empty, then full again. Steady on or off only.

**Pass:** each press shows the next count, and only that many lights are on.

**Fail:** a press skips a level, sticks, or lights an extra segment. Stop adding fabric until this passes.

### 3. Battery-shaped patch and foam dividers

Cut the felt or craft foam into a battery shape with five windows. Put foam between the windows so light cannot cross.

**Pass:** with one segment lit, the neighbor stays dark.

**Fail:** you can see glow in the next window. Add or reseat a divider before you call the patch done.

### 4. Diffuser

Cover the windows with translucent fabric or a frosted sheet.

**Pass:** each window is a soft patch of light.

**Fail:** you still see a harsh point, or the cover blocks the light so the level is hard to read.

### 5. Pocket pack and a secured cable

Put the microcontroller and the USB power bank in a pocket. Secure the cable so a pull loads the anchor, not the solder joints.

**Pass:** a firm tug on the cable does not move the board.

**Fail:** the board or a joint moves. Add the anchor before you wear it.

### 6. Hook-and-loop on a backpack

Mount the backpack-sized patch. Wear the bag, take it off, pick it up.

**Pass:** the patch stays put.

**Fail:** it peels off. Fix the mount before you shrink anything.

### 7. Shrink for a hat, only after step 6

Make a smaller patch of the same idea. The board still must not bend.

**Pass:** the small patch still cycles through all six levels, and the board and joints stay flat.

**Fail:** the hat shape forces the board to flex. Keep the backpack size.

## Scripts

No scripts yet. The microcontroller is not chosen, so this repository does not include code that could flash a board.

Manual fallback for step 1: use the button-and-LED example that came with the board you already have. Do it on the desk. Write the board's name in the test log. If you have no board, the next tiny step waits. Do not buy a pile of parts in order to invent a pinout.

## Open questions and risks

| Item | What is still unknown |
| --- | --- |
| Microcontroller | No board is chosen, so there is no pin map |
| How the LEDs are driven | Separate LEDs, a module, or something else. No resistor values are listed, because none were chosen |
| Cable | Length and how the strain relief is made are not designed |
| Hat fit | A curved hat may not hold a rigid board without bending joints. That is why the backpack comes first |
| Washing | The electronics have to come out first. How the patch opens is not designed |
| Price | Nothing has been priced or linked |

## Next tiny step

On a desk, make one LED respond to one button. Stop there. Write down the board you used. Do not cut the patch yet.

## Test log

No tests yet. Add a row only after you try something.

| Date | What you tried | Result |
| --- | --- | --- |
| | | |
