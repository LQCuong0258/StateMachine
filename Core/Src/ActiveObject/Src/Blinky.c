#include "Blinky.h"

extern TIM_HandleTypeDef htim7;

struct Active * AO_blinky;
struct Blinky * blinky;

static Status initial (struct Blinky * const self, Event const * const event) {
    Status status = TRAN_STATUS;
    self->super.handler = (StateHandler) self->active;
    return status;
}

/**
 * Hàm này sẽ xử lý khi nhận được sự kiện từ Active Object
 */
static Status active (struct Blinky * const self, Event const * const event) {
  Status status;

  switch (event->signal) {
    /**
     * Đây là Event khi ActiveObject được khởi tạo
     * trong trường hợp này đang khi được khởi tạo
     * sẽ bật timer bắt đầu cout
     */
    case ENTRY_SIG:
      HAL_TIM_Base_Start_IT(&htim7); // Bắt đầu Timer với ngắt
      status = TRAN_STATUS;
      break;
    /**
     * Vì đây là một sự kiện thời gian được post từ timer
     * nên khi timer triger, nó sẽ gửi Event TIME_OUT_SIG đến Active Object
     * khi đó led sẽ Toggle đúng chu kỳ đã được cấu hình cho timer
     */
    case TIME_OUT_SIG:
      HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
      status = TRAN_STATUS;
      break;
    case EXIT_SIG:
      status = HANDLED_STATUS;
    default:
      status = IGNORED_STATUS;
      break;
  }
  return status;
}

/**
 * Hàm này dùng để khởi tạo constructer
 * tức là gán hàm Blinky_dispatch vào vistual function là dispatch
 * của Active Object, khi đó Active Object của Blinky có thể gửi
 * các Event đến đúng dispatch function của nó.
 */
static void constructor (struct Blinky * const self) {
  self->initial = &initial;
  self->active = &active;

  Active_Constructor (&self->super, (StateHandler) initial);

  AO_blinky = &self->super;
  blinky = self;
}

const struct BlinkyClass Blinky = {.constructor = &constructor};
