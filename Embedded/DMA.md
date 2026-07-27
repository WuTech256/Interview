# DMA
DMA stands for Direct Memory Access. It is a hardware mechanism that allows data transfer between peripherals and memory without requiring the CPU to handle every single byte transfer.

Normally, without DMA, if we receive data from UART, the CPU needs to read the UART data register and copy each byte into RAM. This increases CPU load, especially when handling high-speed communication.

With DMA, the CPU only needs to configure the DMA channel, including the source address, destination address, transfer direction, data width, and transfer count. After configuration, the DMA controller can transfer data automatically in the background while the CPU continues executing other tasks.

For example, in a UART communication case, when UART receives data, it generates a DMA request. The DMA channel automatically moves data from the UART data register to a RAM buffer. After the transfer is completed, DMA generates an interrupt to notify the CPU.

In the A34M420 MCU, DMA is organized into 16 DMA channels. Each channel can be configured independently for a specific data transfer. The configuration includes source address, destination address, transfer size, transfer mode, and request source mapping.

DMA supports different transfer directions:

Peripheral-to-memory, for example receiving UART data or storing ADC conversion results into RAM.
Memory-to-peripheral, for example sending data through UART or SPI.
Memory-to-memory, for copying data between memory areas.

Another important feature is address increment mode. Normally, peripheral addresses remain fixed because peripheral registers have fixed locations, while the memory address can automatically increase after each transfer to fill a buffer.

In our project, DMA helps reduce CPU overhead by transferring communication data directly between peripherals and memory. The CPU mainly handles initialization and transfer completion events instead of processing every byte.

So basically, DMA improves system performance by allowing efficient data movement with minimal CPU involvement.
