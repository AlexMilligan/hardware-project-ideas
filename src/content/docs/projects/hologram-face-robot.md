---
title: Hologram face robot
description: Idea for a robot head whose face looks like a floating 3D character. Not planned as a build.
sidebar:
  order: 5
  badge:
    text: Idea
    variant: note
---

A robot whose face looks like a small 3D character hanging in the air, in the spirit of the driving visualization some cars draw around the road. The author is not planning to build this yet. It is here so someone else can try a first version. Nothing on this page has been filmed, projected, or wired.

## Status

| | |
| --- | --- |
| Status | idea |
| Evidence | unbuilt |

There is no enclosure, no chosen projector or screen, and no code in this repository.

## Concept image

No image.

## Why

A true floating hologram, light hanging in empty air with nothing in the way, is not a hobby build. What people call a hologram robot is usually a reflection, a projection, or a spinning light source your eyes fuse into a solid. Pick the easiest picture that still looks like a face, and only then chase a walk-around volume.

The face itself can be a [3D Gaussian splat](https://repo-sam.inria.fr/fungraph/3d-gaussian-splatting/): Kerbl, Kopanas, Leimkühler, and Drettakis, SIGGRAPH 2023. That paper represents a scene as 3D blobs and draws a view by splatting them. Their page reports real-time novel views at 1080p on the GPUs they used. It is a method for a captured scene, not a finished talking-head product, and it does not by itself animate a character with motion vectors. Motion between frames, and any extra frame-interpolation step, is work you add on the computer that renders.

Do that render on a GPU PC, or try a Jetson-class board if you want the GPU on the robot. Stream the pictures to whatever is driving the display. A Raspberry Pi-class board or an ESP32-class board is in the right neighborhood for playing a video or a stream. A [Pico](https://www.raspberrypi.com/products/raspberry-pi-pico/) is for the neck and eye servos only. The RP2040 on that board has 264 kB of on-chip SRAM and no hardware floating-point unit in the datasheet's Cortex-M0+ configuration. It is the wrong place to render splats.

:::note
This is inspired by the look of an in-car "reality" view. It is not that car's software, and this page does not claim to know how that view is drawn.
:::

## Display options

Easiest first. None of these has been tried for this project. Cost words are relative. Dollar amounts appear only where a page was opened on Nov 25, 2026.

### 1. Pepper's ghost

A screen plays the face. A clear sheet, or a small clear pyramid, sits at an angle and the viewer sees the reflection in a dim room. John Henry Pepper popularized this kind of stage reflection in the 1860s. A phone and a piece of plastic are enough for a desk demo.

**Difficulty:** lowest. **Cost:** low if you already have a screen.

**For a robot face:** convincing in the dark, from a limited angle. It is a flat reflection, not a head you can walk around. Bright rooms wash it out.

### 2. Other flat "floating" screens

A spinning LED fan draws a disc in the air by persistence of vision. A transparent OLED is a see-through flat panel. Both are easier than a swept volume and harder than a Pepper's ghost box. Neither was priced for this page.

**For a robot face:** the fan is a flat disc, noisy, and still a moving machine. A transparent OLED is a window, not a volume, and the panels are a specialty part.

### 3. Swept-volume display

A rotating LED strip or panel, or a screen that shuttles up and down, sweeps a volume. Your eyes fuse the flashes into a solid you can walk around. [Voxel Photonics](https://voxel3d.co/) describes this as swept-volume LED: the scene is cut into slices, and a fast surface paints those slices so persistence of vision builds the cloud. Their VX2 page, checked Nov 25, 2026, lists 30 volumes a second in a 25 cm volume, at $6,800 before shipping and tax. That is a commercial example, not a parts requirement.

The hobby version of the idea, briefly: each revolution (or each up-down pass) is one refresh of the whole volume. An encoder or a once-per-turn sensor tells the controller the angle. At that angle you light only the voxels in that slice. If the slice clock drifts from the motor, the head twists or smears. Power and data have to cross the moving joint, by a slip ring or by wireless power and a radio. This page has no slice count and no motor speed. Those are design choices nobody has measured here.

:::danger[Spinning volume]
A POV rotor is a machine, not a toy you hold. Balance it before it spins fast. Put it in an enclosure you cannot put a finger, hair, or a cable into. Stop it before you adjust anything. A loose LED board at those speeds can throw pieces. Do not power it through wires that wind up around the shaft. Use a slip ring or wireless power, and treat a home-built rotor as unproven until it has survived a slow, guarded test.
:::

## Projection options (theme-park style)

Projection is a fair way to get a character face. These are still ideas, not a tested booth.

### Projection-mapped mask

Project the face from inside a translucent shell, or from the front onto a sculpted mask or bust. A long-circulated [Haunted Mansion fan FAQ](http://www.betterhaunts.com/hmfaq/hm.txt) describes the singing busts as film, later video, projected from the front onto head statues. That page is a fan write-up, not an official Disney document. The technique it describes is ordinary projection onto a shaped surface.

**Difficulty:** low, and the best first solid face if you want something that reads as a head rather than a reflection. **Cost:** low to moderate if you already have a projector. The mask can be a found object or a print. Price not checked.

**Pros:** the face has a nose and cheeks, so it holds up better in a lit room than Pepper's ghost. **Cons:** you are aligning a projector to a shape. Shadows and a bright hotspot are normal. It is not floating in mid-air.

### Concave or parabolic mirror

The classic "mirage" toy is two parabolic mirrors. [Opti-Gone](https://optigone.com/index.php/specifications-of-the-giant-mirage/) describes a pair of opposing mirrors that form a real image of a small physical object in the opening, viewable all the way around, with no electricity. That image is of the object you put inside, not of a video, unless you put a tiny screen at the focus and accept a very small picture.

A projector plus a scrim or foil is the stage version of Pepper's ghost: the projector hits a hidden surface, and the audience sees the bounce in a clear sheet.

**Difficulty:** low for the toy, awkward for a moving robot face. **Cost:** the toy is a finished product; a custom pair of mirrors is not priced here.

**Pros:** the toy really does put an image in front of the glass. **Cons:** the floating picture is small, and a video face is a poor fit unless the display sits exactly at the focus.

### Mist, fog, or gauze

A thin sheet of mist can be a rear-projection screen. FogScreen's install notes describe a water-mist curtain that you project onto, best in still indoor air and dimmer light. Theatrical gauze (a scrim, sometimes sold as "holo gauze") is the cloth version: a see-through fabric that turns bright when you hit it with a projector and disappears when you do not.

**Difficulty:** moderate. You are managing air, light, and a projector, not electronics. **Cost:** not checked. A gauze scrap is cheap. A purpose-built mist curtain is a rental-grade machine.

**Pros:** the image can look as if it hangs in the room. **Cons:** it is still a flat sheet. Drafts tear a mist screen. Glycol fog is a different machine from a water-mist screen, and it does not belong in a closed robot head.

### Aerial plate and lens-array displays

[ASKA3D](https://aska3d.com/ja/technology.html) is a plate of tiny crossed mirrors. Asukanet describes it forming a real image in the air on the far side of the plate, at the same distance as the source. [Looking Glass](https://lookingglassfactory.com/) light-field displays show many perspectives at once through their optics (their site describes on the order of 45 to 100 views in a viewing cone). That is a lens-array style flat display, not a head you can walk around.

**Difficulty:** high, because you are buying a specialty optic. **Cost:** not checked.

**For a robot face:** possible later, not the first build. Both need the source image set up for that optic. Neither has been tried here.

:::caution[Projector light, lasers, and fog]
Do not stare into a projection lens. A laser-phosphor or scanning-laser projector is a laser product: read its class label, and do not look down the beam or bounce it into someone's eyes with a mirror. The [FDA's laser class list](https://www.fda.gov/radiation-emitting-products/home-business-and-entertainment-products/laser-products-and-instruments) is the hazard scale; this page does not assign a class to a projector you have not picked.

Glycol fog and haze are not the same as a water-mist screen. [Ontario's live-performance guideline](https://www.ontario.ca/document/safety-guidelines-live-performance-industry/fog-and-haze-effects) says the droplets are easy to inhale and can irritate eyes and airways, and tells you not to aim the nozzle at a person. A Martin haze-machine manual tells you not to fill a poorly ventilated room. Do not put a fog machine inside a closed head.
:::

## Tools

Nothing here has been priced.

| Tool | Purpose | Price | Link |
| --- | --- | --- | --- |
| A computer with a GPU | Fit and render the splat. The SIGGRAPH 2023 results were on desktop GPUs, not on a Pico | not checked | TBD |
| A dim corner | Pepper's ghost and mist screens need it. A lit shop bench hides them | not checked | TBD |
| A camera the tracking stage can use | Face tracking for the neck. A webcam is enough to start | not checked | TBD |

## Parts

Categories only, except the two product pages opened for this write-up. Every row is illustrative.

| Part | Purpose | Qty | Example link | Price | Compatibility |
| --- | --- | --- | --- | --- | --- |
| Screen you already have | Hidden image for a Pepper's ghost box | 1 | TBD | not checked | Illustrative |
| Clear sheet or pyramid | Reflects that screen | 1 | TBD | not checked | Illustrative |
| Projector | Mapped mask, scrim, mist, or foil ghost | 1 | TBD | not checked | Illustrative |
| Sculpted mask or bust | Surface for projection mapping | 1 | TBD | not checked | Illustrative |
| Servos | Neck and eyes | as needed | TBD | not checked | Illustrative |
| Raspberry Pi Pico | Servos only, not rendering | 1 | [Raspberry Pi Pico](https://www.raspberrypi.com/products/raspberry-pi-pico/) | not checked | Illustrative |
| Board that plays the stream | Pi-class or ESP32-class display driver | 1 | TBD | not checked | Illustrative |
| Swept-volume display, optional | Walk-around voxels. Commercial example, not a starter part | 1 | [Voxel Photonics VX2](https://voxel3d.co/) | $6,800 on that page, checked Nov 25, 2026, before shipping and tax | Illustrative |

## Build steps

Whoever builds this should stop at the first picture that looks like a face. The author is not scheduling these.

### 1. A looping face in a Pepper's ghost box

Play a short face video on a screen. Reflect it in a clear angled sheet inside a dim box or dome.

**Pass:** in a dim room, a second person sees a face in the plastic and can point at it.

**Fail:** they only see the screen, or the room lights erase the reflection. Darken the box before you add servos.

### 2. Alternative: a projection-mapped mask

Skip the ghost box if you would rather have a solid head. Project a looping face onto a mask or bust, from inside a translucent shell or from the front, and line the picture up with the features.

**Pass:** the eyes land on the sculpted eyes, and the face is still readable with the room lights on.

**Fail:** the picture slides off the nose when you move your head. Fix the mount before you call it mapped.

### 3. The head turns toward a person

Put the box or the mask on a neck. A camera finds a face. Servos on a Pico turn the head, and maybe the eyes.

**Pass:** one person walking across the room gets the face pointed at them, and the mechanism stops at the end of its travel instead of grinding.

**Fail:** it tracks a poster or a lamp. Tighten the detector before you stream a live render.

### 4. A live splat, streamed from the PC

On the GPU PC, render the Gaussian-splat head and send that view to the screen or projector. The Pico still only moves servos.

**Pass:** the face on the display changes when the render changes, without you copying a new video file by hand.

**Fail:** the stream stutters or the servos hitch. Slow the render before you add frame interpolation.

### 5. Optional: smoother motion

Interpolate extra frames, or warp with motion you computed between renders. This is extra software on the PC. The 2023 splat paper does not do this for you.

**Pass:** motion looks steadier, and dropping the extra step still leaves a working face from stage 4.

**Fail:** the face doubles or smears. Turn the extra step off. Stage 4 remains the result.

### 6. Optional: a swept-volume prototype

Only after a face already works on a screen. Render slices instead of one flat view, lock them to the motor, and keep the rotor inside the guard from the safety note.

**Pass:** two people standing in different places agree the head occupies a volume, and a finger cannot reach the rotor.

**Fail:** the image smears, or the rig vibrates. Stop the motor. Do not raise the speed to hide a sync bug.

## Software side

Rendering and any training stay on a PC with a GPU. The robot receives pictures, or slice data, and servo targets.

A splat is built from photos or video of a real person or a scan. Those pictures are someone else's likeness. Get permission before you capture a face, and do not assume a downloaded model is free to put on a machine you show in public. This page does not name a dataset or a model file.

## Scripts

No scripts and no firmware. Nothing here should be flashed.

Manual fallback for stage 3: a hobby servo and the sweep example that came with the board, on the desk, with the horn unloaded. Write the board name in the test log.

## Open questions and risks

| Item | What is still unknown |
| --- | --- |
| Which illusion | Pepper's ghost, a mapped mask, and a swept volume are different machines. None is chosen | 
| Likeness | Whose face, and whether you may capture or show it |
| Stream format | How pictures get from the PC to the display board. No protocol is picked |
| Jetson | A possible on-robot GPU. Not measured against the SIGGRAPH paper's desktop results |
| Swept volume | Slice count, rpm, and power across the joint are undesigned. The VX2 price is a commercial reference, not a build plan |
| Tracking | Which camera, and what happens with two people, is untested |

## Next tiny step

Someone else can do stage 1 or stage 2, not both at once: either a dim Pepper's ghost with a looping video, or a projector aimed at a mask. The author is not planning that build yet.

## Test log

No tests yet. Add a row only after you try something.

| Date | What you tried | Result |
| --- | --- | --- |
| | | |
