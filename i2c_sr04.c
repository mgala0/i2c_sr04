/**
 * @file i2c_sr04.c
 * @author M.A.G (m.gala@mgala.eu)
 * @brief STM32 HAL-based procedures for HC-SR04d ultrasonic distance meter with I2C interface
 * @version 1.0
 * @date 2024-02-05
 *
 * @copyright Copyright (c) 2024
 * STM32 HAL-based procedures for HC-SR04d ultrasonic distance meter with I2C interface @ 0x57 address
 * Sensor based on RCWL-9610 chip, need to be configure to I2C interface via shortening pads
 * 
 */

#include "main.h"
#include "i2c.h"
#include "i2c_sr04.h"

#define I2C_RCWL9610_ADDRESS	0x57
#define I2C_READ_BIT			0x01
#define I2C_WRITE_BIT			0x00
#define COMMAND_MEASURE_START	0x01

/**
 * @brief Read data value from sensor and compute distance value in mm
 * All-in-one start measure, read and compute distance function but written with
 * blocking manner (delay inside function) 
 *
 * @param hi2c - pointer to h2ic instance 
 * @return distance to target in mm or -1 if error
 */
float SR04MeasureDistanceMM(I2C_HandleTypeDef *hi2c)
{
    uint8_t command_measure_start = COMMAND_MEASURE_START;
    uint8_t readbuff[3];

    if (HAL_I2C_Master_Transmit(hi2c, (I2C_RCWL9610_ADDRESS<<1)|I2C_WRITE_BIT, &command_measure_start, 1, 20) != HAL_OK)
    {
        return -1;
    }
    
    HAL_Delay(150); //minimum processing time 120ms
    
    if (HAL_I2C_Master_Receive(hi2c, (I2C_RCWL9610_ADDRESS<<1)|I2C_READ_BIT, readbuff, 3, 50) == HAL_OK) //read 3 bytes of distance data
    {
        //According to datasheet: distance = (readbuff[0]<<16 + readbuff[1]<<8 +readbuff[2])/1000 
        //uint32_t tmp_read_distance = readbuff[0] * 65536 + readbuff[1] * 256 + readbuff[2];
        uint32_t tmp_read_distance = (readbuff[0]<<16)|(readbuff[1]<<8)|(readbuff[3]);
        float distance_mm = tmp_read_distance / 1000.f;
        return distance_mm;
    }
    else
    {
        return -1;
    }
}

/**
 * @brief Send command to start measuring distance
 * Remember to have min. 120ms between calling this function and function that read measured distance
 * 
 * @param hi2c - pointer to h2ic instance
 * @return HAL_OK or HAL_ERROR depending of success or fail 
 */

HAL_StatusTypeDef SR04StartMeasureDistance(I2C_HandleTypeDef *hi2c)
{
	uint8_t command_measure_start = COMMAND_MEASURE_START;
	if (HAL_I2C_Master_Transmit(hi2c, (I2C_RCWL9610_ADDRESS<<1)|I2C_WRITE_BIT, &command_measure_start, 1, 20) == HAL_OK)
    {
        return HAL_OK;
    }
    else
    {
        return HAL_ERROR;
    }
}

/**
 * @brief Read data value from sensor and compute distance value in mm
 * Remember to call this function min. 120ms after start measuring 
 * 
 * @param hi2c - pointer to h2ic instance
 * @return float measured distance in mm or -1 if failed
 */

float SR04GetDistanceMM(I2C_HandleTypeDef *hi2c)
{

    uint8_t readbuff[3];

    if (HAL_I2C_Master_Receive(hi2c, (I2C_RCWL9610_ADDRESS<<1)|I2C_READ_BIT, readbuff, 3, 50) ==HAL_OK) //read 3 bytes of distance data
    {
        //According to datasheet: distance = (readbuff[0]<<16 + readbuff[1]<<8 +readbuff[2])/1000 
        //uint32_t tmp_read_distance = readbuff[0] * 65536 + readbuff[1] * 256 + readbuff[2];
        uint32_t tmp_read_distance = (readbuff[0]<<16)|(readbuff[1]<<8)|(readbuff[3]);
        float distance_mm = tmp_read_distance / 1000.f;
        return distance_mm;
    }
    else
    {
        return (-1.0);
    }
}


