#ifndef __TIMER_H__
#define __TIMER_H__

#include "AI8051U.H"
#include "ai8051u_conf.h"

void delay_ms(unsigned int ms);
void delay_us(unsigned int us);  // 若精度允许
// 初始化定时器（指定编号、模式、中断回调）

#endif // __TIMER_H__