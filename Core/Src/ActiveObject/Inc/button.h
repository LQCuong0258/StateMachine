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
struct  Button{
  Active super;

  Status (*initial) (struct Button * const self, Event const * const para);
  Status (*active) (struct Button * const self, Event const * const para);
};

extern const struct ButtonClass {
  void (*constructor) (struct Button * const self);
} Button;


#endif /* __BUTTON_H */