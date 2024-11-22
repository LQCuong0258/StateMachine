/* 
 * Author: Syaoran
 * Created on: 2024-11-13
 */
#ifndef __BUTTON_H
#define __BUTTON_H

#include "Active_Object.h"

typedef enum {
    BUTTON_PRESSED_SIG = USER_SIG,
    BUTTON_RELEASED_SIG,
} ButtonSignal;

/* The Button Active Object */
typedef struct {
  Active super;
} Button;

void Button_ctor (Button * const self);

Status Button_initial (Button * const self, void const * const para);
Status Button_active (Button * const self, Event const * const para);


#endif /* __BUTTON_H */