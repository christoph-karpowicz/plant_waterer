#ifndef DISPLAY_H
#define DISPLAY_H

/*
    10             13
   ___            ___
  |   |          |   |
8 |_9_| 11    12 |_2_|  14
  |   |          |   |
15|___| 6  .5  4 |___|  1  .0
    7              3 
*/
#define EMPTY 0b1111111111111111
#define HI 0b0111010100100110
#define ERROR 0b1001011000010000
#define DOTS 0b0111101111111111

#define DISPLAY_EXIT 0b1010111001011000
#define DISPLAY_BACK 0b1111011101110110
#define DISPLAY_INTERVAL_SETTING_OPTION 0b1001011111110101

void init_display();
void display_number(uint8_t number);
void display(uint16_t output);

#endif