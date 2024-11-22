#include "Button.h"

static void Button_dispatch (Button * const self, Event const * const event) {
  switch (event->signal)
  {
    case BUTTON_PRESSED_SIG:
      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
      break;

    case BUTTON_RELEASED_SIG:
      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
      break;
  
  default:
    break;
  }
}

/**
 * Hàm này dùng để khởi tạo constructer
 * tức là gán hàm Button_dispatch vào vistual function là dispatch
 * của Active Object, khi đó Active Object của Button có thể gửi
 * các Event đến đúng dispatch function của nó.
 */
void Button_ctor (Button * const self) {
  Active_ctor (&self->super, (DispatchHandler) &Button_dispatch);
}

