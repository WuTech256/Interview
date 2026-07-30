## Interview Answer: Explain the MCU Used in Your Project

In my current project, we use a dual-MCU architecture for smart home appliances. 
We have two different microcontrollers: one is for the main control and user interface, and the other one is for low-level hardware control.

For the Load Control Processor, or LCP, we use a custom board based on a Toshiba TMPM380 series MCU. 
It is an ARM Cortex-M3 microcontroller.

The main purpose of this MCU is real-time hardware control, especially for motor and actuator control.

Some important peripherals of this MCU include:

- PWM modules, which are used for motor inverter control.
- ADC channels, which are used to read sensor signals such as temperature, water level, and current feedback.
- Hardware timers and interrupts, which provide accurate timing and fast response for real-time control tasks.
- Communication interfaces such as UART, SPI, and I2C. In our system, UART is mainly used for communication between LCP and the main MCU.

For the main MCU, called LUPA, we use a more powerful ARM Cortex-M based microcontroller, typically Cortex-M4 or Cortex-M7 class depending on the product.

This MCU is responsible for higher-level functions such as application logic, display control, and user interface.

Its main features include:

- More Flash and RAM memory to store application firmware, UI resources, fonts, and graphics data.
- LCD or graphics controller support for driving the display efficiently.
- More powerful CPU performance for running the application framework and RTOS.
- Communication interfaces such as UART, SDIO, or other high-speed interfaces for external modules like Wi-Fi.

In our project, the LUPA MCU runs FreeRTOS, which helps manage multiple tasks such as UI handling, communication, and application logic.
