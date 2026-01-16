// adc.c
#include "adc.h"
#include "ai8051u_conf.h"  // 假设有系统时钟定义

// 默认配置
#define DEFAULT_ADC_SPEED     ADC_SPEED_DIV16
#define DEFAULT_SAMPLE_TIME   ADC_SAMPLE_TIME_10CLK
#define DEFAULT_FORMAT        ADC_FORMAT_LEFT_ALIGN
#define DEFAULT_AUTO_COUNT    0


// 全局变量：存储当前 Vcc（mV）
static float g_vcc_mV = 0.0f;

// 初始化 ADC
void adc_init(const ADC_InitTypeDef* init) {
    if (init == NULL) return;

    // 1. 设置 ADC 时序控制寄存器 ADCTIM
    ADCTIM = 0;
    ADCTIM |= (1 << 7);           // CSSETUP = 1（Tsetup=2）
    ADCTIM |= (1 << 5);           // CSHOLD = 1（Thold=2）
    ADCTIM |= (init->sample_time & 0x1F); // SMPDUTY[4:0]

    // 2. 设置 ADC 配置寄存器 ADCCFG
    ADCCFG = 0;
    ADCCFG |= ((init->format == ADC_FORMAT_RIGHT_ALIGN) ? 1 : 0) << 5;  // RESFMT
    ADCCFG |= (init->speed & 0x0F);                                   // SPEED[3:0]

    // 3. 设置扩展配置寄存器 ADCEXCFG
    ADCEXCFG = 0;
    ADCEXCFG |= (init->auto_convert_count << 0); // CVTIMESSEL[2:0]
    ADCEXCFG |= (0 << 4);                          // ADCTRS = 0（禁止 ETR 触发）

    // 4. 关闭 ADC 电源
    ADC_CONTR &= ~0x80;  // ADC_POWER = 0

    // 5. 等待 1ms，让 ADC 电源稳定
    delay_ms(1);

    // 6. 打开 ADC 电源
    ADC_CONTR |= 0x80;   // ADC_POWER = 1

    // 7. 选择默认通道（可选）
    adc_start_conversion(init->channel);
}

// 启动一次 ADC 转换
void adc_start_conversion(ADC_Channel_TypeDef channel) {
    // 清除标志位
    ADC_CONTR &= ~0x20;  // ADC_FLAG = 0

    // 选择通道
    ADC_CONTR &= ~0x0F;  // 清除 ADC_CHS[3:0]
    ADC_CONTR |= (channel & 0x0F);  // 设置通道

    // 启动转换
    ADC_CONTR |= 0x40;  // ADC_START = 1
}

// 获取 ADC 转换结果（12位）
unsigned int adc_get_result(void) {
    while (!(ADC_CONTR & 0x20));  // 等待 ADC_FLAG = 1

    // 读取结果
    unsigned int result = 0;
    unsigned char res_high = ADC_RES;
    unsigned char res_low = ADC_RESL;

    // 根据 RESFMT 判断数据格式
    if (ADCCFG & 0x20) {  // RESFMT = 1，右对齐
        result = ((res_high & 0x0F) << 8) | res_low;
    } else {  // RESFMT = 0，左对齐
        result = ((res_high & 0xF0) << 4) | (res_low >> 4);
    }

    return result;
}

// 读取原始 ADC 值（0～4095）
unsigned int adc_read_raw(ADC_Channel_TypeDef channel) {
    adc_start_conversion(channel);
    return adc_get_result();
}

// 读取电压值（mV），使用当前 Vcc 作为参考
float adc_read_voltage(ADC_Channel_TypeDef channel) {
    if (g_vcc_mV <= 0) {
        g_vcc_mV = adc_calibrate_vcc();  // 若未校准，则先测 Vcc
    }

    unsigned int raw = adc_read_raw(channel);
    return (raw * g_vcc_mV) / 4096.0f;
}

// 校准 Vcc：使用内部 1.19V 通道测量当前 Vcc
float adc_calibrate_vcc(void) {
    // 1. 选择内部 1.19V 通道（通道 15）
    adc_start_conversion(ADC_CHANNEL_15_INTERNAL_REF);

    // 2. 读取 ADC 值
    unsigned int raw = adc_get_result();

    // 3. 计算 Vcc = 1.19V * (4096 / ADC_Value)
    // 注意：手册说内部参考源有 ±1% 误差，这里按 1.19V 计算
    float vcc_mV = (1.19f * 1000.0f * 4096.0f) / raw;

    // 存储到全局变量
    g_vcc_mV = vcc_mV;

    return vcc_mV;
}