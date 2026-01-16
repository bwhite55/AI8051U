#include "timer.h"

// 根据 FOSC 自动计算重装载值 (1T模式)
#define TICKS_PER_MS (FOSC / 1000)
#define TICKS_PER_US (FOSC / 1000000)
#define RELOAD_VAL_1MS (65536UL - TICKS_PER_MS)

void delay_ms(unsigned int ms)
{
    unsigned int i;
    AUXR |= 0x80; // 定时器0设为1T模式
    TMOD &= 0xF0; // 定时器0设为模式0 (16位自动重载)
    
    // 即使是自动重载模式，为了兼容性和明确性，我们在循环中赋值
    // 如果是单次延时，也可以利用自动重载特性
    
    for (i = 0; i < ms; i++)
    {
        TL0 = (unsigned char)(RELOAD_VAL_1MS);
        TH0 = (unsigned char)(RELOAD_VAL_1MS >> 8);
        TR0 = 1;        // 启动定时器
        while (!TF0);   // 等待溢出
        TR0 = 0;        // 停止定时器
        TF0 = 0;        // 清除标志
    }
}

void delay_us(unsigned int us)
{
    unsigned long ticks;
    unsigned int reload;

    // 16位定时器最大延时约为 2.7ms (24MHz下)
    // 如果延时较长 (>2000us)，先调用 delay_ms 处理大部分时间
    if (us >= 1000)
    {
        delay_ms(us / 1000);
        us = us % 1000;
        if (us == 0) return;
    }

    // 计算所需 ticks
    ticks = (unsigned long)us * TICKS_PER_US;
    reload = 65536UL - ticks;

    AUXR |= 0x80; // 1T模式
    TMOD &= 0xF0; // 模式0
    
    TL0 = (unsigned char)(reload);
    TH0 = (unsigned char)(reload >> 8);
    TR0 = 1;
    while (!TF0);
    TR0 = 0;
    TF0 = 0;
}