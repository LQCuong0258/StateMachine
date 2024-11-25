#include "main.h"


static struct Blinky blinky;
static struct Active * blinky_AO = &blinky.super;


static struct Button button;
static struct Active * button_AO = &button.super;


int main(void) {
  HAL_Init();
  SystemClock_Config();
  MX_Periferal_Init();

  Blinky.constructor(&blinky);
  blinky.super.start(blinky_AO, 2, 10, configMINIMAL_STACK_SIZE);

  Button.constructor(&button);
  button.super.start(button_AO, 3, 10, configMINIMAL_STACK_SIZE);

  vTaskStartScheduler();
}




