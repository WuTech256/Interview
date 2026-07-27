# MCU

> "Since my company uses a dual-MCU architecture for our smart appliances, I will describe the features of both chips we are using:
> 
> **1. The LCP Chip (Load Control Processor):**
> For the LCP, we use a custom board based on the **Toshiba TMPM380**, which is an **ARM Cortex-M3** microcontroller. Its main purpose is hardware and motor control. Its key features include:
> * **High-Resolution PWM (Pulse Width Modulation):** Essential for precise control of the washing machine's inverter motor.
> * **Multi-channel ADC (Analog-to-Digital Converter):** Used to rapidly and accurately read analog signals from various sensors, such as temperature, water level, and IR sensors.
> * **Hardware Timers & Interrupts:** It has robust hardware timers for real-time task execution and fast interrupt response times, which is critical for safety features like over-current or over-temperature protection.
> * **Communication Interfaces:** It includes standard peripherals like **UART** (which we use to talk to the main chip), SPI, and I2C.
> 
> **2. The LUPA Chip (Main / Display MCU):**
> For the LUPA platform, we use a more powerful microcontroller (typically an **ARM Cortex-M4 or M7 class**) because it needs to drive a graphical display. Its key features include:
> * **LCD Controller / GDC (Graphics Display Controller):** It has built-in hardware to push pixel data (framebuffers) directly to the physical LCD panel efficiently.
> * **Larger Memory (RAM & Flash):** It requires significantly more internal SRAM and external Flash memory to store UI assets, fonts, and multiple application binaries.
> * **Networking Interfaces:** It includes high-speed interfaces like SDIO or high-speed UART to connect to an external Wi-Fi/IoT module.
> * **RTOS Support (MPU):** The architecture includes an MPU (Memory Protection Unit) which is great for running FreeRTOS safely."
