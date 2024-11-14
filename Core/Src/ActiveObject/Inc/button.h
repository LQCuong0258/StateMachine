/* 
 * Author: Syaoran
 * Created on: 2024-11-13
 */
#ifndef __BUTTON_H
#define __BUTTON_H

#include "Active_Object.h"

typedef enum {
    TIME_OUT_SIG = USER_SIG,   // First-signal available to the users
    BUTTON_PRESSED_SIG,
    BUTTON_RELEASED_SIG,
} ButtonSignal;

// typedef enum {
    
// } BlinkySignal;

#endif /* __BUTTON_H */