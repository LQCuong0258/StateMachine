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

typedef struct Active Active; /* Forward declaration */

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
    ENTRY_SIG,  //
    EXIT_SIG,   //
    USER_SIG,   // First-signal available to the users
} ReservedSignal;

typedef enum {
    TRAN_STATUS,    /* Trạng thái cho biết đã chuyển sang một sự kiện mới */
    HANDLED_STATUS, /* Trạng thái cho biết sự kiện đã được xử lý */
    IGNORED_STATUS, /* Trạng thái cho biết sự kiện đã bị bỏ qua */
    INIT_STATUS     /* Trạng thái ban đầu khi khởi tạo Active Object */
} Status;

/*-------------------------------------------------------------------------------------------------------*/
/* Active Object facilities */

typedef Status (*StateHandler) (Active * const self, Event const * const event);

/* Active object base class */
struct Active {
    // Members ---------------------------------
    TaskHandle_t thread;    /* Private thread */
    StaticTask_t thread_cb; /* thread control-block (FreeRTOS static alloc) */

    QueueHandle_t queue;    /* Private message queue */
    StaticQueue_t queue_cb; /* queue control-block (FreeRTOS static alloc) */

    StateHandler handler;

    /* Active object data added in subclass of Active */

    /* Methods */
    void (*initial) (Active * const self, Event const * const event);
    void (*start) (Active * const self,
                   UBaseType_t uxPriority,      // priority (1-based)
                   UBaseType_t uxQueueLength,   // 
                   uint32_t usStackDepth);
    void (*dispatch) (Active * const self, Event const * const event);
    void (*eventLoop) (Active * const self, Event const * const event);
    void (*post) (Active * const self, Event const * const event);
    void (*postFromISR) (Active * const self, Event const * const event,
                         BaseType_t *pxHigherPriorityTaskWoken);
};

/* Contructor */
void Active_Constructor (Active * const self, StateHandler dispatch);

#endif /* __ACTIVE_OBJECT_H */