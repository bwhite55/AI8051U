#include "gpio.h"

void gpio_init(void)
{
    // GPIO初始化代码（如果有需要）
    P0M0 = 0x00; // 设置P0为准双向口
    P0M1 = 0x00; // 设置P0为准双向口
    P1M0 = 0x00; // 设置P1为准双向口
    P1M1 = 0x00; // 设置P1为准双向口
    P2M0 = 0x00; // 设置P2为准双向口
    P2M1 = 0x00; // 设置P2为准双向口
    P3M0 = 0x00; // 设置P3为准双向口
    P3M1 = 0x00; // 设置P3为准双向口
    P4M0 = 0x00; // 设置P4为准双向口
    P4M1 = 0x00; // 设置P4为准双向口
    P5M0 = 0x00; // 设置P5为准双向口
    P5M1 = 0x00; // 设置P5为准双向口
}

void gpio_set_output(unsigned char port, unsigned char pin)
{
    switch (port)
    {
        case 0:
            P0M0 |= (1 << pin);
            P0M1 &= ~(1 << pin);
            break;
        case 1:
            P1M0 |= (1 << pin);
            P1M1 &= ~(1 << pin);
            break;
        case 2:
            P2M0 |= (1 << pin);
            P2M1 &= ~(1 << pin);
            break;
        case 3:
            P3M0 |= (1 << pin);
            P3M1 &= ~(1 << pin);
            break;
        case 4:
            P4M0 |= (1 << pin);
            P4M1 &= ~(1 << pin);
            break;
        case 5:
            P5M0 |= (1 << pin);
            P5M1 &= ~(1 << pin);
            break;
        default:
            // 无效端口处理
            break;
    }
}

void gpio_set_input(unsigned char port, unsigned char pin)
{
    switch (port)
    {
        case 0:
            P0M0 &= ~(1 << pin);
            P0M1 |= (1 << pin);
            break;
        case 1:
            P1M0 &= ~(1 << pin);
            P1M1 |= (1 << pin);
            break;
        case 2:
            P2M0 &= ~(1 << pin);
            P2M1 |= (1 << pin);
            break;
        case 3:
            P3M0 &= ~(1 << pin);
            P3M1 |= (1 << pin);
            break;
        case 4:
            P4M0 &= ~(1 << pin);
            P4M1 |= (1 << pin);
            break;
        case 5:
            P5M0 &= ~(1 << pin);
            P5M1 |= (1 << pin);
            break;
        default:
            // 无效端口处理
            break;
    }
}

void gpio_write(unsigned char port, unsigned char pin, unsigned char value)
{
    switch (port)
    {
        case 0:
            value ? (P0 |= (1 << pin)) : (P0 &= ~(1 << pin));
            break;
        case 1:
            value ? (P1 |= (1 << pin)) : (P1 &= ~(1 << pin));
            break;
        case 2:
            value ? (P2 |= (1 << pin)) : (P2 &= ~(1 << pin));
            break;
        case 3:
            value ? (P3 |= (1 << pin)) : (P3 &= ~(1 << pin));
            break;
        case 4:
            value ? (P4 |= (1 << pin)) : (P4 &= ~(1 << pin));
            break;
        case 5:
            value ? (P5 |= (1 << pin)) : (P5 &= ~(1 << pin));
            break;
        default:
            break;
    }
}

unsigned char gpio_read(unsigned char port, unsigned char pin)
{
    unsigned char val = 0;
    switch (port)
    {
        case 0: val = (P0 & (1 << pin)); break;
        case 1: val = (P1 & (1 << pin)); break;
        case 2: val = (P2 & (1 << pin)); break;
        case 3: val = (P3 & (1 << pin)); break;
        case 4: val = (P4 & (1 << pin)); break;
        case 5: val = (P5 & (1 << pin)); break;
        default: return 0;
    }
    return val ? 1 : 0;
}
