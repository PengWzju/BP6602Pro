#ifndef USART_H
#define USART_H
#include<Arduino.h>

#define USART_COMMAND "home.t0.txt=\""

void print_txt(char string[], char com[]);

void print_txt_string(char string[], double value, byte floatbit = 1);

void update_ui();

extern int ui_refresh_count;

#endif
