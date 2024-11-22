#include "main.h"

static Blinky1 blinky1;
Active *AO_blinky1 = &blinky1.super;

static Button button;
Active *AO_button = &button.super;


int main(void) {
  HAL_Init();
  SystemClock_Config();
  MX_Periferal_Init();

  Blinky1_ctor(&blinky1);
  Active_start(AO_blinky1, 2, 10, configMINIMAL_STACK_SIZE);

  Button_ctor(&button);
  Active_start(AO_button, 2, 10, configMINIMAL_STACK_SIZE);

  vTaskStartScheduler();
}




