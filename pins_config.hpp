/**
Pin assignments file
HA7DN, 2024, Mock Radio Project
*/

/*
See display.hpp
*/
const int PIN_DISPLAY_CLK = 27;
const int PIN_DISPLAY_CLR = 26;
const int PIN_DISPLAY_LD =  25;

/*
See cw.hpp
*/
const int PIN_CW = 32;
const int PIN_BUZZER = 33;

/*
Uses ai_rotary_encoder library. BTN is not used. If encoder works the wrong way, swap A and B either here or by wiring.
*/
const int PIN_ROT_A = 34;
const int PIN_ROT_B = 35;
const int PIN_ROT_BTN = 19; // not actually used