# I2C

I2C stands for **Inter-Integrated Circuit**. It is a synchronous serial communication protocol commonly used for communication between a microcontroller and peripheral devices such as sensors, EEPROMs, and display modules.

I2C uses only two communication lines:

- **SCL (Serial Clock Line):** Provides the clock signal.
- **SDA (Serial Data Line):** Transfers data.

Both lines use an **open-drain configuration**, so they require pull-up resistors to the supply voltage. Devices can only pull the line low, while the pull-up resistor brings the line back to high.

I2C follows a **master-slave architecture**. The master controls the communication by generating the clock signal and initiating transactions. The slave waits until it receives its address from the master.

A typical I2C transaction starts with a **START condition**, where SDA changes from high to low while SCL is high. Then the master sends the slave address (usually 7-bit address) followed by the **R/W bit** to indicate write or read operation.

If the address matches, the slave sends an **ACK (Acknowledge)** signal by pulling SDA low.

During data transfer, each byte is followed by an **ACK/NACK bit**:

- In a **write operation**, the slave sends ACK after receiving data from the master.
- In a **read operation**, the master sends ACK after receiving data to request more bytes.
- The master sends **NACK** after receiving the final byte to indicate that it wants to stop receiving data.

When communication is finished, the master generates a **STOP condition**, where SDA changes from low to high while SCL is high, releasing the bus.

One important feature of I2C is that it supports multiple slaves on the same bus. All slaves share the same SDA and SCL lines, but each slave has a unique address, allowing the master to select a specific device.

I2C supports different speed modes:

- **Standard mode:** 100 kHz
- **Fast mode:** 400 kHz
- **Fast mode Plus:** 1 MHz
- **High-speed mode:** 3.4 MHz

In embedded systems, I2C is commonly used when connecting multiple low-speed peripherals while minimizing GPIO usage.

Basically, I2C is a simple two-wire, address-based communication protocol where the master controls the clock and selects different slaves through their addresses.


# How does I2C master transmit data to multiple slaves?

In I2C, the master can communicate with multiple slaves because every slave device has a unique address.

The master does not send data to all slaves simultaneously. Instead, it selects a specific slave by sending the slave address at the beginning of each transaction.

The communication process:

1. The master generates a START condition.
2. The master sends the slave address + R/W bit.
3. Only the slave with the matching address responds with an ACK signal.
4. The master sends or receives data with that specific slave.
5. The master generates a STOP condition to release the bus.


# I2C Master Register-Level Cheat Sheet

A concise reference guide for configuring and operating the I2C peripheral in Master Mode at register level (STM32 example).


## 1. Clock Initialization

Enable clock for both GPIO port (SDA/SCL) and I2C peripheral.

```c
RCC->AHB1ENR |= GPIO_CLK;  // Enable GPIO Clock
RCC->APB1ENR |= I2C_CLK;   // Enable I2C Clock
```

## 2. GPIO Configuration (SDA & SCL)
Configure SDA and SCL pins:

- **Mode:** Alternate Function
- **Output Type:** Open-Drain
- **Pull:** Pull-up enabled

```c
GPIO->MODER  |= AF_MODE;     // Alternate Function mode
GPIO->OTYPER |= OPEN_DRAIN;  // Open-drain output
GPIO->PUPDR  |= PULL_UP;     // Enable pull-up
GPIO->AFR    |= I2C_AF;      // Select I2C alternate function
```

## 3. Clock & Speed Configuration
Main registers:

- **CR2:** Peripheral clock frequency
- **CCR:** Generate SCL clock
- **TRISE:** Maximum rise time

Example: Standard Mode 100 kHz with PCLK = 16 MHz

```c
I2C->CR2   = 16;  // Peripheral clock = 16 MHz
I2C->CCR   = 80;  // Generate 100 kHz SCL
I2C->TRISE = 17;  // Rise time configuration
```

## 4. Enable Peripheral
Enable I2C by setting the PE bit in CR1.

```c
I2C->CR1 |= I2C_CR1_PE;
```

---

## Master Transmit Flow

```text
START
  |
Slave Address + WRITE
  |
Slave ACK
  |
Data
  |
Slave ACK
  |
STOP
```

### 1. Generate START Condition
Set START bit in CR1.

```c
I2C->CR1 |= I2C_CR1_START;
```

Wait until:
```c
SR1.SB == 1
```
*(SB means Start Bit generated successfully.)*

### 2. Send Slave Address (Write Mode)
Send 7-bit slave address with WRITE bit.

```c
I2C->DR = (SLAVE_ADDR << 1) | 0;
```

Example: Address = 0x50
`1010000` + `WRITE(0)` = `10100000`

Wait until:
```c
SR1.ADDR == 1
```
*(The slave responds with ACK.)*

### 3. Send Data
Write data to Data Register.

```c
I2C->DR = data;
```

Wait until:
```c
SR1.TXE == 1
```
*(TXE means transmit buffer is empty.)*

### 4. Generate STOP Condition
```c
I2C->CR1 |= I2C_CR1_STOP;
```

---

## Master Receive Flow

```text
START
  |
Slave Address + READ
  |
Slave ACK
  |
Receive Data
  |
Master ACK (continue)
  |
Master NACK (last byte)
  |
STOP
```

### 1. Generate START Condition
```c
I2C->CR1 |= I2C_CR1_START;
```

Wait until:
```c
SR1.SB == 1
```

### 2. Send Slave Address (Read Mode)
Send slave address with READ bit.

```c
I2C->DR = (SLAVE_ADDR << 1) | 1;
```

Example: Address = 0x48
`1001000` + `READ(1)` = `10010001`

Wait until:
```c
SR1.ADDR == 1
```
*(Slave responds with ACK.)*

### 3. Enable ACK for Multiple Bytes
If the master wants to receive more data:

```c
I2C->CR1 |= I2C_CR1_ACK;
```

Flow:
```text
Slave sends Byte 1
        |
Master sends ACK
        |
Slave sends Byte 2
        |
Master sends ACK
```

### 4. Read Data
Wait until:
```c
SR1.RXNE == 1
```

Then read Data Register:
```c
data = I2C->DR;
```

### 5. Send NACK for Last Byte
Before receiving the final byte, disable ACK:

```c
I2C->CR1 &= ~I2C_CR1_ACK;
```

After the last byte:
```text
Slave sends last byte
        |
Master sends NACK
        |
Slave stops transmitting
```
*(NACK means: "I received the last byte and do not need more data.")*

### 6. Generate STOP Condition
```c
I2C->CR1 |= I2C_CR1_STOP;
```
