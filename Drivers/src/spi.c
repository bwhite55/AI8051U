#include "spi.h"

#define SPIF_BIT     0x80  // SPSTAT.7
#define WCOL_BIT     0x40  // SPSTAT.6
#define SSIG_BIT     0x80  // SPCTL.7
#define SPEN_BIT     0x40  // SPCTL.6
#define DORD_BIT     0x20  // SPCTL.5
#define MSTR_BIT     0x10  // SPCTL.4
#define CPOL_BIT     0x08  // SPCTL.3
#define CPHA_BIT     0x04  // SPCTL.2
#define SPR_MASK     0x03  // SPCTL.1:0

// 初始化 SPI
void spi_init(const SPI_InitTypeDef* init) 
{
    if (init == NULL) return;

    // 1. 关闭 SPI 功能
    SPCTL &= ~SPEN_BIT;

    // 2. 设置模式（MSTR=1，主模式）
    SPCTL |= MSTR_BIT;

    // 3. 设置 CPOL/CPHA（SPI 模式）
    SPCTL &= ~(CPOL_BIT | CPHA_BIT);
    SPCTL |= ((init->mode & 0x02) << 1);  // CPOL
    SPCTL |= ((init->mode & 0x01) << 0);  // CPHA

    // 4. 设置数据顺序（MSB/LSB）
    SPCTL &= ~DORD_BIT;
    if (init->data_order == SPI_LSB_FIRST) 
    {
        SPCTL |= DORD_BIT;
    }

    // 5. 设置时钟分频（SPR[1:0]）
    SPCTL &= ~SPR_MASK;
    SPCTL |= (init->speed & SPR_MASK);

    // 6. 忽略 SS 引脚功能（SSIG=1）
    SPCTL |= SSIG_BIT;

    // 7. 启用 SPI
    SPCTL |= SPEN_BIT;

    // 8. 清除状态标志（可选）
    SPSTAT = 0x00;
}

// 发送并接收一个字节（全双工）
unsigned char spi_transfer(unsigned char spi_data) 
{
    // 1. 等待上一次传输完成（SPIF=1）
    while (!(SPSTAT & SPIF_BIT));

    // 2. 写入数据到 SPDAT，触发传输
    SPDAT = spi_data;

    // 3. 等待传输完成
    while (!(SPSTAT & SPIF_BIT));

    // 4. 返回接收到的数据
    return SPDAT;
}

// 仅发送（忽略返回值）
void spi_send(unsigned char spi_data) 
{
    spi_transfer(spi_data);
}

// 仅接收（发送 0xFF）
unsigned char spi_receive(void) 
{
    return spi_transfer(0xFF);
}