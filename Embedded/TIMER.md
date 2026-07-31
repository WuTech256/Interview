# Timers in Embedded Systems - Interview Speaking Style

## Tell me about timers in embedded systems

So timers are honestly one of the most fundamental peripherals you deal with in embedded systems. Basically a timer is a counter that increments at a known frequency. You set it up, it counts, and at certain points it fires interrupts or generates output signals.

The basic structure is pretty simple. You have a clock source, which is usually derived from your system clock. So if your MCU runs at 168 MHz, the timer might use that directly or divide it down with a prescaler. The prescaler lets you adjust the counter frequency. Then you have the counter register itself which just increments every clock pulse, and typically you have a compare unit where you set a value, and when the counter reaches that value it does something - fires an interrupt, toggles an output pin, generates a PWM signal.

On Cortex-M MCUs we have several types of timers. SysTick is specifically for the RTOS tick - I talked about that earlier. Then you have general-purpose timers like Timer0, Timer1, etc. that can do various things. And there's usually a watchdog timer for system safety.

## How do you configure a timer for a specific frequency?

This is where prescaler comes in. Let's say you want to count at 1 MHz but your system clock is 168 MHz. The prescaler divides the clock. So prescaler value of 167 gives you 168/168 = 1 MHz. Note it's 167 not 168 - the prescaler register value is the divider minus one. That catches a lot of people.

The formula is: prescaler = (system_clock / desired_frequency) - 1

So once you have the right prescaler, the counter increments at your desired frequency. If you set up a compare value of 1000, then with a 1 MHz counter, you get an interrupt every 1 millisecond.

The key thing is understanding the relationship between prescaler, compare value, and interrupt frequency. Get the prescaler wrong and all your timing is off.

## What about counter wraparound?

A 16-bit counter maxes out at 65535, then rolls back to 0. For timing measurements you need to handle this.

Actually the way it works out is pretty neat. If you do simple subtraction like `elapsed = counter_now - counter_start`, unsigned integer arithmetic automatically handles wraparound correctly. So if counter wraps from 65535 to 0 in the middle of your measurement, the subtraction still gives you the right answer.

For example, if you start at 65530 and measure when counter is at 10, that's 10 - 65530 = -65520. But in unsigned arithmetic that wraps around and gives you 20, which is correct - we went from 65530 to 65535 (6 counts) then 0 to 10 (10 counts) = 16 counts total. Wait, that's not matching. Let me think about that again. Actually 65536 - 65520 = 16, so we need 16 counts. The modular arithmetic of unsigned subtraction handles this.

The point is you don't need special wraparound handling code if you use unsigned integers and simple subtraction.

## How would you implement a timeout using a timer?

Timeouts are pretty important in real systems. You start an operation, set a timeout, and if the timeout expires before the operation completes, you handle the timeout.

So you'd set up the timer with your desired timeout value, reset the counter, start your operation. Then either in an interrupt or in polling code, you check if the operation completed before the timer fired. If the operation completes, you stop the timer. If the timer fires, you know you have a timeout.

In an RTOS context, this is often abstracted away - you call a blocking function with a timeout parameter. But underneath, the RTOS is using a timer to ensure that the block doesn't last forever. If the event doesn't happen within the timeout, the task unblocks and gets control back.

The tricky part is choosing appropriate timeout values. Too short and you get spurious timeouts on slow operations. Too long and you have poor responsiveness when something actually fails. In real systems you measure typical operation times and set timeouts well above that with some margin.

## Explain PWM

PWM stands for Pulse Width Modulation. It's a way of generating an analog-like signal using digital outputs.

The idea is you have a period - let's say 1000 microseconds. And you have a duty cycle - the fraction of time the signal is high. If duty is 25 percent, then for the first 250 microseconds the output is high, for the remaining 750 microseconds it's low. Then the cycle repeats.

From a hardware perspective, many timers have PWM mode. You set the period in an auto-reload register and the duty cycle in a compare register. The hardware automatically toggles the output pin based on those values. Your code doesn't have to do anything - set it up once and the timer handles it forever.

This is super efficient because there's zero CPU overhead. Compare that to software PWM where you'd toggle a GPIO pin in a timer interrupt - that wastes CPU time and can have jitter depending on interrupt latency.

PWM is used all over - motor speed control, LED brightness, servo positioning. Any situation where you need an analog value using only digital outputs.

The frequency and resolution matter. If you want high frequency PWM - maybe for audio or high-speed switching - you need a fast timer. If you want high resolution in the duty cycle - many steps between 0 and 100 percent - you need a long period or fast clock.

## What's input capture?

Input capture is where a timer captures its counter value when an input pin transitions. It's the opposite of PWM in a way - instead of outputting based on timer values, you input based on timer values.

You configure a timer input pin and tell it to capture the counter value on rising edge, falling edge, or both. Then when that edge occurs, the counter value gets latched into a capture register and you get an interrupt.

This is really useful for measuring pulse widths. Let's say you have a sensor that outputs pulses. You set the timer to capture on both rising and falling edges. When a rising edge occurs, you note the timer value. When the falling edge occurs, you capture another timer value. The difference tells you the pulse width.

Input capture is more accurate than trying to measure in software because there's no latency - the hardware captures the exact moment the transition occurs.

Another use is measuring encoder outputs. Some timers can decode quadrature encoder signals directly.

## How does timer interrupt latency work?

When a compare match occurs, the timer hardware sets an interrupt flag. The CPU recognizes there's a pending interrupt, and depending on whether that interrupt is enabled and what the priority is, the CPU jumps to the ISR.

The latency from when the compare match occurs to when the ISR starts executing - that depends on several things. First the hardware latency, usually just a few clock cycles. Then whether there are higher-priority interrupts running - if so, those finish first. Then the ISR startup overhead.

