# I2C

I2C stands for Inter-Integrated Circuit. It is a synchronous serial communication protocol that is commonly used for communication between a microcontroller and low-speed peripheral devices such as sensors, EEPROMs, and display modules.

I2C uses only two communication lines:

* **SCL (Serial Clock Line)**, which provides the clock signal.
* **SDA (Serial Data Line)**, which carries the data.

Both lines use an open-drain configuration, so they require pull-up resistors to the supply voltage. The devices can only pull the line low, and the pull-up resistor brings it back to high.

I2C follows a master-slave architecture. The master always controls the communication by generating the clock signal and initiating transactions. The slave waits until it receives its address from the master.

A typical I2C transaction starts with a **START** condition, where SDA changes from high to low while SCL is high. Then the master sends the slave address, usually a 7-bit address, followed by the R/W bit to indicate whether it wants to write data or read data.

If the slave address matches, the slave sends an **ACK** signal by pulling SDA low. After that, data bytes are transferred, and each byte is followed by an **ACK** to confirm that the data was received successfully.

When the communication is finished, the master generates a **STOP** condition, where SDA changes from low to high while SCL is high, which releases the bus.

One important feature of I2C is that it supports multiple slaves on the same bus. All slaves share the same SDA and SCL lines, but each slave has a unique address, so the master can select which device it wants to communicate with.

I2C supports different speed modes, such as:

* **Standard mode:** 100 kHz
* **Fast mode:** 400 kHz
* **Fast mode Plus:** 1 MHz
* **High-speed mode:** 3.4 MHz

In embedded systems, I2C is commonly used when we need to connect multiple low-speed peripherals with minimal GPIO usage.

So basically, I2C is a simple two-wire, address-based communication protocol where the master controls the clock and selects different slaves through their addresses.
