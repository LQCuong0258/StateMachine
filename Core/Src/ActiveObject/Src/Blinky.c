#include "Blinky.h"

extern TIM_HandleTypeDef htim7;

/**
 * Hàm này sẽ xử lý khi nhận được sự kiện từ Active Object
 */
static void Blinky_dispatch (Blinky1 * const self, Event const * const event) {
  switch (event->signal) {
    /**
     * Đây là Event khi ActiveObject được khởi tạo
     * trong trường hợp này đang khi được khởi tạo
     * sẽ bật timer bắt đầu cout
     */
    case INIT_SIG:
      HAL_TIM_Base_Start_IT(&htim7); // Bắt đầu Timer với ngắt
      break;
    /**
     * Vì đây là một sự kiện thời gian được post từ timer
     * nên khi timer triger, nó sẽ gửi Event TIME_OUT_SIG đến Active Object
     * khi đó led sẽ Toggle đúng chu kỳ đã được cấu hình cho timer
     */
    case TIME_OUT_SIG:
      HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
      break;
  
    default:
      break;
  }
}

/**
 * Hàm này dùng để khởi tạo constructer
 * tức là gán hàm Blinky_dispatch vào vistual function là dispatch
 * của Active Object, khi đó Active Object của Blinky có thể gửi
 * các Event đến đúng dispatch function của nó.
 */
void Blinky1_ctor (Blinky1 * const self) {
  Active_ctor (&self->super, (DispatchHandler) &Blinky_dispatch);
}

