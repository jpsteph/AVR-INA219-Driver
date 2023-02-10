/*
 * CurrentSensor.c
 *
 * Created: 1/27/2023 9:45:23 PM
 * Author : jpsteph
 */ 

#include <avr/io.h>
#include "I2C.h"
#include <string.h>
#include "clock.h"
#include <util/delay.h>
#include "uart.h"

#define BAUDRATE 9600

#define LED 7

#define INA1ADDR    0b10000000
#define CONFIG_REG  0x00                // INA configuration register address
#define CALIB_REG   0x05                // INA calibration register address
#define SHUNT_REG   0x01                // INA shunt voltage register address
#define VOLTAGE_REG 0x02                // INA bus voltage register address
#define POWER_REG   0x03                // INA power register address
#define CURRENT_REG 0x04                // INA current register address
#define INA1CONFIG  0b0010011001100111  // INA config register according to datasheet
#define INACALIB    5120                // INA calibration register according to R_SHUNT
#define CONFIG_REG  0x00                // INA configuration register address
#define CALIB_REG   0x05                // INA calibration register address
#define SHUNT_REG   0x01                // INA shunt voltage register address
#define VOLTAGE_REG 0x02                // INA bus voltage register address
#define POWER_REG   0x03                // INA power register address
#define CURRENT_REG 0x04                // INA current register address
#define ILCAL1      1                   // linear current calibration factor (load)    



int main(void)
{
	USART_Init(BAUDRATE);
	uint16_t shuntVoltageReg;
	uint16_t busVoltageReg;
	float Rshunt = 0.1;
	float shuntVoltage;
	float busVoltage;
	float current;
	//making LED pin output
	DDRC |= (1 << LED);
	
	TWIInit(300000); //300kHz
	writeINA(INA1ADDR, CONFIG_REG, INA1CONFIG);
	writeINA(INA1ADDR, CALIB_REG,  ILCAL1 * INACALIB);
	
    /* Replace with your application code */
    while (1) 
    {
		PORTC |= (1 << LED);
		_delay_ms(1000);
		PORTC &= ~(1 << LED);
		_delay_ms(1000);
		
		shuntVoltageReg = readINA(INA1ADDR, SHUNT_REG);
		
		if (shuntVoltageReg & 0x8000) {
			// Two's complete on 16-bit unsigned register value
			shuntVoltage = (int16_t)((~shuntVoltageReg) + 1) * -1;
			} else {
			shuntVoltage = shuntVoltageReg;
		}
		
		shuntVoltage /= 100;
		
		// Calculate current in mA: I = U / Rshunt
		current = shuntVoltage / Rshunt;
		USART_Transmit_String("Current in (mA):");
		USART_Transmit_Float(current);
		
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
		busVoltage /= 1000;
		
		USART_Transmit_String("Bus Voltage (V):");
		USART_Transmit_Float(busVoltage);
    }
}

