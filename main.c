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
#include "INA219.h"

#define BAUDRATE 9600

#define LED 7
#define LEDINIT() DDRC |= (1 << LED);
#define LEDBLINK() PORTC |= (1 << LED);\
					_delay_ms(500);\
					PORTC &= ~(1 << LED);\
					_delay_ms(500);
					
//shunt resistance used with the INA219 IC in ohms
#define RSHUNT 0.1

int main(void)
{
	USART_Init(BAUDRATE);
	
	TWIInit(300000); //300kHz
	INA219_init();
	
    /* Replace with your application code */
    while (1) 
    {
		
		USART_Transmit_String("Current in (mA):");
		USART_Transmit_Float(INA219_get_current(RSHUNT));
		
		USART_Transmit_String("Bus Voltage (V):");
		USART_Transmit_Float(get_bus_voltage());
		
		LEDBLINK();
    }
}

