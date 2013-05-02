#include "Wire.h"
#include "l3g.h"

// Writes a gyro register
void L3G4200D_writeReg(byte reg, byte value)
{
	Wire.beginTransmission(GYR_ADDRESS);
	Wire.write(reg);
	Wire.write(value);
	Wire.endTransmission();
}

void initL3G4200D(void)
{
	// 0x0F = 0b00001111
	// Normal power mode, all axes enabled
	L3G4200D_writeReg(L3G4200D_CTRL_REG1, 0x0F);
}

// Reads the 3 gyro channels and stores them in vector g
void L3G4200D_read(int arr[])
{
	Wire.beginTransmission(GYR_ADDRESS);
	// assert the MSB of the address to get the gyro 
	// to do slave-transmit subaddress updating.
	Wire.write(L3G4200D_OUT_X_L | (1 << 7)); 
	Wire.endTransmission();
	Wire.requestFrom(GYR_ADDRESS, 6);

	while (Wire.available() < 6);
	
	byte xla = Wire.read();
	byte xha = Wire.read();
	byte yla = Wire.read();
	byte yha = Wire.read();
	byte zla = Wire.read();
	byte zha = Wire.read();
        short t;
        t = xha << 8 | xla;arr[0] = t+L3G4200D_ZERO[0];
	t = yha << 8 | yla;arr[1] = -(t+L3G4200D_ZERO[1]);
	t = zha << 8 | zla;arr[2] = t+L3G4200D_ZERO[2];
}
