#include "usart.h"

#define SM0_FE    0x80  // SCON.7
#define REN       0x10  // SCON.4
#define TB8       0x08  // SCON.3
#define RB8       0x04  // SCON.2
#define TI        0x02  // SCON.1
#define RI        0x01  // SCON.0

#define S2SM0_FE  0x80  // S2CON.7
#define S2REN     0x10  // S2CON.4
#define S2TB8     0x08  // S2CON.3
#define S2RB8     0x04  // S2CON.2
#define S2TI      0x02  // S2CON.1
#define S2RI      0x01  // S2CON.0

// 引脚选择位
#define S1_S_MASK   0x03  // P_SW1.B7:B6
#define S2_S_BIT    0x04  // P_SW2.B2
#define S3_S_BIT    0x02  // P_SW2.B1
#define S4_S_BIT    0x01  // P_SW2.B0

static unsigned int calculate_baudrate(unsigned long sysclk, unsigned long baud) 
{
    return (sysclk / (baud * 16)) - 1;
}

void usart_init(USART_TypeDef usart)
{
    unsigned int br_value;
    unsigned char sw_reg;

    switch (usart) {
        case USART1:
            if (USART1_ENABLE) 
            {
                // 设置引脚
                P_SW1 = 0x00;
                P_SW1 |= (USART1_PIN & 0x03) << 6;

                // 设置模式
                SCON = 0;
                if (USART1_MODE == 0) 
                {  // 同步模式,rx为数据口,tx为同步移位脉冲输出脚，低位在先
                    SM0 = 0;
                    SM1 = 0;
                    S1M0x6 = 0; //波特率固定为12分频(SYSclk/12)
                } 
                else if(USART1_MODE == 1)
                {               // 8位可变波特率
                    SM0 = 0;
                    SM1 = 1;
                }
                else if(USART1_MODE == 2)
                {               // 9位固定波特率
                    SM0 = 1;
                    SM1 = 0;
                    SM2 = 1;
                }
                else if(USART1_MODE == 3)
                {               // 9位可变波特率
                    SM0 = 1;
                    SM1 = 1;
                    SM2 = 1;
                }
                SCON |= REN;           // 允许接收
                S1BRT = 1;          // 波特率发生器工作在方式1,选择定时器2作为波特率发生器
                // 设置波特率
                // br_value = calculate_baudrate(FOSC, USART1_BAUDRATE);
                // USARTBR = br_value;

                // 启用中断（如果配置）
                if (USART1_INTERRUPT) 
                {
                    IE |= 0x04;  // EA=1, ES=1
                }
            }
            break;

        case USART2:
            if (USART2_ENABLE) 
            {
                // 设置引脚
                P_SW2 = 0x00;
                if(USART2_PIN == 0)
                {
                    S2_S = 0;
                }
                else if(USART2_PIN == 1)
                {
                    S2_S = 1;
                }

                // 设置模式
                S2CON = 0;
                if (USART2_MODE == 0) 
                {
                    S2SM0 = 0;
                    S2SM1 = 0;
                } 
                else if(USART2_MODE == 1)
                {
                    S2SM0 = 0;
                    S2SM1 = 1;
                }
                else if(USART2_MODE == 2)
                {
                    S2SM0 = 1;
                    S2SM1 = 0;
                    S2SM2 = 1;
                }
                else if(USART2_MODE == 3)
                {
                    S2SM0 = 1;
                    S2SM1 = 1;
                    S2SM2 = 1;
                }
                S2CON |= S2REN; //使能usart2接收
                S2CFG = 0x01;   //波特率不加倍,无帧检错
                // 设置波特率
                // br_value = calculate_baudrate(FOSC, USART2_BAUDRATE);
                // USART2BR = br_value;

                if (USART2_INTERRUPT) 
                {
                    IE |= 0x08;  // ES2=1
                }
            }
            break;

        case UART3:
            if (UART3_ENABLE) 
            {
                // 设置引脚
                if(UART3_PIN == 0)
                {
                    S3_S = 0;
                }
                else if(UART3_PIN == 1)
                {
                    S3_S = 1;
                }

                // 设置模式
                S3CON = 0;
                if (UART3_MODE == 0) 
                {
                    S3SM0 = 0;
                } 
                else if (UART3_MODE == 1)
                {
                    S3SM0 = 1;
                    S3SM2 = 1;
                }
                S3ST3 = 0;  // 选择定时器2作为波特率发生器
                S3CON |= S3REN;

                // 设置波特率
                // br_value = calculate_baudrate(FOSC, UART3_BAUDRATE);
                // 假设 UART3 波特率寄存器为 7EFDCD 或类似
                // 此处需查手册确认
                // USART3BR = br_value;

                if (UART3_INTERRUPT) 
                {
                    IE |= 0x10;  // ES3=1
                }
            }
            break;

        case UART4:
            if (UART4_ENABLE) 
            {
                // 设置引脚
                if(UART4_PIN == 0)
                {
                    S4_S = 0;
                }
                else if(UART4_PIN == 1)
                {
                    S4_S = 1;
                }

                // 设置模式
                S4CON = 0;
                if (UART4_MODE == 0) 
                {
                    S4SM0 = 0;
                } 
                else if (UART4_MODE == 1)
                {
                    S4SM0 = 1;
                    S4SM2 = 1;
                }
                S4ST4 = 0;  // 选择定时器2作为波特率发生器
                S4CON |= S4REN;

                // 设置波特率
                // br_value = calculate_baudrate(FOSC, UART4_BAUDRATE);
                // USART4BR = br_value;

                if (UART4_INTERRUPT) 
                {
                    IE |= 0x20;  // ES4=1
                }
            }
            break;
    }
}

