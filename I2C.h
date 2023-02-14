/*
 * I2C.h
 *
 * Created: 1/27/2023 9:46:34 PM
 *  Author: jpsteph
 */ 

#include <stdint.h>

#ifndef I2C_H_
#define I2C_H_

void TWIInit(uint32_t speed);

void TWIStart(void);

void TWIStop(void);

void TWIWrite(uint8_t u8data);

void TWIWriteACK(uint8_t u8data);

uint8_t TWIReadACK(void);

uint8_t TWIReadNACK(void);

uint8_t TWIGetStatus(void);

#endif /* I2C_H_ */