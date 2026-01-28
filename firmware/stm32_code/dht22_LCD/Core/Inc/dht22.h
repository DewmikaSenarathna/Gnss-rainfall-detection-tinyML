#ifndef __DHT22_H
#define __DHT22_H

#include "stm32f1xx_hal.h"

void DHT22_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
uint8_t DHT22_Read(float *temperature, float *humidity);

#endif

