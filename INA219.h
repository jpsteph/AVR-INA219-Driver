/*
 * INA219.h
 *
 * Created: 2/10/2023 10:28:44 PM
 *  Author: jpsteph
 */ 


#ifndef INA219_H_
#define INA219_H_

#define INA1ADDR    0b10000000
#define CONFIG_REG  0x00                // INA configuration register address
#define CALIB_REG   0x05                // INA calibration register address
#define SHUNT_REG   0x01                // INA shunt voltage register address
#define VOLTAGE_REG 0x02                // INA bus voltage register address
#define POWER_REG   0x03                // INA power register address
#define CURRENT_REG 0x04                // INA current register address
#define CONFIG_REG  0x00                // INA configuration register address
#define CALIB_REG   0x05                // INA calibration register address
#define SHUNT_REG   0x01                // INA shunt voltage register address
#define VOLTAGE_REG 0x02                // INA bus voltage register address
#define POWER_REG   0x03                // INA power register address
#define CURRENT_REG 0x04                // INA current register address
#define ILCAL1      1                   // linear current calibration factor (load)

void writeINA(uint8_t adr, uint8_t reg, uint16_t u16data);

uint16_t readINA(uint8_t adr, uint8_t reg);

void INA219_init(void);

void INA219_wakeup(void);

void INA219_sleep(void);

float INA219_get_current(float shunt_resistance);

float get_bus_voltage(void);

#endif /* INA219_H_ */