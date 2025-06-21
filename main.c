/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : STM32F303RE - Bare-metal LED blink (PC8 & PC9)
  ******************************************************************************
  * This project blinks two external LEDs connected to PC8 and PC9
  * using register-level GPIO control and a SysTick-based delay system.
  *
  * © 2025 Francis Davis
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN PD */
#define LED_DELAY 1000              // Delay in milliseconds (1 Hz blink rate)
#define MCU_CLOCK 48000000          // STM32F303RE system clock (48 MHz)
/* USER CODE END PD */

/* USER CODE BEGIN PV */
volatile unsigned int TimingDelay = 0;  // Used by Delay() and SysTick_Handler
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Delay(unsigned int Delay);         // Custom blocking delay function
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Initialization */
  HAL_Init();                      // Initializes HAL and system timer
  SystemClock_Config();            // Configures PLL to run at 48 MHz
  SysTick_Config(MCU_CLOCK / 1000); // Configure SysTick to trigger every 1ms

  /* USER CODE BEGIN 2 */

  // Enable GPIOC clock (needed to access PC8/PC9)
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

  // Set PC8 & PC9 as general-purpose output (MODER = 01)
  GPIOC->MODER &= ~(GPIO_MODER_MODER8 | GPIO_MODER_MODER9);
  GPIOC->MODER |=  (GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0);

  // Set output type to push-pull (0 = push-pull)
  GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);

  // Set output speed to low (00)
  GPIOC->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9);

  // Disable pull-up/pull-down resistors (PUPDR = 00)
  GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);

  /* USER CODE END 2 */

  /* Infinite loop */
  while (1)
  {
    // Turn both LEDs ON (set PC8 and PC9 high)
    GPIOC->BSRR |= (GPIO_BSRR_BS_8 | GPIO_BSRR_BS_9);
    Delay(LED_DELAY);

    // Turn both LEDs OFF (reset PC8 and PC9)
    GPIOC->BRR  |= (GPIO_BRR_BR_8 | GPIO_BRR_BR_9);
    Delay(LED_DELAY);
  }
}

/**
  * @brief System Clock Configuration
  *        Configures the PLL to set system clock at 48 MHz.
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  // Configure HSI and PLL (HSI * 6 = 48 MHz)
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  // Select PLL as system clock source and configure dividers
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/**
  * @brief Custom delay function using SysTick interrupt
  * @param Delay: time in milliseconds
  */
void Delay(unsigned int Delay)
{
  TimingDelay = Delay;
  while (TimingDelay != 0);  // Wait until SysTick_Handler decrements to 0
}

/**
  * @brief SysTick Interrupt Handler - runs every 1ms
  */
void SysTick_Handler(void)
{
  TimingDelay--;     // Decrement delay counter
  HAL_IncTick();     // Maintain HAL timing if needed
}

/* USER CODE END 4 */

/**
  * @brief Called on critical error
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1);         // Trap CPU in error state
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief Report assert failures (debug builds only)
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  // You can implement custom logging here if needed
}
#endif /* USE_FULL_ASSERT */
