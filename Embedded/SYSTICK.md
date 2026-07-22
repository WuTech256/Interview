SysTick is a 24-bit down-counting timer built into the Cortex-M core. 
It counts down from the value in the LOAD register to zero, sets the count flag, then reloads automatically.

It has 3 main registers: LOAD for the reload value, VAL for the current value, and CTRL to enable the timer, enable interrupts, 
and choose the clock source. To create a delay, compute the reload value from the desired delay times the system clock.
For example, 16,000 ticks for 1 millisecond at 16 megahertz. You can either poll the count flag in a loop for a simple blocking delay, 
or enable the SysTick interrupt to generate a periodic tick, like 1 millisecond, which an RTOS uses as the heartbeat for its scheduler
