# DMA
DMA stands for Direct Memory Access. It is a hardware mechanism that allows data transfer between peripherals and memory without requiring the CPU to handle every single byte transfer.

Normally, without DMA, if we receive data from UART, the CPU needs to read the UART data register and copy each byte into RAM. This increases CPU load, especially when handling high-speed communication.

With DMA, the CPU only needs to configure the DMA channel, including the source address, destination address, transfer direction, data width, and transfer count. After configuration, the DMA controller can transfer data automatically in the background while the CPU continues executing other tasks.

For example, in a UART communication case, when UART receives data, it generates a DMA request. The DMA channel automatically moves data from the UART data register to a RAM buffer. After the transfer is completed, DMA generates an interrupt to notify the CPU.

In the A34M420 MCU, DMA is organized into 16 DMA channels. Each channel can be configured independently for a specific data transfer. The configuration includes source address, destination address, transfer size, transfer mode, and request source mapping.

DMA supports different transfer directions:

- Peripheral-to-memory, for example receiving UART data or storing ADC conversion results into RAM.
- Memory-to-peripheral, for example sending data through UART or SPI.
- Memory-to-memory, for copying data between memory areas.

Another important feature is address increment mode. Normally, peripheral addresses remain fixed because peripheral registers have fixed locations, while the memory address can automatically increase after each transfer to fill a buffer.

In our project, DMA helps reduce CPU overhead by transferring communication data directly between peripherals and memory. The CPU mainly handles initialization and transfer completion events instead of processing every byte.

So basically, DMA improves system performance by allowing efficient data movement with minimal CPU involvement.

# FIFO Mode and Direct Mode
## Direct Mode
In Direct Mode, data is transferred directly between the peripheral and memory without using an internal FIFO buffer.

The advantages are:

- Lower latency
- Simpler operation
- Less internal buffering

This mode is suitable when the peripheral data width and memory data width are the same, for example UART 8-bit to RAM 8-bit.
##  FIFO Mode
In FIFO Mode, DMA uses an internal FIFO buffer, typically 4 words deep in many STM32-like MCUs.

The DMA can first store several data items in the FIFO, then transfer them to memory in bursts.

The advantages are:

- Better bus efficiency
- Support for burst transfers
- Useful when peripheral and memory data widths are different
- Can reduce bus arbitration overhead

For example, if a peripheral provides 8-bit data but memory is accessed more efficiently in 32-bit words, the FIFO can pack four 8-bit bytes into one 32-bit transfer.

FIFO mode is commonly used for SPI, SDIO, I2S, camera interfaces, or high-throughput data streams.

## When would I choose each mode?

- UART RX/TX: usually Direct Mode is enough.
- ADC continuous sampling: often Direct Mode or Circular Mode.
- SPI with large buffers: FIFO Mode can improve throughput.
- Memory-to-memory large copy: FIFO Mode with burst transfer is usually more efficient.

# Transfer mode
DMA supports two main transfer modes: Normal Mode and Circular Mode.

In Normal Mode, DMA transfers data only once. After the transfer is completed, DMA stops and generates a transfer complete interrupt. The CPU needs to restart DMA for the next transfer.

In Circular Mode, after completing the transfer, DMA automatically reloads the transfer settings and starts again from the beginning of the buffer. This mode is suitable for continuous data streams, such as UART reception or ADC sampling.

## Cheat sheet:

- Normal Mode → Transfer once → Stop → CPU restarts DMA
- Circular Mode → Transfer continuously → Auto reload → Used for continuous data flow

# DMA Configuration Steps (Interview Cheat Sheet)
- Enable DMA clock → Enable the clock for the DMA controller.
- Select DMA channel/stream → Map the DMA channel to the required peripheral request.
- Configure transfer direction → Select Peripheral-to-Memory, Memory-to-Peripheral, or Memory-to-Memory.
- Configure source and destination address → Set peripheral register address and memory buffer address.
- Configure data width → Set transfer size (Byte, Half-word, Word).
- Configure address increment mode → Enable/disable increment for peripheral and memory addresses.
- Configure transfer mode → Select Normal Mode or Circular Mode.
- Configure FIFO (optional) → Enable FIFO mode and set FIFO threshold/burst if needed.
- Set transfer length → Define the number of data items to transfer.
- Configure DMA interrupts → Enable Transfer Complete, Half Transfer, or Error interrupts.
- Enable DMA channel/stream → Start the DMA transfer.
- Handle DMA events in ISR → Process data when transfer is completed or an error occurs.
