SysTick is a 24-bit down-counting timer built into the Cortex-M core. It counts down from the value in the LOAD register to zero, sets the COUNTFLAG, and then automatically reloads the value from LOAD.

It has three main registers: LOAD for the reload value, VAL for the current counter value, and CTRL to enable the timer, enable interrupts, and select the clock source.

The reload value is calculated based on the desired tick period and the system clock frequency. For example, to generate a 1 millisecond tick with a 16 MHz clock, the reload value is 16,000 - 1.

SysTick can be used in two common ways. For simple timing or blocking delays, software can poll the COUNTFLAG until the counter reaches zero. Alternatively, SysTick can generate periodic interrupts by enabling the SysTick interrupt. This periodic interrupt is commonly used as the system time base for an RTOS scheduler, for example generating a 1 millisecond tick to manage task delays and scheduling.
