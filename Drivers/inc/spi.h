#ifndef __SPI_H__
#define __SPI_H__

#include "AI8051U.H"
#include "ai8051u_conf.h"

typedef enum {
    SPI_MODE_0 = 0,  // CPOL=0, CPHA=0: 空闲低，上升沿采样
    SPI_MODE_1 = 1,  // CPOL=0, CPHA=1: 空闲低，下降沿采样
    SPI_MODE_2 = 2,  // CPOL=1, CPHA=0: 空闲高，上升沿采样
    SPI_MODE_3 = 3   // CPOL=1, CPHA=1: 空闲高，下降沿采样
} SPI_Mode_TypeDef;

// SPI 时钟分频设置（SPR[1:0]）
typedef enum {
    SPI_SPEED_DIV4  = 0x00,  // SCLK = SYSclk / 4
    SPI_SPEED_DIV8  = 0x01,  // SCLK = SYSclk / 8
    SPI_SPEED_DIV16 = 0x02,  // SCLK = SYSclk / 16
    SPI_SPEED_DIV2  = 0x03   // SCLK = SYSclk / 2
} SPI_Speed_TypeDef;

// 数据顺序
typedef enum {
    SPI_MSB_FIRST = 0,
    SPI_LSB_FIRST = 1
} SPI_DataOrder_TypeDef;

// SPI 初始化结构体
typedef struct {
    SPI_Mode_TypeDef mode;
    SPI_Speed_TypeDef speed;
    SPI_DataOrder_TypeDef data_order;
    unsigned char enable_interrupt;  // 是否启用中断（暂不实现）
} SPI_InitTypeDef;

// 函数声明
void spi_init(const SPI_InitTypeDef* init);
unsigned char spi_transfer(unsigned char data);
void spi_send(unsigned char data);
unsigned char spi_receive(void);
#endif // __SPI_H__