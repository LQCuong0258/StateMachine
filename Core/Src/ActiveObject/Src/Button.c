#include "Button.h"

struct Active * AO_button;
struct Button * button;

static Status initial (struct Button * const self, Event const * const event) {
  Status status = TRAN_STATUS;
  self->super.handler = (StateHandler) self->active;
  return status;
}

static Status active (struct Button * const self, Event const * const event) {
  Status status;
  
  switch (event->signal) {
    case ENTRY_SIG:
      HAL_NVIC_EnableIRQ(EXTI0_IRQn);
      status = TRAN_STATUS;
      break;

    case BUTTON_PRESSED_SIG:
      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
      status = HANDLED_STATUS;
      break;

    case BUTTON_RELEASED_SIG:
      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
      status = HANDLED_STATUS;
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
 * tức là gán hàm Button_dispatch vào vistual function là dispatch
 * của Active Object, khi đó Active Object của Button có thể gửi
 * các Event đến đúng dispatch function của nó.
 */
void constructor (struct Button * const self) {
  self->initial = &initial;
  self->active = &active;

  Active_Constructor(&self->super, (StateHandler) &initial);

  AO_button = &self->super;
  button = self;
}

const struct ButtonClass Button = {.constructor = &constructor};

