#include "tft.h"

#define LCMIFIE_BIT     0x80  // LCMIFCFG.7
#define LCMIFIP_MASK    0x0C  // LCMIFCFG.5:4
#define LCMIFDPS_MASK   0x03  // LCMIFCFG.3:2
#define D16_D8_BIT      0x02  // LCMIFCFG.1
#define M68_I80_BIT     0x01  // LCMIFCFG.0
#define LCMIFCPS_MASK   0x03  // LCMIFCFG2.5:4
#define SETUPT_MASK     0x07  // LCMIFCFG2.3:1
#define HOLDT_MASK      0x03  // LCMIFCFG2.0:1
#define ENLCMIF_BIT     0x80  // LCMIFCR.7
#define ST_ENDIAN_BIT   0x10  // LCMIFCR.4
#define CMD_WRITE_CMD   0x04  // 100
#define CMD_WRITE_DATA  0x05  // 101
#define CMD_READ_CMD    0x06  // 110
#define CMD_READ_DATA   0x07  // 111
#define LCMIFIF_BIT     0x01  // LCMIFSTA.0

// 初始化函数
void tft_init(void) {
    // 1. 配置 LCMIFCFG
    LCMIFCFG = 0;

    // 设置中断使能（如果启用）
    if (TFT_ENABLE_INTERRUPT) {
        LCMIFCFG |= LCMIFIE_BIT;
    }

    // 设置中断优先级（默认最低）
    LCMIFCFG &= ~LCMIFIP_MASK;
    LCMIFCFG |= (0x00 << 4);  // 最低级

    // 设置数据宽度（8/16位）
    if (TFT_DATA_WIDTH_16BIT) {
        LCMIFCFG |= D16_D8_BIT;
    }

    // 设置接口模式（i8080/M6800）
    if (TFT_MODE_I8080) {
        LCMIFCFG &= ~M68_I80_BIT;
    } else {
        LCMIFCFG |= M68_I80_BIT;
    }

    // 设置数据脚选择（LCMIFDPS）
    LCMIFCFG &= ~LCMIFDPS_MASK;
    LCMIFCFG |= (TFT_DATA_WIDTH_16BIT ? 0x03 : 0x00);  // 16位用 P0+P2，8位用 P2

    // 2. 配置 LCMIFCFG2
    LCMIFCFG2 = 0;

    // 设置控制信号引脚
    LCMIFCFG2 &= ~LCMIFCPS_MASK;
    LCMIFCFG2 |= (TFT_CONTROL_PIN_SELECT << 4);

    // 设置建立时间（SETUPT）
    LCMIFCFG2 &= ~SETUPT_MASK;
    LCMIFCFG2 |= (TFT_SETUP_TIME << 1);

    // 设置保持时间（HOLDT）
    LCMIFCFG2 &= ~HOLDT_MASK;
    LCMIFCFG2 |= (TFT_HOLD_TIME);

    // 3. 配置 LCMIFPSCR（时钟预分频）
    LCMIFPSCR = TFT_CLOCK_PRESCALE;

    // 4. 启用 LCMIF 功能
    LCMIFCR |= ENLCMIF_BIT;

    // 5. 设置大小端（仅对 16 位有效）
    if (TFT_DATA_WIDTH_16BIT) {
        LCMIFCR |= ST_ENDIAN_BIT;  // 小端模式（推荐）
    }
}

// 写命令
void tft_write_command(unsigned short cmd) {
    // 设置 CMD[2:0] = 100（写命令）
    LCMIFCR = (LCMIFCR & ~0x07) | CMD_WRITE_CMD;

    // 写入数据
    if (TFT_DATA_WIDTH_16BIT) {
        LCMIFDATH = (cmd >> 8);
        LCMIFDATL = (cmd & 0xFF);
    } else {
        LCMIFDATL = cmd;
    }

    // 等待完成（可选：轮询 LCMIFIF 或使用中断）
    while (LCMIFSTA & LCMIFIF_BIT);
    LCMIFSTA = LCMIFIF_BIT;  // 清除中断标志
}

// 写数据
void tft_write_data(unsigned short tft_data) 
{
    // 设置 CMD[2:0] = 101（写数据）
    LCMIFCR = (LCMIFCR & ~0x07) | CMD_WRITE_DATA;

    // 写入数据
    if (TFT_DATA_WIDTH_16BIT) {
        LCMIFDATH = (tft_data >> 8);
        LCMIFDATL = (tft_data & 0xFF);
    } else {
        LCMIFDATL = tft_data;
    }

    // 等待完成
    while (LCMIFSTA & LCMIFIF_BIT);
    LCMIFSTA = LCMIFIF_BIT;
}

// 写多个数据（批量）
void tft_write_data_buffer(const unsigned short* buf, unsigned int len) 
{
    for (unsigned int i = 0; i < len; i++) {
        tft_write_data(buf[i]);
    }
}

// 写命令 + 数据（常用）
void tft_write_command_data(unsigned short cmd, unsigned short tft_data) 
{
    tft_write_command(cmd);
    tft_write_data(tft_data);
}