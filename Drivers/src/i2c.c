#include "AI8051U.H"
#include "i2c.h"
bit busy;

bit isda;
bit isma;
unsigned char address;
unsigned char edata buffer[32];


void I2C_init(int I2C_speed)
{
    // Initialize I2C peripheral settings
    // Set clock speed, addressing mode, and enable I2C module
    #if I2C_PIN_MAP == 0
    I2C_S1 = 0;
    I2C_S0 = 0;
    // 可选：配置 P2.3/P2.4 为开漏输出（根据芯片要求）
    #elif I2C_PIN_MAP == 1
    I2C_S1 = 0;
    I2C_S0 = 1;
    #elif I2C_PIN_MAP == 2
    I2C_S1 = 1;
    I2C_S0 = 1;
    #else
    #error "Invalid I2C_PIN_MAP value! Must be 0~3."
    #endif
    int MSSPEED = (240000 / I2C_speed/ 2 - 4) / 2;
    #if I2C_MODE == I2C_MODE_MASTER
    I2CCFG = 0xC0 | (MSSPEED & 0x3F); // 设置 I2C 时钟频率
    I2CPSCR = (MSSPEED >> 6) & 0xFF;
    I2CMSST = 0x07; // 设置主机模式
    EA = 1;        // 使能总中断
    #elif I2C_MODE == I2C_MODE_SLAVE
    I2CCFG = 0x81; // 设置为从机模式
    I2CSLADR = I2C_SLAVE_ADDR_7BIT & 0x7F; // Ensure only 7 bits are set
    I2CSLST = 0x00;
    I2CSLCR = 0x78; // 使能从机模式
    EA = 1;        // 使能总中断
    isda = 1;
    isma = 1;
    address = 0;
    #endif

}

void I2C_Isr(void) interrupt 24
{
    // I2C 中断服务程序
    #if I2C_MODE == I2C_MODE_MASTER
    // 主机模式下的中断处理
    if(I2CMSST & 0x40)
    {
        I2CMSST &= ~0x40; // 清除中断标志
        busy = 0;
    }
    #elif I2C_MODE == I2C_MODE_SLAVE
    // 从机模式下的中断处理
    if(I2CSLST & 0x40)
    {
        I2CSLST &= ~0x40; // 处理START事件
        isda = 1;       //若为重复起始信号时必须作此设置
    }
    else if(I2CSLST & 0x20)
    {
        I2CSLST &= ~0x20; // 处理RECV事件
        if(isda)
        {
            isda = 0;   //RECV DEVICE ADDRESS
        }
        else if(isma)
        {
            isma = 0;   //RECV MEMORY ADDRESS
            address = I2CRXD;
            I2CTXD = buffer[address];
        }
        else
        {
            buffer[address++] = I2CRXD; //RECV DATA
        }
    }
    else if(I2CSLST & 0x10)
    {
        I2CSLST &= ~0x10; // 处理SEND事件
        if(I2CSLST & 0x02)
        {
            I2CTXD = 0xFF; // 接收到NAK则停止读取数据
        }
        else
        {
            I2CTXD = buffer[++address]; //接收到ACK则继续读取数据
        }
    }
    else if(I2CSLST & 0x08)
    {
        I2CSLST &= ~0x08; // 处理STOP事件
        isda = 1;
        isma = 1;
        // 停止条件处理
    }
    #endif
}



void I2C_start(void)
{
    busy = 1;
    I2CMSCR = 0x81; // 发送起始条件
    while (busy);
}

void I2C_SendData(char dat)
{
    I2CTXD = dat;                           //写数据到数据缓冲区
    busy = 1;
    I2CMSCR = 0x82;                         //发送SEND命令
    while (busy);
}

void I2C_RecvACK()
{
    busy = 1;
    I2CMSCR = 0x83;                         //接收ACK命令
    while (busy);
}

char I2C_RecvData(void)
{
    busy = 1;
    I2CMSCR = 0x84;                         //接收数据命令
    while (busy);
    return I2CRXD;
}

void I2C_SendACK(void)
{
    I2CMSST = 0x00;                         //设置ACK信号
    busy = 1;
    I2CMSCR = 0x85;                         //发送ACK命令,发送I2CMSST.0 = 0表示ACK，若位1表示NAK
    while (busy);
}

void I2C_SendNAK(void)
{
    I2CMSST = 0x01;                         //设置NAK信号
    busy = 1;
    I2CMSCR = 0x85;                         //发送NAK命令,发送I2CMSST.0 = 0表示ACK，若位1表示NAK
    while (busy);
}

void I2C_Stop(void)
{
    busy = 1;
    I2CMSCR = 0x86; // 发送停止条件
    while (busy);
}

void I2C_Start_SendData_RecvACK(char dat)
{
    I2CTXD = dat;                           //写数据到数据缓冲区
    busy = 1;
    I2CMSCR = 0x89;                         //发送START+SEND+RecvACK命令
    while (busy);
}

void I2C_SendData_RecvACK(char dat)
{
    I2CTXD = dat;                           //写数据到数据缓冲区
    busy = 1;
    I2CMSCR = 0x8A;                         //发送SEND命令
    while (busy);
}

void I2C_RecvData_SendACK(void)
{
    busy = 1;
    I2CMSCR = 0x8B;                         //接收数据命令,此命令所返回的应答信号固定为 ACK（0），不受 MSACKO 位的影响
    while (busy);
}

void I2C_RecvData_SendNAK(void)
{
    busy = 1;
    I2CMSCR = 0x8C;                         //接收数据命令,此命令所返回的应答信号固定为 NAK（1），不受 MSACKO 位的影响
    while (busy);
}