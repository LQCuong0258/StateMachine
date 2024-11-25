#include "Active_Object.h"

/*-------------------------------------------------------------------------------------------------------*/
/* Event-loop thread function for all Active Object (FreeRTOS task signature) */
static void Active_eventloop (void *pvParameters) {
    Active *self = (Active *) pvParameters;

    static Event const entry_evt = { ENTRY_SIG };
    /* Active object must be provided */
    configASSERT(self);

    /* Vào sự kiện mới */
    (*self->dispatch) (self, &entry_evt);

    for(;;) {
        Event * event;  /* Pointer to Event object (message) */
    
        /* Wait for any event and receive it into object 'event' */
        xQueueReceive(self->queue, &event, portMAX_DELAY);
        /* Dispatch to Active Object 'self' */
        (*self->dispatch) (self, event);
    }
}
/*-------------------------------------------------------------------------------------------------------*/

static void initial (Active * const self, Event const * const event) {
    /* Initialize Active Object */
    static Event const entry_evt = { .signal = ENTRY_SIG };

    /* khởi tạo trạng thái mới */
    (*self->handler) (self, event);
    /* Thực thi các hành động khi bắt đầu trạng thái mới */
    (*self->handler) (self, &entry_evt);
}

static void start (Active * const self,
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

/* Hàm phân phối các sự kiện cho Active Object */
static void dispatch (struct Active * const self, Event const * const event) {
    Status status;
    /* Lưu lại quá trình xử lý hiện tại */
    StateHandler prev_handler = self->handler;
    static Event const entry_evt = {.signal = ENTRY_SIG};
    static Event const exit_evt = {.signal = EXIT_SIG};

    /* Thực hiện xử lý sự kiện mới được nhận */
    status = (*self->handler)(self, event);

    /* Chuyển đổi sự kiện */
    if(status == TRAN_STATUS) {
        (*prev_handler)(self, &exit_evt);   /* Thoát khởi sự kiện hiện tại */
        (*self->handler)(self, &entry_evt); /* Vào một sự kiện mới */
    }
    else if (status == INIT_STATUS) {
        (*self->handler)(self, &entry_evt); /* Vào một sự kiện mới */
    }
}

static void post (Active * const self, Event const * const event) {
    BaseType_t status = xQueueSendToBack(self->queue, (void *)&event, (TickType_t) 0);
    configASSERT(status == pdPASS);
}

static void postFromISR (Active * const self, Event const * const event,
                         BaseType_t *pxHigherPriorityTaskWoken)
{
    BaseType_t status = xQueueSendToBackFromISR(self->queue, (void *)&event, pxHigherPriorityTaskWoken);
    configASSERT(status == pdPASS);
}

/**
 * Hàm constructer
 */
void Active_Constructor (Active * const self, StateHandler handler) {
    self->handler = handler;

    self->eventLoop = &Active_eventloop;
    self->initial = &initial;
    self->start = &start;
    self->dispatch = &dispatch;
    self->post = &post;
    self->postFromISR = &postFromISR;
}
