# UART

**UART stands for Universal Asynchronous Receiver-Transmitter. It is a hardware communication protocol used for serial communication between two devices.**

The main idea of UART is that it transfers data **one bit at a time** through two signals:

* **TX (Transmit)**: sends data
* **RX (Receive)**: receives data

UART is called **asynchronous** because it does not use a shared clock signal between the transmitter and receiver. Instead, both sides must be configured with the same communication parameters, especially the **baud rate**.

For example, if both devices use **115200 baud**, they can correctly sample the incoming bits.

A UART frame usually contains:

1. **Start bit** – indicates the beginning of a frame, usually logic 0.
2. **Data bits** – normally 8 bits, transmitted LSB first.
3. **Optional parity bit** – used for simple error detection.
4. **Stop bit(s)** – indicates the end of the frame, usually logic 1.

For example, with an **8N1 configuration**:

* 8 data bits
* No parity
* 1 stop bit

One byte of data requires:

```
1 start bit + 8 data bits + 1 stop bit = 10 bits
```

So at **115200 baud**, the theoretical throughput is around:

```
115200 / 10 ≈ 11520 bytes per second
```

---

**At the hardware level**, UART has a transmitter and receiver module.

When sending data:

1. CPU writes data into the UART data register.
2. UART hardware converts the byte into a serial bit stream.
3. The TX pin shifts out bits according to the baud rate.

When receiving:

1. UART monitors the RX line.
2. It detects the start bit.
3. It samples each data bit at the correct timing.
4. It reconstructs the byte and stores it in the receive register.

The CPU can handle receiving data in two ways:

* **Polling method**:
  CPU continuously checks the UART status register to see whether data is available.

* **Interrupt method**:
  UART generates an interrupt when data arrives, and the CPU executes an ISR to process the received data.

For high-speed communication, UART can also use **DMA**, where UART transfers data directly to memory without CPU handling every byte.

---

**Regarding baud rate generation**, the UART peripheral usually has a clock source from the MCU. The baud rate register is configured based on this clock.

For example, if MCU clock is 16 MHz and we need 115200 baud, the UART calculates a divider value and stores it into the baud rate register.

---

**Compared with SPI and I2C:**

* UART:

  * Asynchronous
  * Usually point-to-point communication
  * Uses TX/RX only
  * Simple but lower speed

* SPI:

  * Synchronous
  * Requires clock line
  * Faster, suitable for sensors and memories

* I2C:

  * Synchronous
  * Uses two wires (SDA/SCL)
  * Supports multiple devices with addresses

---

**In embedded systems, UART is commonly used for debugging logs, bootloader communication, firmware update interfaces, and communication between MCUs or modules such as GPS, Bluetooth, and Wi-Fi modules.**

---

### Short senior embedded interview version:

> "UART is an asynchronous serial communication protocol. It uses two lines, TX and RX, and does not require a clock signal. Both devices must agree on parameters like baud rate, data bits, parity, and stop bits. The UART hardware handles serialization and deserialization of data. Data can be handled by polling, interrupt, or DMA depending on the performance requirement. In embedded systems, UART is widely used for debugging, bootloaders, and communication with external modules."
