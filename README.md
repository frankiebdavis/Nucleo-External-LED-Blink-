# 🧠 STM32 Nucleo LED Controller

This project demonstrates how to manually control GPIO pins on an **STM32F303RE Nucleo board** using **register-level programming**, without relying on the STM32 HAL or CubeMX libraries.

By writing directly to CMSIS-defined registers, you gain full control over GPIO configuration and timing — ideal for learning the inner workings of STM32 hardware.

---

## ❓ Problem

How can two external LEDs be toggled with precise timing **without using high-level libraries**?  
The challenge: directly configure GPIO registers and build a custom timing system using the SysTick peripheral.

---

## 🔨 Method

- **Direct Register Access (CMSIS):** Configure PC8 and PC9 as push-pull outputs, with no pull-ups/pull-downs, low speed.  
- **SysTick Timer:** Configure to tick every 1 ms, decrementing a global counter.  
- **Custom Delay:** Implement a blocking `Delay(ms)` function based on this counter.  
- **Main Loop:** Alternate ON/OFF states with a 1000 ms delay for a 1 Hz blink rate.  

---

## ✅ Result

- Two LEDs connected to PC8 and PC9 blink **once per second**.  
- Entirely achieved through **bare-metal register programming** and a SysTick-based delay.  
- Reinforces low-level embedded systems skills: GPIO configuration, timing, and register control.  

---

## 🔧 Hardware Used

- **STM32F303RE** Nucleo Board  
- Breadboard  
- 2× LEDs (red)  
- 2× 220 Ω resistors  
- Jumper wires  

### 🔌 Circuit Wiring

| MCU Pin | Connection                     |
|---------|--------------------------------|
| PC8     | → Resistor → LED → GND         |
| PC9     | → Resistor → LED → GND         |

---

## 💻 Demo Code

```c
// ================= GPIO Configuration =================

// Set PC8 and PC9 as general purpose output
GPIOC->MODER &= ~(GPIO_MODER_MODER8 | GPIO_MODER_MODER9);
GPIOC->MODER |=  GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0;

// Push-pull, no pull-up/pull-down, low speed
GPIOC->OTYPER  &= ~(GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);
GPIOC->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9);
GPIOC->PUPDR   &= ~(GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);

// ================= SysTick Delay Implementation =================

volatile uint32_t timer = 0;

void SysTick_Handler(void) {
    if (timer > 0) timer--;
}

void Delay(uint32_t ms) {
    timer = ms;
    while (timer > 0);
}

// ================= LED Blinking Loop =================

while (1) {
    GPIOC->BSRR |= (GPIO_BSRR_BS_8 | GPIO_BSRR_BS_9); // Turn LEDs ON
    Delay(1000);
    GPIOC->BRR  |= (GPIO_BRR_BR_8 | GPIO_BRR_BR_9);   // Turn LEDs OFF
    Delay(1000);
}
```

## 💡 Blinking Demo Video

[![Watch the demo](https://img.youtube.com/vi/cPPfdc07ZzA/hqdefault.jpg)](https://www.youtube.com/watch?v=cPPfdc07ZzA)

---
