/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "button.h"  /* Free Active Object interface */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
TIM_HandleTypeDef htim7;

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/* USER CODE BEGIN PFP */
void MX_TIM7_Init(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*-------------------------------------------------------------------------------------------------------*/
/* The Blinky Active Object */
typedef struct {
  Active super;
} Blinky;

/**
 * Hàm này sẽ xử lý khi nhận được sự kiện từ Active Object
 */
static void Blinky_dispatch (Blinky * const self, Event const * const event) {
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
      HAL_GPIO_TogglePin(GPIOD, LedOrange);
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
void Blinky_ctor (Blinky * const self) {
  Active_ctor (&self->super, (DispatchHandler) &Blinky_dispatch);
}

static Blinky blinky;
Active *AO_blinky = &blinky.super;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  if (htim->Instance == TIM7) {
    static const Event time_evt = { .signal = TIME_OUT_SIG };
    /* Send the time out event to the Blinky Active Object */
    Active_postFromISR(AO_blinky, &time_evt, &xHigherPriorityTaskWoken);
  }
  portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

/**
 * This thread will manage the Toggle led
 */
// static void blinky_thread (void *pvParameters) {
//   (void) pvParameters;
//   for(;;) {
//     HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
//     vTaskDelay(pdMS_TO_TICKS(1000));
//   }
// }
/*-------------------------------------------------------------------------------------------------------*/
/* The Button Active Object */
typedef struct {
  Active super;
} Button;

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

static Button button;
Active *AO_button = &button.super;

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
    Active_postFromISR(AO_button, &btnEvt, &xHigherPriorityTaskWoken);
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
  }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM7_Init();

  /* USER CODE BEGIN 2 */

  // xTaskCreate(blinky_thread, "Blinky_Thread", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
  Blinky_ctor(&blinky);
  Active_start(AO_blinky, 2, 10, configMINIMAL_STACK_SIZE);

  Button_ctor(&button);
  Active_start(AO_button, 2, 10, configMINIMAL_STACK_SIZE);

  vTaskStartScheduler();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}



/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  HAL_GPIO_WritePin(GPIOD, LedGreen | LedRed | LedBlue | LedOrange, Reset);
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pin   = LedGreen | LedRed | LedBlue | LedOrange;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  GPIO_InitStruct = (GPIO_InitTypeDef) {0};
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING; // Hoặc GPIO_MODE_IT_FALLING, GPIO_MODE_IT_RISING_FALLING
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Khởi tạo NVIC cho EXTI0
  HAL_NVIC_SetPriority(EXTI0_IRQn, 7, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
// Hàm khởi tạo cho Timer 7
void MX_TIM7_Init (void) {
  __HAL_RCC_TIM7_CLK_ENABLE(); // Bật clock cho Timer 7

  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 8400 - 1; // Prescaler để Timer chạy ở tần số 100 kHz (chu kỳ 1 ms)
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 10000 - 1; // Đặt chu kỳ ngắt là 100 ms
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

  if (HAL_TIM_Base_Init(&htim7) != HAL_OK) {
      // Xử lý lỗi nếu cần
  }
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
      // Error_Handler();
  }

  // Kích hoạt ngắt
  HAL_NVIC_SetPriority(TIM7_IRQn, 8, 0);
  HAL_NVIC_EnableIRQ(TIM7_IRQn);
}
void TIM7_IRQHandler(void) {
  HAL_TIM_IRQHandler(&htim7);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
