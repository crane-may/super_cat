#include "Wire.h"
#include "adxl.h"

void ADXL_write_reg(byte reg, byte data)
{
  Wire.beginTransmission(ADXL345_ADDRESS);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

void ADXL_read(int arr[])
{
  byte t8[2];
  short t16;
  for (int i=0;i<3;i++){
    Wire.beginTransmission(ADXL345_ADDRESS);
    Wire.write(ADXLREG_DATA[i*2]);
    Wire.write(ADXLREG_DATA[i*2+1]);
    Wire.endTransmission();
    Wire.requestFrom(ADXL345_ADDRESS, 2);
    t8[0]=Wire.read();
    t8[1]=Wire.read();
    t16 = ((t8[1]<<8)|t8[0]);
    arr[i] = t16 + ADXL345_ZERO[i];
  }
}

void initADXL()
{
  ADXL_write_reg(ADXLREG_POWER_CTL,0x08);
  ADXL_write_reg(ADXLREG_DATA_FORMAT,0x08);
  ADXL_write_reg(ADXLREG_FIFO_CTL,0x00);
  ADXL_write_reg(ADXLREG_BW_RATE ,0x0E);
}
