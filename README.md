# 🧠 STM32 Nucleo LED Controller – Bare-Metal GPIO

This project demonstrates how to manually control GPIO pins on an **STM32F303RE Nucleo board** using **register-level programming**, without relying on the STM32 HAL or CubeMX libraries.

By writing directly to CMSIS-defined registers, you gain full control over GPIO configuration and behavior — ideal for learning the inner workings of STM32 hardware.

---

## 🎯 Project Goal

To blink two external LEDs using:

- Direct register access (CMSIS)
- Custom delay logic with SysTick
- Zero use of HAL or Cube-generated code

---

## 🔧 Hardware Used

- **STM32F303RE** Nucleo Board  
- Breadboard  
- 2x Red LEDs  
- 2x 220Ω Resistors  
- Jumper wires (Male-to-Male)

### 🔌 Circuit Wiring

| MCU Pin | Connection                     |
|---------|--------------------------------|
| PC8     | → Resistor → LED → GND         |
| PC9     | → Resistor → LED → GND         |

---

## 🚦 What This Project Does

Two LEDs connected to PC8 and PC9 blink **alternating HIGH/LOW states** with a 1-second delay in between.

All GPIO setup is done using **CMSIS bit masks**, and timing is achieved via the **SysTick timer** and a manually maintained delay counter.

---

## 💻 How It Works

### ✅ GPIO Configuration

```c
// Set PC8 and PC9 as general purpose output
GPIOC->MODER &= ~(GPIO_MODER_MODER8 | GPIO_MODER_MODER9);
GPIOC->MODER |=  GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0;

// Set as push-pull outputs, no pull-up/pull-down, low speed
GPIOC->OTYPER  &= ~(GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);
GPIOC->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9);
GPIOC->PUPDR   &= ~(GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);
```

### ⏲️ SysTick Delay Implementation

- SysTick configured to generate an interrupt every 1ms
- Global `volatile` counter decremented inside `SysTick_Handler`
- Custom `Delay(ms)` waits until counter reaches zero

```c
void SysTick_Handler(void) {
    if (timer > 0) timer--;
}

void Delay(uint32_t ms) {
    timer = ms;
    while (timer > 0);
}
```

### 🔁 LED Blinking Loop

```c
// Main loop
while (1) {
    GPIOC->BSRR |= (GPIO_BSRR_BS_8 | GPIO_BSRR_BS_9); // Turn LEDs ON
    Delay(1000);
    GPIOC->BRR  |= (GPIO_BRR_BR_8 | GPIO_BRR_BR_9);   // Turn LEDs OFF
    Delay(1000);
}
```

---

## 💡 Blinking Demo Video

[![Watch the demo](https://img.youtube.com/vi/cPPfdc07ZzA/hqdefault.jpg)](https://www.youtube.com/watch?v=cPPfdc07ZzA)

---
