/**
Driver for the "Display" peripheral
Random 4xTIL306 (iirc) based display found in the junkbox
Pin assignments can be configured in pins_config.hpp
Wiring:
PIN_DISPLAY_CLK -> CLK
PIN_DISPLAY_CLR -> CLR
PIN_DISPLAY_LD  -> LD
Other pins of the display are GND / 5V per datasheet
zero suppression should be turned off, along with any decimals

This driver uses a background task to bitbang the control signals asynchronously. Since the display is just a counter, every refresh needs to send up to 9999 impulses, so it's not very efficient. Might be possible to use timers, but I was in a hurry, and it worked out well.

HA7DN, 2024, Mock Radio Project
*/

#include<stdint.h>

/**
Start background task, setup pins
*/
void setupDisplay();

/**
Set display to value. Does not block, display will be set in the background.
*/
void setDisplay(uint32_t value);