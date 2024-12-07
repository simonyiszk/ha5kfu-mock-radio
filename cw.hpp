/**
Driver for the "CW" peripheral
i.e. buzzer & CW display
Pin assignments can be configured in pins_config.hpp
Wiring:
- buzzer on PIN_BUZZER, can be connected directly
- PWM driven (with external random transistor with enough current) deprez-meter & lightbulb on PIN_CW

HA7DN, 2024, Mock Radio Project
*/

#include<stdint.h>

/**
"OFF" value for PWM. The light dims very slow if it's cold, so keep it warm just enough to make it fast.
*/
const uint32_t CW_POWERDOWN_VALUE = 20;

/**
Setup pins & stuff
*/
void setupCW();

/**
Based on current freq and freq of transmission, setup values for CW
Calculates and applies tone offset (tone is cw_freq - freq + 1kHz, steps of both are 200 Hz) and power levels
(maximum at freq=cw_freq, dims in 5 steps to zero based on absolute difference)
*/
void setTunedFreq(uint32_t freq, uint32_t cw_freq);

/**
Bypass setTunedFreq's calculation for fine-grained control. Values apply directly. Only used with Ricky
*/
void setExactFreqAndPower(uint32_t freq, uint8_t pwr_tone, uint8_t pwr_cw);

/**
Set CW state:
- true enables buzzer and light with values calculated by setTunedFreq or setExactFreqAndPower
- false disables buzzer and sets PWM to CW_POWERDOWN_VALUE
*/
void setCW(bool state);