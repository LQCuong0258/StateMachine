/* 
 * Author: SyAOran
 * Created on: 2024-11-13
 */
#ifndef __ACTIVE_OBJECT_H
#define __ACTIVE_OBJECT_H

#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/*-------------------------------------------------------------------------------------------------------*/
/* Event facilities */

/** Event base class
 * Đây là tín hiệu của sự kiện
 */
typedef struct {
    uint16_t signal;
    /* Event parameters add in subclass of Event */
} Event;

typedef enum {
    INIT_SIG,   // Dispatched to AO before entering event-loop
    USER_SIG,   // First-signal available to the users
} ReservedSignal;

/*-------------------------------------------------------------------------------------------------------*/
/* Active Object facilities */

typedef struct Active Active; /* Forward declaration */

typedef void (* DispatchHandler)(Active * const self, Event const * const event);

/* Active object base class */
struct Active {
    // Members ---------------------------------
    TaskHandle_t thread;    /* Private thread */
    StaticTask_t thread_cb; /* thread control-block (FreeRTOS static alloc) */

    QueueHandle_t queue;    /* Private message queue */
    StaticQueue_t queue_cb; /* queue control-block (FreeRTOS static alloc) */

    DispatchHandler dispatch;

    /* Active object data added in subclass of Active */
};

/* Contructor */
void Active_ctor (Active * const self, DispatchHandler dispatch);

void Active_start (Active * const self,
                   UBaseType_t uxPriority,
                   UBaseType_t uxQueueLength,
                   uint32_t usStackDepth);

void Active_post (Active * const self, Event const * const event);
void Active_postFromISR (Active * const self, Event const * const event,
                         BaseType_t *pxHigherPriorityTaskWoken);

/*-------------------------------------------------------------------------------------------------------*/
/* Time Event class */
// typedef struct {
//     Event super;        /* Inherit Event */
//     Active *act;        /* The AO that request this TimeEvent */
//     uint32_t timeout;   /* Timeout counter; 0 means not armed */
//     uint32_t interval;  /* Interval for periodic TimeEvent, 0 means not one-shot */
// } TimeEvent;

#endif /* __ACTIVE_OBJECT_H */