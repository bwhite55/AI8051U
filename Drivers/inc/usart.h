#ifndef __USART_H__
    #define __USART_H__
    #include "AI8051U.H"
    #include "ai8051u_conf.h"
    // 串口编号枚举
    typedef enum {
        USART1 = 0,
        USART2 = 1,
        UART3  = 2,
        UART4  = 3
    } USART_TypeDef;

    // 函数声明
    void usart_init(USART_TypeDef usart);
    void usart_send_byte(USART_TypeDef usart, unsigned char data);
    unsigned char usart_receive_byte(USART_TypeDef usart);
    void usart_send_buffer(USART_TypeDef usart, const unsigned char* buf, unsigned int len);
    void usart_receive_buffer(USART_TypeDef usart, unsigned char* buf, unsigned int len);

    // 中断相关（如果启用）
    void usart_interrupt_enable(USART_TypeDef usart);
    void usart_interrupt_disable(USART_TypeDef usart);

#endif // __USART_H__