// STATUS: untested.
// Copied from the binaural recorder plan dated Oct 30, 2026.
// Do not treat this as working firmware. It has not been compiled or uploaded
// for this notebook.
//
// OS: not specified in the plan.
// Tools: Arduino IDE + Arduino-Pico core (Earle Philhower). No versions pinned.
// This file does not upload itself. There is no flash flag in this repository.
// check-setup.sh only prints paths. It never opens the IDE or a serial port.
//
// Manual fallback: type this sketch into the Arduino IDE by hand, then upload
// it yourself only when you mean to.
//
// License: The Unlicense. See the repository UNLICENSE file.

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
