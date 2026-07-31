# GPIO Basics - Simple Interview Style

## What is GPIO?

GPIO stands for General Purpose Input Output. It's literally just pins on your microcontroller that you can control. You can set them to output a voltage - high or low. Or you can read them as input to see if something is connected.

That's really it. GPIO is super simple conceptually. It's just digital I/O - on or off, high or low, 1 or 0.

On an STM32 microcontroller, you might have GPIO Port A, Port B, Port C, etc. Each port has multiple pins - typically 16 pins per port. So Port A has pins PA0 through PA15. Each pin can be independently controlled.

## What are the basic GPIO modes?

Each GPIO pin can be configured in different modes.

**Output mode** - You control the pin voltage. You can set it high (typically 3.3 volts) or low (0 volts). This is how you control LEDs, relays, motors, anything that needs a digital signal.

**Input mode** - You read the pin voltage. You can check if the pin is high or low. This is how you read buttons, sensors, signals from other circuits.

**Alternate function** - This is when you're not using the pin as a regular GPIO. Maybe it's connected to a UART, SPI, I2C, or timer. The peripheral controls the pin instead of you.

**Analog** - The pin is used for analog functions like ADC (analog to digital conversion).

Most of the time you're either configuring pins as output or input.

## How do you control a GPIO pin?

Let's say you want to control an LED on pin PA5. First you configure the pin as output. Then you set it high to turn the LED on, set it low to turn it off.

In code it's simple:

```c
// Configure PA5 as output
GPIO_InitTypeDef GPIO_InitStruct;
GPIO_InitStruct.Pin = GPIO_PIN_5;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
GPIO_InitStruct.Pull = GPIO_NOPULL;
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

// Set PA5 high
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

// Set PA5 low
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

// Read PA5
GPIO_PinState state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);
```

The mode `GPIO_MODE_OUTPUT_PP` means output with push-pull. That's the most common. Push-pull means the pin actively drives high or low - it has the strength to drive current.

There's also open-drain mode where the pin can only pull low, not push high. That's used in I2C and other open-drain applications.

## What does pull-up and pull-down mean?

A pull-up resistor connects the pin to the positive voltage through a resistor. If nothing is driving the pin, it defaults to high.

A pull-down resistor connects the pin to ground through a resistor. If nothing is driving the pin, it defaults to low.

These are useful for input pins. Let's say you have a button. When pressed, it connects the pin to ground. When not pressed, the pin is floating - undefined. If you enable a pull-up, the pin is pulled high when the button isn't pressed. When you press the button, it goes low. Now you have a clean signal.

Most microcontrollers have built-in pull-up and pull-down resistors. You can enable them in software. That saves you from needing external resistors.

For an input pin reading a button, you typically use pull-up. The pin is high when the button isn't pressed, goes low when pressed.

## How do you read a button?

Reading a button is simple. Configure the pin as input with pull-up enabled. Then read the pin.

```c
// Configure PA0 as input with pull-up
GPIO_InitTypeDef GPIO_InitStruct;
GPIO_InitStruct.Pin = GPIO_PIN_0;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_PULLUP;
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

// Read the button
if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET) {
    // Button is pressed
} else {
    // Button is not pressed
}
```

The pull-up means the pin is normally high. When you press the button, it connects to ground and goes low. So you check for GPIO_PIN_RESET when pressed.

One thing to watch - button bouncing. When you physically press a button, it doesn't instantly go from high to low. There's bouncing - it might go low, bounce back high, go low again, over a few milliseconds. If you just read the pin once, you might catch it mid-bounce and get inconsistent results.

The solution is debouncing - read the pin multiple times with a small delay, or use a timer to ignore changes for a few milliseconds after the first change. Or use a capacitor on the button pin to smooth out the bounces.

## What's the difference between digital and analog GPIO?

Digital GPIO is just high or low - 1 or 0. You're not measuring the exact voltage, just whether it's above or below a threshold (usually around half the supply voltage).

Analog GPIO is connected to an ADC - analog to digital converter. The ADC measures the exact voltage and converts it to a digital value. So instead of just knowing if a sensor is high or low, you get the precise voltage.

If you're reading a digital signal like a button or interrupt, use digital GPIO. If you're reading a variable voltage like a temperature sensor or light sensor, use analog GPIO with ADC.

## What about GPIO interrupts?

Some pins can generate interrupts when they change state. Let's say you have a button and you want your code to react immediately when it's pressed, not constantly poll it.

You configure the GPIO pin for interrupt on falling edge (when it goes from high to low, which is when the button is pressed). Then you attach an ISR - interrupt service routine. When the button is pressed, the hardware fires the interrupt and your ISR code runs.

