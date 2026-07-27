# DMA
So, DMA stands for Direct Memory Access, and the core idea is: it lets data move between a peripheral and memory — or even memory to memory — 
without the CPU having to babysit every single byte.

Let me give an analogy. Without DMA, if you want to move data from, say, a UART into RAM, the CPU has to sit there in a loop — read a byte, write a byte, read a byte, 
write a byte — over and over. That's a total waste of CPU time on something that's basically just copying. 
So DMA offloads that whole job to a dedicated piece of hardware called the DMA controller, and the CPU is free to go do actual useful computation while that's happening.

Here's roughly how it works. First, the CPU configures the DMA controller — it tells it the source address, the destination address, 
how much data to move, and what mode to use. Then the transfer either starts right away, or it's triggered by the peripheral itself, like 'hey, UART has a byte ready.' At that point, the DMA controller takes over the bus — this is called bus mastering, sometimes people call it cycle stealing, because it's briefly stealing bus access from the CPU — and it moves the data directly between the peripheral and memory. Once the transfer's done, or a buffer's full, the DMA controller fires an interrupt to let the CPU know, like 'hey, I'm done, come process this.' So that's actually where DMA and interrupts connect — DMA does the grunt work, and the interrupt is how it reports back.

A few other things worth mentioning: most DMA controllers have multiple channels, so you can have one channel handling your ADC,
 another handling SPI, and so on, all independently. There's also different transfer modes — you can do a one-shot transfer, 
 or circular mode where it just keeps wrapping around a buffer, which is great for continuous stuff like audio streaming. And a common technique is double buffering, or ping-pong buffering, where DMA is filling one buffer while the CPU processes the other, and then they swap — that way you don't lose data in a continuous stream.

And if an interviewer wants to see depth, I'd bring up two trade-offs. One is cache coherency — if the CPU has cached data 
and DMA writes straight to RAM behind the cache's back, the CPU might read stale data, 
so you need to either invalidate the cache or use non-cacheable memory for DMA buffers. The other is that DMA doesn't use CPU cycles, 
but it still competes for memory bus bandwidth, so heavy DMA traffic can still slow the CPU down indirectly.

So bottom line — DMA is a hardware mechanism that offloads data transfers from the CPU to a dedicated controller, 
which is way more efficient for large or continuous transfers, and it typically notifies the CPU it's done using an interrupt
