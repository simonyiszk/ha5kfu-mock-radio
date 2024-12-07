/**
"microcoded morse sender", sending dozens of messages in parallel

Can 'send'
- ASCII letters (small and capital)
- numbers
- spaces
- '\0' terminator meaning "repeat with end-of-transmission pause"
- special control character '@' meaing "repeat from start without end-of-transmission pause"


Internally, it uses a translation system to translate each letter into a series of equal-timed steps. 
Timing:
- short symbol        1
- long symbol         3
- between symbols     1 pause
- between letters     3 pauses    
- between words       7 pauses    (a SPACE typed in the message)
- end of transmission 14 pauses

Letters are translated to their full representation, like 'A' is ". ---   ", meaning
- ON  for 1 period
- OFF for 1 period
- ON  for 3 period
- OFF for 3 period
The last one adds the pause between letters. The '\0' terminator is also interpreted as "goto next letter immediately, and redo parsing", resulting in the next letter's first symbol going out

Possible INTERNAL codes are:
- '.'   (dih)               ->  set output to 1 for current period, continue
- '-'   (dah)               ->  same as dih
- ' '   (space)             ->  set output to 0 for current period, continue
- '\0'  (null terminator)   ->  increment letter counter, re-parse next letter in same period
- '@'   (reset control)     ->  reset to start of word, re-parse in the same period
*/

#include<stdint.h>

typedef struct {
  uint32_t freq; // frequency of transmission - not used by the morse engine
  const char* msg; // ONLY ASCII LETTERS OR NUMBERS!!
  uint8_t letter; // which letter are we sending. For internal use
  uint8_t phase; // which sub-character are we sending. For internal use
  bool state; // current state of output. Should be read from main program
} cw_msg;


// should be setup somewhere
extern cw_msg msgs[];

// must be set to sizeof(msgs)/sizeof(msgs[0])
extern uint32_t msg_cnt;


/*
Update current state of ALL messages. Should be called on a slow-ish clock (~200-300ms) equaling the short symbol ('dih')
*/
void update_msgs();