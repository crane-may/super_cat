#ifndef _ADXL_H_
#define _ADXL_H_

// ADXL Control Registers
#define ADXLREG_BW_RATE      0x2C //(sending 0x0C is 400Hz)
#define ADXLREG_POWER_CTL    0x2D
#define ADXLREG_DATA_FORMAT  0x31
#define ADXLREG_FIFO_CTL     0x38
#define ADXLREG_INT_ENABLE   0x2E
#define ADXLREG_INT_MAP      0x2F

//ADXL Data Registers
#define ADXLREG_DEVID        0x00
#define ADXLREG_DATAX0       0x32
#define ADXLREG_DATAX1       0x33
#define ADXLREG_DATAY0       0x34
#define ADXLREG_DATAY1       0x35
#define ADXLREG_DATAZ0       0x36
#define ADXLREG_DATAZ1       0x37
#define ADXLREG_INT_SOURCE   0x30

static int ADXLREG_DATA[] = {ADXLREG_DATAX0,ADXLREG_DATAX1,ADXLREG_DATAY0,ADXLREG_DATAY1,ADXLREG_DATAZ0,ADXLREG_DATAZ1};

#define ADXLREG_VAL1         0x28
#define ADXLREG_VAL2         0x0B
#define ADXLREG_VAL3         0x00
#define ADXLREG_VAL4         0x0F

#define ADXL345_ADDRESS      0x53

static int ADXL345_ZERO[3] = {-9,0,0};

void ADXL_write_reg(byte reg, byte data);
void ADXL_read(int arr[]);
void initADXL();

#endif
