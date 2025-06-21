# STM32 Bare-Metal LED Blink

Blinking external LEDs on PC8 and PC9 using direct register manipulation on the STM32F303RE Nucleo board. No HAL or Cube drivers used.

## 🔧 Hardware

- STM32F303RE Nucleo board  
- Breadboard, 2x LEDs (red + white), 2x 220Ω resistors  
- PC8 and PC9 wired to LEDs via resistors to GND

## ⚙️ Code Overview

- SysTick configured for 1ms ticks  
- `MODER`, `OTYPER`, `PUPDR`, and `BSRR` used to toggle PC8/PC9
- Custom `Delay()` using `TimingDelay` + `SysTick_Handler`

## 📷 Photos

> Include your breadboard pictures, pin wiring, and blinking LED video here

## 💻 main.c Highlight

```c
GPIOC->MODER |= (1 << (8 * 2));   // Set PC8 to output
GPIOC->BSRR |= GPIO_BSRR_BS_8;    // Set PC8 high
Delay(1000);
GPIOC->BRR |= GPIO_BRR_BR_8;      // Set PC8 low
