#include "cw.hpp"
#include "pins_config.hpp"
#include "Arduino.h"


bool state = false;

uint32_t tone_freq = 0;
uint32_t tone_ampl = 0;
uint8_t  cw_ampl = 20;


void updateBuzzer();

void setupCW() {
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_CW, OUTPUT);

  updateBuzzer();
}

void setTunedFreq(uint32_t freq, uint32_t cw_freq) {
  tone_freq = (cw_freq + 5 - freq)*200;

  switch (tone_freq) {
    
    case 1000:
      tone_ampl = 10;
      cw_ampl = 246;
      break;

    case 1200:
    case 800:
      tone_ampl = 6;
      cw_ampl = 160;
      break;

    case 1400:
    case 600:
      tone_ampl = 4;
      cw_ampl = 108;
      break;

    case 1600:
    case 400:
      tone_ampl = 2;
      cw_ampl = 72;
      break;

    case 1800:
    case 200:
      tone_ampl = 1;
      cw_ampl = 45;
      break;

    default:
      tone_ampl = 0;
      tone_freq = 1000;
      cw_ampl = CW_POWERDOWN_VALUE;
  }
  updateBuzzer();
}

void setExactFreqAndPower(uint32_t freq, uint8_t pwr_tone, uint8_t pwr_cw) {
  tone_ampl = pwr_tone;
  cw_ampl = pwr_cw;
  tone_freq = freq;
  updateBuzzer();
}

void setExactFreqAndPower(uint32_t freq, uint power) {
  cw_ampl = power;
}

void setCW(bool st) {
  if (state == st)
    return;
  state = st;
  updateBuzzer();
}

void updateBuzzer() {
  analogWrite(PIN_CW, state ? cw_ampl : CW_POWERDOWN_VALUE);
  analogWrite(PIN_BUZZER, state ? tone_ampl : 0);
  analogWriteFrequency(PIN_BUZZER, tone_freq);
}