#!/bin/sh
# Binaural recorder — prerequisite check only.
#
# Status: print-only. Run once on Linux with no Arduino tools installed.
#         Not run against the Arduino IDE or a Pico.
#         This script does not upload, flash, erase, or overwrite anything.
# OS: the Oct 30, 2026 plan names none. This file is POSIX sh.
# Tools: Arduino IDE and the Arduino-Pico core (Earle Philhower).
#        No tool version is pinned, because the plan did not pin one.
# License: The Unlicense. See the repository UNLICENSE file.
#
# Manual fallback (no script required):
#   1. Install the Arduino IDE.
#   2. Add the Arduino-Pico board URL from its README:
#      https://github.com/earlephilhower/arduino-pico
#   3. Select the board "Raspberry Pi Pico".
#   4. For the first milestone, start from the IDE Blink example and
#      print hello once a second. The plan did not paste that sketch.
#   5. For the clock, open m2-adc-clock.ino and upload it yourself,
#      only when you mean to. The sketch is untested.

echo "Status: untested prerequisite check."
echo "This script will not install software, write a sketch, or upload to a board."
echo "OS seen right now: $(uname -s) $(uname -r)"
echo "The plan does not require this OS, and it pins no tool version."

if command -v arduino-cli >/dev/null 2>&1; then
  echo "arduino-cli is on PATH. Reported version:"
  arduino-cli version || echo "arduino-cli was found, but 'version' failed. Nothing was changed."
else
  echo "arduino-cli: not on PATH. That is fine. The plan uses the Arduino IDE."
fi

if command -v arduino >/dev/null 2>&1; then
  echo "arduino: found at $(command -v arduino). This script will not launch it."
else
  echo "arduino: not on PATH."
fi

echo
echo "Sketch files (this script does not open or upload them):"
echo "  scripts/binaural-recorder/m2-adc-clock/m2-adc-clock.ino"
echo "    untested clock sketch copied from the plan"
echo "  scripts/binaural-recorder/m3-i2s-fragment.txt"
echo "    untested and incomplete. Not an Arduino sketch, so it cannot be uploaded as one."

echo
echo "Manual fallback:"
echo "  1. Install the Arduino IDE."
echo "  2. Add the Arduino-Pico board URL printed in its README:"
echo "     https://github.com/earlephilhower/arduino-pico"
echo "  3. Select \"Raspberry Pi Pico\"."
echo "  4. Milestone 1: IDE Blink example, plus a line that prints hello once a second."
echo "  5. Milestone 2: open m2-adc-clock.ino and upload it only when you mean to."

if [ "$#" -gt 0 ]; then
  echo
  echo "Arguments were ignored: $*"
  echo "Refusing to flash or overwrite. Upload only from the Arduino IDE, by hand."
fi

exit 0
