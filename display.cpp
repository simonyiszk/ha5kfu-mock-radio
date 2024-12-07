#include "display.hpp"
#include "pins_config.hpp"
#include "Arduino.h"

volatile uint32_t COUNT = 1000;
volatile bool DIRTY = true;

void updateDisplay(void*);

void setupDisplay() {
  pinMode(PIN_DISPLAY_CLK, OUTPUT);
  pinMode(PIN_DISPLAY_CLR, OUTPUT);
  pinMode(PIN_DISPLAY_LD, OUTPUT);
  // put your setup code here, to run once:
  xTaskCreatePinnedToCore(
    updateDisplay,    // Function that should be called
    "Update display",   // Name of the task (for debugging)
    1000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    NULL,             // Task handle
    1
  );
}

void setDisplay(uint32_t value) {
  COUNT = value;
  DIRTY = true;
}




void updateDisplay(void*) {
  for(;;) {
    if (!DIRTY) {
      vTaskDelay(10 / portTICK_PERIOD_MS);
      continue;
    }
    DIRTY = false;

    digitalWrite(PIN_DISPLAY_LD, 1);
    vTaskDelay(1 / portTICK_PERIOD_MS);
    digitalWrite(PIN_DISPLAY_CLR, 0);
    vTaskDelay(1 / portTICK_PERIOD_MS);
    digitalWrite(PIN_DISPLAY_CLR, 1);

    vTaskDelay(1 / portTICK_PERIOD_MS);

    for (uint32_t c = 0; c<COUNT; c++) {
      digitalWrite(PIN_DISPLAY_CLK, 0);
      for(uint32_t i = 0; i<100;i++){}
      digitalWrite(PIN_DISPLAY_CLK, 1);
    }

    digitalWrite(PIN_DISPLAY_LD, 0);
  }
}
