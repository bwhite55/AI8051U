#ifndef __I2C_H
#define __I2C_H

#include "AI8051U.H"
#include "ai8051u_conf.h"

void I2C_init(int I2C_speed);

#if I2C_MODE == I2C_MODE_MASTER
    void I2C_start(void);
    void I2C_SendData(char dat);
    void I2C_RecvACK();
    char I2C_RecvData(void);
    void I2C_SendACK(void);
    void I2C_Stop(void);
    void I2C_Start_SendData_RecvACK(char dat);
    void I2C_SendData_RecvACK(char dat);
    void I2C_RecvData_SendACK(void);
    void I2C_RecvData_SendNAK(void);

#endif

#endif // __I2C_H