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
struct Blinky {
    /* Members */
    Active super;

    /* Methods */
    Status (*initial) ( struct Blinky * const self, Event const * const event );
    Status (*active) ( struct Blinky * const self, Event const * const event );
};

extern const struct BlinkyClass {
    void (*constructor) (struct Blinky * const self);
} Blinky;


// void Blinky_ctor (Blinky * const self);

// Status Blinky_initial (Blinky * const self, void const * const para);
// Status Blinky_active (Blinky * const self, Event const * const para);

#endif /* __BLINKY_H */