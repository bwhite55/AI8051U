// ai8051u_conf.h
#ifndef __AI8051U_CONF_H
    //主频定义
    #define FOSC 24000000UL
    #define __AI8051U_CONF_H
    /* I2C相关的配置*/
    // 0: P2.4(SCL)/P2.3(SDA)
    // 1: P1.1(SCL)/P1.0(SDA)
    // 2: P3.7(SCL)/P3.6(SDA)
    // 3: P0.5(SCL)/P0.4(SDA)
    //请选择I2C_PIN_MAP
    #define I2C_PIN_MAP  0

    #define I2C_MODE_MASTER   0
    #define I2C_MODE_SLAVE    1

    //Please define I2C_MODE as I2C_MODE_MASTER or I2C_MODE_SLAVE
    #define I2C_MODE   I2C_MODE_SLAVE
    //Define I2C slave address when I2C_MODE is I2C_MODE_SLAVE
    #define I2C_SLAVE_ADDR_7BIT  0x5A   //7-bit I2C slave address


    /*SPI相关的配置*/
    //0：P1.4(SS)/P1.5(MOSI)/P1.6(MISO)/P1.7(SCK)
    //1：P2.4(SS)/P2.5(MOSI)/P2.6(MISO)/P2.7(SCK)
    //2：P4.0(SS)/P4.1(MOSI)/P4.2(MISO)/P4.3(SCK)
    //3：P3.5(SS)/P3.6(MOSI)/P3.7(MISO)/P3.4(SCK)
    //请选择SPI_PIN_MAP
    #define SPI_PIN_MAP  0

    #define SPI_MODE_MASTER   0
    #define SPI_MODE_SLAVE    1
    //Please define SPI_MODE as SPI_MODE_MASTER or SPI_MODE_SLAVE
    #define SPI_MODE   SPI_MODE_MASTER
    
    #define SPI_CLOCK_DIV  4  //SPI时钟分频系数，支持2、4、8、16、32、64、128
    
    
    /*TFT相关的配置*/
    // 数据宽度选择：0=8位，1=16位
    #define TFT_DATA_WIDTH_16BIT    1

    // 接口模式：0=i8080, 1=M6800
    #define TFT_MODE_I8080          0

    // 控制信号引脚选择（LCMIFCPS[1:0]）
    // 00: RS=P4.5, RD=P4.4, WR=P4.2
    // 01: RS=P4.5, RD=P3.7, WR=P3.6
    // 10: RS=P4.0, RD=P4.4, WR=P4.2
    // 11: RS=P4.0, RD=P3.7, WR=P3.6
    #define TFT_CONTROL_PIN_SELECT  0x00

    // 数据建立时间（SETUPT[2:0]）：0～7，单位为系统时钟周期
    #define TFT_SETUP_TIME          0x01  // 建议 1～3

    // 数据保持时间（HOLDT[1:0]）：0～3，单位为系统时钟周期
    #define TFT_HOLD_TIME           0x01  // 建议 1～2

    // 时钟预分频（LCMIFPSCR[7:0]）：分频后频率 = SYSCLK / (LCMIFPSCR + 1)
    // 若 SYSCLK=22.1184MHz，设置为 0x0F，则 LCMIF 时钟 = 22.1184 / 16 ≈ 1.38MHz
    #define TFT_CLOCK_PRESCALE      0x0F

    // 是否启用中断（暂不推荐，除非需要异步处理）
    #define TFT_ENABLE_INTERRUPT    0

    //usart相关的配置
    // 选择要使用的串口（可选多个）
    #define USART1_ENABLE     1
    #define USART2_ENABLE     1
    #define UART3_ENABLE      0
    #define UART4_ENABLE      0

    // usart串口模式：0:SM[0,1]=00(同步移位串行)
    //               1:SM[0,1]=01(可变波特率8位数据)
    //               2:SM[0,1]=10(固定波特率9位数据)
    //               3:SM[0,1]=11(可变波特率9位数据)
    #define USART1_MODE       0
    #define USART2_MODE       0
    //uart串口模式：0:SM0=0(8位数据)
    //             1:SM0=1(9位数据)
    #define UART3_MODE        0
    #define UART4_MODE        0

    // 波特率（单位 bps）
    #define USART1_BAUDRATE   2000000
    #define USART2_BAUDRATE   9600
    #define UART3_BAUDRATE    115200
    #define UART4_BAUDRATE    9600

    // 引脚选择（根据手册）
    #define USART1_PIN     0  // 0: P3.0, 1: P3.6, 2: P1.6, 3: P4.3 (S1_S[1:0])
    #define USART2_PIN     0  // 0: P1.2, 1: P4.2 (S2_S)
    #define UART3_PIN      0  // 0: P0.0, 1: P5.0 (S3_S)
    #define UART4_PIN      0  // 0: P0.2, 1: P5.2 (S4_S)

    // 是否启用中断（推荐使用中断）
    #define USART1_INTERRUPT  1
    #define USART2_INTERRUPT  1
    #define UART3_INTERRUPT   0
    #define UART4_INTERRUPT   0

    
#endif // __AI8051U_CONF_H