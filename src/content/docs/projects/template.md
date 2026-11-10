---
title: Project template
description: The sections every project page uses, and the blank file to copy.
sidebar:
  order: 1
  badge:
    text: Template
    variant: tip
---

Every project page uses the same sections, in this order. The blank file to copy is [`src/content/docs/projects/_template.md`](https://github.com/AlexMilligan/hardware-project-ideas/blob/main/src/content/docs/projects/_template.md). Starlight does not publish files whose names start with `_`.

Do not invent a spec, a price, or a link. Write `TBD` or `not checked`, and say what you would need in order to fill it in.

## One-liner

One sentence: what it is and who it is for.

## Status

Pick one status: `idea`, `draft design`, `parts ordered`, `desk test`, `built`, or `abandoned`.

Pick every evidence tag that is true: `unbuilt`, `hardware untested`, `firmware untested`, `verified on bench`. Name the check when you use verified on bench.

## Concept image

Optional. If you add a picture that was generated, caption it with this sentence:

> AI-generated concept — not built; not a wiring reference

Do not use that picture to show wires, pins, or polarity. Leave the slot empty rather than adding a picture you cannot stand behind.

## Why

A few short paragraphs: the reason to build it, and the decisions that are already made. Put gaps in open questions, not in the middle of a confident sentence.

## Tools

Tools are not parts. List what you need in hand to do the steps (iron, meter, scissors) apart from the bill of materials. If a price was not checked, say `not checked`. Do not borrow a price from memory.

## Parts

A table with these columns:

| Column | What to put there |
| --- | --- |
| Part | The thing itself. Generic is fine when nothing is chosen |
| Purpose | The job it does in this project |
| Qty | How many. Use "as needed" if a count would be a guess |
| Example link | A URL you opened, or `TBD` |
| Price | Currency, amount, and the date you checked. Or `not checked` |
| Compatibility | `verified-compatible` or `illustrative` |

**Verified-compatible** means this exact part was checked in this build and the page says what passed. **Illustrative** means it is an example or a design pick that has not been proven here. When you are unsure, it is illustrative.

## Build steps

Five to eight short numbered steps. Each step has a **Pass** line and a **Fail** line so you can tell whether you are still on track. Longer reference material (pin tables, calculations) goes in a sub-section or a sub-page, and the step links to it.

## Scripts

Reader-facing helpers for programming or configuration, so the build steps stay at five to eight. Each script states:

- tested or untested
- the OS, and the tool versions that were actually used (or that none were pinned)
- that it checks prerequisites before it changes anything
- that it never flashes or overwrites unless the person running it takes an explicit action
- a manual fallback that does not need the script

No script yet is a valid answer. Say so, and say why.

## Open questions and risks

What is still unknown, and what could go wrong. This is the right place for a datasheet limit, a pinout you have not metered, or a runtime nobody has measured.

## Next tiny step

The single next action. Small enough to do without finishing the project.

## Test log

A dated list of what was tried and whether it passed. No rows until someone tries something. Do not backfill a log from a plan.

## Asides

Warnings and safety notes on the site use asides, not GitHub alert quotes.

:::note
Notes carry context: an odd sample rate, a price date, a missing measurement.
:::

:::tip
Tips are optional technique, not safety.
:::

:::caution
Cautions are likely mistakes: an unverified pinout, an untested upload, bending a solder joint.
:::

:::danger
Danger is harm. Battery fire and mains voltage belong here.
:::
