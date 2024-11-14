#include "Active_Object.h"

/**
 * Hàm constructer
 */
void Active_ctor (Active * const self, DispatchHandler dispatch) {
    /* Assign dispatch handler */
    self->dispatch = dispatch;
}

/*-------------------------------------------------------------------------------------------------------*/
/* Event-loop thread function for all Active Object (FreeRTOS task signature) */
static void Active_eventloop (void *pvParameters) {
    Active *self = (Active *) pvParameters;
    static Event const initEvt = { INIT_SIG };
    /* Active object must be provided */
    configASSERT(self);

    /* Initialize Active Object */
    (*self->dispatch) (self, &initEvt);
    for(;;) {
        Event * event;  /* Pointer to Event object (message) */
    
        /* Wait for any event and receive it into object 'event' */
        xQueueReceive(self->queue, &event, portMAX_DELAY);
        /* Dispatch to Active Object 'self' */
        (*self->dispatch) (self, event);
    }
}
/*-------------------------------------------------------------------------------------------------------*/


void Active_start (Active * const self,
                   UBaseType_t uxPriority,      // priority (1-based)
                   UBaseType_t uxQueueLength,   // 
                   uint32_t usStackDepth)       //
{   
    self->queue = xQueueCreate(uxQueueLength, sizeof(Event *));

    xTaskCreate(&Active_eventloop,              /* The thread function */
                "AO",                           /* The name of the task */
                usStackDepth,                   /* Stack depth */
                self,                           /* The pvParameters */
                uxPriority + tskIDLE_PRIORITY,  /* FreeROTS priority */
                &self->thread);                 /* pxCreatedTask */
    /* Thread must be created */
    configASSERT(self->thread);
}

void Active_post (Active * const self, Event const * const event) {
    BaseType_t status = xQueueSendToBack(self->queue, (void *)&event, (TickType_t) 0);
    configASSERT(status == pdPASS);
}

void Active_postFromISR (Active * const self, Event const * const event,
                         BaseType_t *pxHigherPriorityTaskWoken)
{
    BaseType_t status = xQueueSendToBackFromISR(self->queue, (void *)&event, pxHigherPriorityTaskWoken);
    configASSERT(status == pdPASS);
}
