#ifndef __ADC_H__
#define __ADC_H__
#include "AI8051U.H"
#include "ai8051u_conf.h"

// ADC 通道枚举（0～14 为外部，15 为内部 1.19V）
typedef enum {
    ADC_CHANNEL_0 = 0,
    ADC_CHANNEL_1,
    ADC_CHANNEL_2,
    ADC_CHANNEL_3,
    ADC_CHANNEL_4,
    ADC_CHANNEL_5,
    ADC_CHANNEL_6,
    ADC_CHANNEL_7,
    ADC_CHANNEL_8,
    ADC_CHANNEL_9,
    ADC_CHANNEL_10,
    ADC_CHANNEL_11,
    ADC_CHANNEL_12,
    ADC_CHANNEL_13,
    ADC_CHANNEL_14,
    ADC_CHANNEL_15_INTERNAL_REF  // 内部 1.19V 参考源
} ADC_Channel_TypeDef;

// ADC 结果格式
typedef enum {
    ADC_FORMAT_LEFT_ALIGN = 0,  // 左对齐：ADC_RES[11:4], ADC_RESL[3:0]
    ADC_FORMAT_RIGHT_ALIGN = 1  // 右对齐：ADC_RES[3:0], ADC_RESL[11:0]
} ADC_Format_TypeDef;

// ADC 时钟分频设置（SPEED[3:0]）
typedef enum {
    ADC_SPEED_DIV1  = 0x00,  // SYSclk/2/1
    ADC_SPEED_DIV2  = 0x01,  // SYSclk/2/2
    ADC_SPEED_DIV3  = 0x02,  // SYSclk/2/3
    ADC_SPEED_DIV4  = 0x03,  // SYSclk/2/4
    ADC_SPEED_DIV5  = 0x04,  // SYSclk/2/5
    ADC_SPEED_DIV6  = 0x05,  // SYSclk/2/6
    ADC_SPEED_DIV7  = 0x06,  // SYSclk/2/7
    ADC_SPEED_DIV8  = 0x07,  // SYSclk/2/8
    ADC_SPEED_DIV9  = 0x08,  // SYSclk/2/9
    ADC_SPEED_DIV10 = 0x09,  // SYSclk/2/10
    ADC_SPEED_DIV11 = 0x0A,  // SYSclk/2/11
    ADC_SPEED_DIV12 = 0x0B,  // SYSclk/2/12
    ADC_SPEED_DIV13 = 0x0C,  // SYSclk/2/13
    ADC_SPEED_DIV14 = 0x0D,  // SYSclk/2/14
    ADC_SPEED_DIV15 = 0x0E,  // SYSclk/2/15
    ADC_SPEED_DIV16 = 0x0F   // SYSclk/2/16
} ADC_Speed_TypeDef;

// ADC 采样时间控制（SMFDUTY[4:0]）
typedef enum {
    ADC_SAMPLE_TIME_1CLK = 0x00,  // 1 个时钟周期
    ADC_SAMPLE_TIME_2CLK = 0x01,  // 2 个时钟周期
    ADC_SAMPLE_TIME_3CLK = 0x02,  // 3 个时钟周期
    ADC_SAMPLE_TIME_4CLK = 0x03,  // 4 个时钟周期
    ADC_SAMPLE_TIME_5CLK = 0x04,  // 5 个时钟周期
    ADC_SAMPLE_TIME_6CLK = 0x05,  // 6 个时钟周期
    ADC_SAMPLE_TIME_7CLK = 0x06,  // 7 个时钟周期
    ADC_SAMPLE_TIME_8CLK = 0x07,  // 8 个时钟周期
    ADC_SAMPLE_TIME_9CLK = 0x08,  // 9 个时钟周期
    ADC_SAMPLE_TIME_10CLK = 0x09, // 10 个时钟周期
    ADC_SAMPLE_TIME_11CLK = 0x0A, // 11 个时钟周期
    ADC_SAMPLE_TIME_12CLK = 0x0B, // 12 个时钟周期
    ADC_SAMPLE_TIME_13CLK = 0x0C, // 13 个时钟周期
    ADC_SAMPLE_TIME_14CLK = 0x0D, // 14 个时钟周期
    ADC_SAMPLE_TIME_15CLK = 0x0E, // 15 个时钟周期
    ADC_SAMPLE_TIME_16CLK = 0x0F  // 16 个时钟周期
} ADC_SampleTime_TypeDef;

// ADC 配置结构体
typedef struct {
    ADC_Channel_TypeDef channel;
    ADC_Speed_TypeDef speed;
    ADC_SampleTime_TypeDef sample_time;
    ADC_Format_TypeDef format;
    unsigned char auto_convert_count;  // 自动转换次数（1～16），0 表示禁用
} ADC_InitTypeDef;

// 函数声明
void adc_init(const ADC_InitTypeDef* init);
unsigned int adc_read_raw(ADC_Channel_TypeDef channel);
float adc_read_voltage(ADC_Channel_TypeDef channel);  // 返回 mV
float adc_calibrate_vcc(void);  // 返回当前 Vcc（mV）
void adc_start_conversion(ADC_Channel_TypeDef channel);
unsigned int adc_get_result(void);
#endif // __ADC_H__