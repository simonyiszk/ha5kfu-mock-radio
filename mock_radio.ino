// TODO: encoder driver
// TODO: buzzer driver
// TODO: light + meter driver

#include "AiEsp32RotaryEncoder.h"
#include "display.hpp"
#include "cw.hpp"
#include "pins_config.hpp"
#include "sender.hpp"
#include "rick.hpp"

const int ROTARY_ENCODER_STEPS = 4;
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(PIN_ROT_A, PIN_ROT_B, 0, -1, ROTARY_ENCODER_STEPS, false);


uint32_t FREQ = 1000;
uint32_t CW_INDEX = 0;


void sender_task(void*) {
  for(;;) {
    update_msgs();
    if (!ricky_enabled) {
      setCW(msgs[CW_INDEX].state);
    }
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

const uint32_t ricky_center_freq = 6942;

void IRAM_ATTR readEncoderISR()
{
    rotaryEncoder.readEncoder_ISR();
}

void setup() {
  Serial.begin(115200);
  setupDisplay();

  setupCW();

  rotaryEncoder.begin();
  rotaryEncoder.setup(readEncoderISR);
  rotaryEncoder.setBoundaries(0, 9999, true); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
  rotaryEncoder.setEncoderValue(FREQ);

  xTaskCreate(
    sender_task,    // Function that should be called
    "send CW",   // Name of the task (for debugging)
    1000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    NULL             // Task handle
  );

  xTaskCreate(
    ricky_task,    // Function that should be called
    "send ricky",   // Name of the task (for debugging)
    1000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    NULL             // Task handle
  );
}

// make LOOP setup everything in the first run to a default value
bool FIRST_RUN = true;

void loop() {
  if (rotaryEncoder.encoderChanged() || FIRST_RUN)
  {
    FIRST_RUN = false;
    FREQ = rotaryEncoder.readEncoder();
    setDisplay(FREQ);
    
    // find active transmission 
    CW_INDEX = 0;
    for (int i = 0; i<msg_cnt; i++) {
      // ignore not active ones
      if (msgs[i].freq < FREQ - 4 || msgs[i].freq > FREQ + 4) {
        continue;
      }

      CW_INDEX = i;
      
      return; // added after testing, remove me if everything broken
    }

    // all here (except else clause) moved out of the previous FOR loop after testing
    // re-think me if code is broken
    ricky_enabled = msgs[CW_INDEX].freq == ricky_center_freq;
    if (ricky_enabled)
      ricky_freq_offset = (ricky_center_freq - FREQ) * 200;
    else
      setTunedFreq(FREQ, msgs[CW_INDEX].freq);
  }
}
