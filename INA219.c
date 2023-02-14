/*
 * INA219.c
 *
 * Created: 2/10/2023 10:28:18 PM
 *  Author: jpsteph
 */ 


#include "I2C.h"
#include "INA219.h"

//defs for config in init function
#define INA1CONFIG  0b0010011001100111  // INA config register according to datasheet
#define INACALIB    5120                // INA calibration register according to R_SHUNT

//defs for modes
#define INASETMODE 0xFFF8
#define INACONTMODE 0x0007
#define INASLEEPMODE 0x0000

void writeINA(uint8_t adr, uint8_t reg, uint16_t u16data) {
	TWIStart();
	TWIWrite(adr);
	TWIWrite(reg);
	TWIWrite((uint8_t) (u16data >> 8));
	TWIWrite((uint8_t) u16data | 0xff);
	TWIStop();
}

uint16_t readINA(uint8_t adr, uint8_t reg) {
	uint16_t result;
	TWIStart();
	//setting read pointer to register
	TWIWrite(adr);
	TWIWrite(reg);
	TWIStop();
	
	TWIStart();
	//starting read
	TWIWrite(adr | 0x01);
	result = ((uint16_t) TWIReadACK()) << 8;
	result = ((uint16_t) TWIReadNACK()) | result;
	TWIStop();
	
	return result;
}

void INA219_init(void) {
	
	writeINA(INA1ADDR, CONFIG_REG, INA1CONFIG);
	writeINA(INA1ADDR, CALIB_REG,  ILCAL1 * INACALIB);
}

void INA219_wakeup(void) {
	writeINA(INA1ADDR, CONFIG_REG, (INA1CONFIG & INASETMODE) | INACONTMODE );
}

void INA219_sleep() {
	writeINA(INA1ADDR, CONFIG_REG, (INA1CONFIG & INASETMODE) | INASLEEPMODE );
}

float INA219_get_current(float shunt_resistance) {
	uint16_t shuntVoltageReg;
	float shuntVoltage;
	
	shuntVoltageReg = readINA(INA1ADDR, SHUNT_REG);
	
	if (shuntVoltageReg & 0x8000) {
		// Two's complete on 16-bit unsigned register value
		shuntVoltage = (int16_t)((~shuntVoltageReg) + 1) * -1;
		} else {
		shuntVoltage = shuntVoltageReg;
	}
	
	shuntVoltage /= 100;
	
	// Calculate current in mA: I = U / Rshunt
	return shuntVoltage / shunt_resistance;
}

float get_bus_voltage(void) {
	uint16_t busVoltageReg;
	float busVoltage;
	
	//getting bus voltage
	busVoltageReg = readINA( INA1ADDR, VOLTAGE_REG);
	// Calculate bus voltage in mV
	if (busVoltageReg & 0x01) {
		// OVF (Overflow) bit is set
		busVoltage = 0xFFFF;
		} else {
		// Return bus voltage bits 3..15 where LSB = 4 mV
		busVoltage = (busVoltageReg >> 3) * 4;
	}
	return busVoltage /= 1000;
}