```c
// Configure PA0 for interrupt on falling edge
GPIO_InitTypeDef GPIO_InitStruct;
GPIO_InitStruct.Pin = GPIO_PIN_0;
GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
GPIO_InitStruct.Pull = GPIO_PULLUP;
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

// In ISR
void EXTI0_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

// Callback when interrupt occurs
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_0) {
        // Button pressed
    }
}
```

The advantage of GPIO interrupts over polling is efficiency. You don't waste CPU constantly checking the pin. The hardware tells you when something happens.

The disadvantage is that you still need to handle debouncing, and interrupt overhead can be significant if you have many GPIO interrupts.

## GPIO voltage levels and safety

Most microcontroller GPIO pins operate at 3.3 volts or 5 volts, depending on the device. An STM32 typically uses 3.3 volts.

If you connect a 5-volt signal to a 3.3-volt GPIO pin, you can damage the pin. The pin can only tolerate voltage up to the supply voltage plus a small amount (like 0.3 volts).

If you need to interface with 5-volt devices, use level shifters - circuits that convert between voltage levels.

Similarly, the GPIO pin can only source or sink a limited amount of current - typically 20-30 milliamps per pin, or a few hundred milliamps total for all pins. If you try to drive a high-power device directly from GPIO, you'll exceed this and damage the pin or the device won't work.

For driving high-power loads like motors or relays, use a transistor or MOSFET to amplify the GPIO signal. The GPIO drives the transistor, and the transistor drives the load.

## Common GPIO mistakes

**Forgetting to enable the GPIO clock** - Many microcontrollers require you to explicitly enable the clock to each GPIO port. If you don't do this, the port doesn't work. Very annoying because you don't get an error, the code just doesn't work.

**Using wrong voltage levels** - Connecting 5V to a 3.3V pin, or trying to source 500mA from a pin rated for 20mA.

**Not debouncing buttons** - Reading the button immediately and getting unreliable results due to bouncing.

**Button debouncing in interrupt** - If you use GPIO interrupts for buttons but don't debounce, you get multiple interrupts from one button press due to bouncing.

**Not setting pull-up/pull-down correctly** - Reading a floating input pin that isn't pulled up or down gives undefined results.

**Forgetting GPIO initialization** - Not initializing GPIO before using it. The pin is in some default state.

**Using the wrong pin** - Configuring PA5 but then reading PA6. Silly mistake but happens.

## How would you blink an LED?

This is the "Hello World" of embedded systems.

```c
// Configure PA5 as output
GPIO_InitTypeDef GPIO_InitStruct;
GPIO_InitStruct.Pin = GPIO_PIN_5;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
GPIO_InitStruct.Pull = GPIO_NOPULL;
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

// In main loop
while (1) {
    // Turn on
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    HAL_Delay(500);  // Wait 500ms
    
    // Turn off
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    HAL_Delay(500);  // Wait 500ms
}
```

That's it. Set high, wait, set low, wait, repeat. The LED blinks.

HAL_Delay() just waits for the specified milliseconds. It's a blocking delay so the CPU is doing nothing while waiting. For real applications you'd use a timer or RTOS to avoid wasting CPU, but for blinking an LED this works fine.

## GPIO and peripheral alternate functions

GPIO pins are flexible. A pin might be usable as:
- Regular GPIO
- UART TX or RX
- SPI MOSI or MISO
- I2C SDA or SCL
- Timer PWM output
- ADC input
- Other peripheral functions

The linker script or device datasheet defines which pins can do which functions. Then you configure the pin's mode to be "alternate function" and it's automatically connected to that peripheral.

So if you want to use PA9 as UART TX, you configure it as alternate function AF7 for UART. The UART peripheral controls that pin, not you manually.

This is powerful because you can reuse the same pins for different purposes depending on what your application needs.

## GPIO in an RTOS context

In an RTOS, GPIO operations are still simple. You still configure pins and read/write them. The only difference is that if you're reading a GPIO in a high-priority task, and a lower-priority task also reads that GPIO, you need to be careful about timing.

Actually GPIO reads and writes are usually atomic single instructions, so there's no concurrency issue. Multiple tasks can read the same GPIO without synchronization because it's just reading a register.

If multiple tasks are writing to GPIO, you might need synchronization to ensure consistent state, but usually each pin is owned by one task.

GPIO interrupts work normally in an RTOS context. The ISR runs, reads the pin, and maybe signals a task through a queue or semaphore.

## Summary

GPIO is probably the simplest peripheral you work with. It's just digital I/O - read and write pins.

Key points:
- Configure pins as input or output
- Set pull-up or pull-down for inputs
- Debounce buttons
- Respect voltage levels and current limits
- Enable GPIO clocks
- Use alternate functions for peripherals

It's simple but you need to understand the details - voltage levels, current limits, debouncing, interrupt handling. Master GPIO and you have the foundation for understanding more complex peripherals.
