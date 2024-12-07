#include "sender.hpp"
#include "Arduino.h"

const char* ALPHABET_LETTERS[26][2] = {
    "A", ". ---   ",
    "B", "--- . . .   ",
    "C", "--- . --- .   ",
    "D", "--- . .   ",
    "E", ".   ",
    "F", ". . --- .   ",
    "G", "--- --- .   ",
    "H", ". . . .   ",
    "I", ". .   ",
    "J", ". --- --- ---   ",
    "K", "--- . ---   ",
    "L", ". --- . .   ",
    "M", "--- ---   ",
    "N", "--- .   ",
    "O", "--- --- ---   ",
    "P", ". --- --- .  ",
    "Q", "--- --- . ---   ",
    "R", ". --- .   ",
    "S", ". . .   ",
    "T", "---   ",
    "U", ". . ---   ",
    "V", ". . . ---   ",
    "W", ". --- ---   ",
    "X", "--- . . ---   ",
    "Y", "--- . --- ---   ",
    "Z", "--- --- . .   ",
};

const char* ALPHABET_NUMBERS[10][2] = {
    "0", "--- --- --- --- ---   ",
    "1", ". --- --- --- ---   ",
    "2", ". . --- --- ---   ",
    "3", ". . . --- ---   ",
    "4", ". . . . ---   ",
    "5", ". . . . .   ",
    "6", "--- . . . .   ",
    "7", "--- --- . . .   ",
    "8", "--- --- --- . .   ",
    "9", "--- --- --- --- .   ",
};


void update_msgs() {
  for (int i = 0; i<msg_cnt; i++) {
    cw_msg *msg = &msgs[i];

GET_LETTER:
    char curr_letter = msg->msg[msg->letter];

    // translate to capital letter
    if (curr_letter >= 'a' && curr_letter <= 'z') {
      curr_letter = curr_letter - 'a' + 'A';
    }
    
    // now translate letter to codes
    const char* curr_cw;
    if (curr_letter >= 'A' && curr_letter <= 'Z') {
      // letter found, look up
      curr_cw = ALPHABET_LETTERS[curr_letter - 'A'][1];
    } else if (curr_letter >= '0' && curr_letter <= '9') {
      // number found, look up
      curr_cw = ALPHABET_NUMBERS[curr_letter - '0'][1];
    } else if (curr_letter == ' ') {
      // space found, send 7 pauses
      curr_cw = "    "; // 3 pauses from letter, 4 more (must be 7)
    } else if (curr_letter == '\0') {
      // end of word reached, 14 pauses
      curr_cw = "           @"; // 3 spaces from letter, 11 more spaces, reset word
    } else if (curr_letter == '@') {
      // control character for immediate reset
      curr_cw = "@";
    } else {
      // never crash, fail silently
      curr_cw = " ";
    }

    char curr_op = curr_cw[msg->phase];
    switch (curr_op) {
      case '.':
      case '-':
        msg->state = 1;
        msg->phase++;
        break;
      case ' ':
        msg->state = 0;
        msg->phase++;
        break;
      case '\0':
        msg->letter++;
        msg->phase = 0;
        goto GET_LETTER;
      case '@':
        msg->letter = 0;
        msg->phase = 0;
        goto GET_LETTER;
      default:
        msg->letter = 0;
        msg->phase = 0;
        goto GET_LETTER;
    }
  }
}
