/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
// void SVC_Handler(void)
// {
//   /* USER CODE BEGIN SVCall_IRQn 0 */

//   /* USER CODE END SVCall_IRQn 0 */
//   /* USER CODE BEGIN SVCall_IRQn 1 */

//   /* USER CODE END SVCall_IRQn 1 */
// }

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
// void PendSV_Handler(void)
// {
//   /* USER CODE BEGIN PendSV_IRQn 0 */

//   /* USER CODE END PendSV_IRQn 0 */
//   /* USER CODE BEGIN PendSV_IRQn 1 */

//   /* USER CODE END PendSV_IRQn 1 */
// }

/**
  * @brief This function handles System tick timer.
  */
// void SysTick_Handler(void)
// {
//   /* USER CODE BEGIN SysTick_IRQn 0 */

//   /* USER CODE END SysTick_IRQn 0 */
//   HAL_IncTick();
//   /* USER CODE BEGIN SysTick_IRQn 1 */

//   /* USER CODE END SysTick_IRQn 1 */
// }

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

extern Active * AO_button;
extern Active * AO_blinky;

/* USER CODE BEGIN 1 */
// Xử lý ngắt cho EXTI0 (PA0)
void EXTI0_IRQHandler (void) {
  // Kiểm tra xem ngắt có phải từ PA0 không
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != RESET) 
  {
    // Xóa cờ ngắt (Clear interrupt flag)
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    Event btnEvt;

    /* Đọc trạng thái của nút nhấn */
    uint8_t btnStatus = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
    /* Nếu bắt được trạng thái nhấn nút */
    if (btnStatus == GPIO_PIN_SET) {
      btnEvt.signal = BUTTON_PRESSED_SIG;
    }
    /* Nếu bắt được trạng thái nhả nút */
    else {
      btnEvt.signal = BUTTON_RELEASED_SIG;
    }

    /* Post Event from ISR to ActiveObject */
    AO_button->postFromISR(AO_button, &btnEvt, &xHigherPriorityTaskWoken);
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  if (htim->Instance == TIM7) {
    static const Event time_evt = { .signal = TIME_OUT_SIG };
    /* Send the time out event to the Blinky Active Object */
    AO_blinky->postFromISR(AO_blinky, &time_evt, &xHigherPriorityTaskWoken);
  }
  portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}
/* USER CODE END 1 */
