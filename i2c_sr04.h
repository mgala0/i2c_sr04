/**
 * @file i2c_sr04.h
 * @author M.A.G (m.gala@mgala.eu)
 * @brief STM32 HAL-based procedures for HC-SR04d ultrasonic distance meter with I2C interface
 * See .c file for detials
 * @version 0.1
 * @date 2024-01-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __I2C_SR04_H__
#define __I2C_SR04_H__

#ifdef __cplusplus
extern "C" {
#endif

float SR04MeasureDistanceMM(I2C_HandleTypeDef *hi2c)
HAL_StatusTypeDef SR04StartMeasureDistance(I2C_HandleTypeDef *hi2c)
float SR04GetDistanceMM(I2C_HandleTypeDef *hi2c)


#ifdef __cplusplus
}
#endif

#endif //__I2C_SR04_H__
