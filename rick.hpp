/**
Ricky code, used to send a song at a *special* frequency.

Modified code from https://github.com/robsoncouto/arduino-songs to run as a background task, and be enabled selectively
Was quite rushed, not very clean

HA7DN, 2024, Mock Radio Project
*/


#include <cstdint>

extern volatile uint32_t ricky_freq;
extern volatile bool ricky_enabled; 
extern volatile int32_t ricky_freq_offset;

/**
RTOS task, needs to be started elsewhere!
*/
void ricky_task(void*);