For most systems the ISR startup overhead is negligible. The real variable is what other interrupts are running.

This is why interrupt priority is important. If you have a critical timer interrupt, you set it to high priority so it doesn't get delayed by other interrupts.

One thing people sometimes miss - the timer interrupt fires at a precise moment based on the clock. But the ISR latency means your code runs slightly later. If you're doing precise timing, you need to account for this. Some systems use the timer's PWM output directly instead of relying on ISR code because the hardware is more precise.

## Tell me about timer accuracy

Timer accuracy depends on the clock source you use. If you use the internal RC oscillator, it might be accurate to ±1 percent. External crystal oscillators are more accurate, maybe ±20 parts per million or better.

Temperature affects frequency too. As temperature changes, oscillator frequency drifts. So a system used in extreme temperatures might need compensation.

For most general applications, the timer accuracy is fine. But for applications like data logging where you want to timestamp events accurately, or for real-time clock applications, you need to think about this.

Crystal oscillators are more stable and accurate than RC oscillators. That's why real-time clocks use external crystals. But crystals take more power and space, so there's a tradeoff.

Jitter is another consideration. Your timer interrupt might fire at slightly different times depending on system load. If other interrupts are running, your timer interrupt gets delayed. That's timing jitter. For most applications it's not significant, but for hard real-time systems you need to measure and account for it.

## What's the difference between hardware PWM and software PWM?

Hardware PWM is where the timer peripheral generates the PWM signal automatically. You set the period and duty cycle, and the hardware toggles the output pin. Zero CPU overhead, perfectly precise, works even when CPU is asleep or handling other interrupts.

Software PWM is where your code toggles a GPIO pin in a timer interrupt to simulate PWM. It works but has downsides. First, it consumes CPU time - every timer tick you're running ISR code. Second, there's jitter - if other interrupts delay your ISR, the output timing becomes imprecise. Third, the frequency is limited by interrupt overhead.

For most applications hardware PWM is the way to go. Use software PWM only when you need extreme flexibility or when the hardware doesn't support PWM.

Actually the more I think about it, software PWM is pretty rare in modern systems because every MCU has multiple hardware timers. If you need more PWM outputs than you have timers, you could consider software PWM, but usually that's not the limitation.

## Common mistakes with timers

One classic mistake is the prescaler off-by-one error I mentioned. You want 1 MHz and write prescaler = 168 instead of 167. Then everything is slightly off frequency. That's annoying because the system still runs, just at wrong speed.

Another is forgetting to clear the interrupt flag in the ISR. If you don't clear the flag, the interrupt fires continuously and hangs the system. The ISR runs, returns, immediately fires again because the flag is still set.

Another one is not enabling the peripheral clock. Many MCUs require you to explicitly enable the clock to each peripheral. If you don't do that, the timer never gets clocked and appears to not work.

And wraparound handling - if you don't account for the counter wrapping around, your time measurements are wrong when the counter overflows. Though like I said, unsigned subtraction handles it automatically.

Also timer interrupt priority. If your timer interrupt is low priority, other interrupts can delay it and you get jitter. And if you're doing blocking operations in a timer ISR - calling blocking RTOS APIs - that can cause deadlock or unpredictable behavior.

One more - not thinking about timer frequency vs application needs. If you need nanosecond precision but your timer is running at 1 MHz (microsecond precision), you can't achieve that. You need to match the timer frequency to your timing requirements.

## How would you use timers in an RTOS context?

In an RTOS, timers are typically hidden behind the RTOS abstraction. The kernel uses SysTick to generate the periodic ticks. Tasks have delays - vTaskDelay() blocks the task for a number of ticks. That's implemented using timers internally.

Queue operations have timeouts - if you try to receive from an empty queue with a timeout, the RTOS is using a timer to ensure that the receive unblocks after the timeout. Task notifications have similar timeout support.

As an application developer, you mostly don't interact with timers directly. You just use the RTOS blocking operations with timeouts. The kernel handles the timer details.

But sometimes you need precise timing that the RTOS tick granularity doesn't provide. Maybe your tick is 1 millisecond but you need microsecond-level precision. Then you'd use a general-purpose timer directly.

Or maybe you need PWM output - hardware timers are perfect for that.

Or measuring encoder inputs - again, timer input capture is the tool.

So timers are still important in RTOS systems, just used in different ways. The kernel uses them for basic scheduling, and applications use them for specialized timing needs.

## What's the relationship between timer frequency and interrupt latency?

The timer's frequency determines how often it can fire interrupts. If a timer counts at 1 MHz with a compare value of 1000, it fires every 1 millisecond.

The interrupt latency - time from when the timer fires to when the ISR starts - doesn't directly depend on the timer frequency. It depends more on how busy the system is with other interrupts and how the priority is set.

But there is an indirect relationship. If you're using the timer interrupt for something time-critical, you need to ensure your ISR latency is small. That means keeping other interrupts short or setting the timer interrupt to high priority or both.

So the timer frequency is about your timing requirements, and latency is about system responsiveness. You can have a slow timer with fast ISR response, or fast timer with slow response. The two are separate concerns.

## Summary on timers

Timers are fundamentally counters that let you implement time-based functionality. The prescaler lets you adjust the counting frequency. The compare unit lets you generate interrupts or PWM outputs at specific values.

Key considerations are prescaler selection to get the right frequency, interrupt priority to ensure timely response, handling wraparound in measurements, and choosing between hardware and software implementations of PWM.

Understanding timers helps you implement timeouts, periodic tasks, PWM control, and precise timing measurements. In an RTOS context, the kernel uses timers for scheduling, and you use them for application-specific timing needs.
