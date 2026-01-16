#ifndef __TFT_H__
#define __TFT_H__

#include "AI8051U.H"
#include "ai8051u_conf.h"

void tft_init(void);
void tft_write_command(unsigned short cmd);
void tft_write_data(unsigned short data);
void tft_write_data_buffer(const unsigned short* buf, unsigned int len);
void tft_write_command_data(unsigned short cmd, unsigned short data);

#endif // __TFT_H__