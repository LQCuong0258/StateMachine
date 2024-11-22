/* 
 * Author: Syaoran
 * Created on: 2024-11-22
 */
#ifndef __SYSCONFIG_H
#define __SYSCONFIG_H

#include "stm32f4xx_hal.h"

void SystemClock_Config(void);
void Error_Handler(void);
void MX_Periferal_Init (void);

#endif /* __SYSCONFIG_H */