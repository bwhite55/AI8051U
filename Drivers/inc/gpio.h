#ifndef __GPIO_H__
#define __GPIO_H__

#include "AI8051U.H"
#include "ai8051u_conf.h"

void gpio_init(void);
void gpio_set_output(unsigned char port, unsigned char pin);//port:0~5, pin:0~7 make pin as output
void gpio_set_input(unsigned char port, unsigned char pin);//port:0~5, pin:0~7 make pin as input
void gpio_write(unsigned char port, unsigned char pin, unsigned char value);//port:0~5, pin:0~7, value:0/1 write value to pin
unsigned char gpio_read(unsigned char port, unsigned char pin);//port:0~5, pin:0~7 read value from pin

#endif // __GPIO_H__
