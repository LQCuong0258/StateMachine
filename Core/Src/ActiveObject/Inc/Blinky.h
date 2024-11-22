/* 
 * Author: Syaoran
 * Created on: 2024-11-22
 */
#ifndef __BLINKY_H
#define __BLINKY_H

#include "stm32f4xx_hal.h"
#include "Active_Object.h"
#include "Button.h"

typedef enum {
    TIME_OUT_SIG = USER_SIG,   // First-signal available to the users
} BlinkySignal;

/* The Blinky Active Object */
typedef struct {
    Active super;

    Event event;
} Blinky1;

void Blinky1_ctor (Blinky1 * const self);

Status Blinky_initial (Blinky1 * const self, void const * const para);
Status Blinky_active (Blinky1 * const self, Event const * const para);

#endif /* __BLINKY_H */