void usart_send_byte(USART_TypeDef usart, unsigned char usart_data) {
    switch (usart) {
        case USART1:
            while (!(SCON & TI));  // 等待发送完成
            SBUF = usart_data;
            break;
        case USART2:
            while (!(S2CON & S2TI));
            S2BUF = usart_data;
            break;
        case UART3:
            while (!(S3CON & S3TI));
            S3BUF = usart_data;
            break;
        case UART4:
            while (!(S4CON & S4TI));
            S4BUF = usart_data;
            break;
    }
}

// 接收字节（轮询）
unsigned char usart_receive_byte(USART_TypeDef usart) {
    unsigned char usart_data = 0;
    switch (usart) {
        case USART1:
            while (!(SCON & RI));
            usart_data = SBUF;
            break;
        case USART2:
            while (!(S2CON & S2RI));
            usart_data = S2BUF;
            break;
        case UART3:
            while (!(S3CON & S3RI));
            usart_data = S3BUF;
            break;
        case UART4:
            while (!(S4CON & S4RI));
            usart_data = S4BUF;
            break;
    }
    return usart_data;
}

// 发送缓冲区
void usart_send_buffer(USART_TypeDef usart, const unsigned char* buf, unsigned int len) {
    for (unsigned int i = 0; i < len; i++) {
        usart_send_byte(usart, buf[i]);
    }
}

// 接收缓冲区
void usart_receive_buffer(USART_TypeDef usart, unsigned char* buf, unsigned int len) {
    for (unsigned int i = 0; i < len; i++) {
        buf[i] = usart_receive_byte(usart);
    }
}

// 启用中断（示例）
void usart_interrupt_enable(USART_TypeDef usart) {
    switch (usart) {
        case USART1:
            IE |= 0x04;
            break;
        case USART2:
            IE |= 0x08;
            break;
        case UART3:
            IE |= 0x10;
            break;
        case UART4:
            IE |= 0x20;
            break;
    }
}

void usart_interrupt_disable(USART_TypeDef usart) {
    switch (usart) {
        case USART1:
            IE &= ~0x04;
            break;
        case USART2:
            IE &= ~0x08;
            break;
        case UART3:
            IE &= ~0x10;
            break;
        case UART4:
            IE &= ~0x20;
            break;
    }
}