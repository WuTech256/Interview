# Bootloader Overview and Execution Flow

## What is a bootloader?

A bootloader is a small firmware program that runs before the main application firmware.

Its main purpose is to initialize the MCU, check the application firmware, and decide whether to start the application or stay in bootloader mode for firmware update.

For example, in a product that supports OTA (Over-The-Air) or UART firmware update, the bootloader can receive a new firmware image, verify it, and program it into Flash without requiring a debugger.

---

## Boot flow after power-on or reset

When the MCU is powered on or reset, it does not directly execute the application.
The CPU starts execution from the reset vector address, which normally points to the bootloader.

The bootloader first performs basic initialization, such as configuring the clock, initializing communication interfaces, and checking the application image.
Then it decides:

* If the application is valid, the bootloader jumps to the application.
* If the application is invalid or an update request exists, the bootloader stays in bootloader mode and waits for a firmware update.

---

## How does the bootloader jump to the application?

Before jumping to the application, the bootloader needs to prepare the MCU environment. The main steps are:

### 1. Disable interrupts
The bootloader disables interrupts to prevent any interrupt from the bootloader affecting the application.

### 2. Reset peripheral states
The bootloader de-initializes peripherals such as UART, timers, and GPIO because the application should start from a clean state.

### 3. Relocate the vector table
Each application has its own interrupt vector table. The bootloader changes the Vector Table Offset Register (VTOR) to point to the application's vector table address.

For example:
```c
SCB->VTOR = APP_ADDRESS;
```
After this, all interrupts will use the application's interrupt handlers.

### 4. Update the stack pointer
The first value in the application's vector table is the initial stack pointer. The bootloader loads this value into the MSP register.

Example:
```c
__set_MSP(*(uint32_t*)APP_ADDRESS);
```

### 5. Jump to the reset handler
The second value in the application's vector table is the reset handler address. The bootloader gets this address and calls it.

Example:
```c
app_reset_handler = *(uint32_t*)(APP_ADDRESS + 4);
app_reset_handler();
```
After this step, the application starts running as if the MCU has just been reset.

---

## Short interview answer

> A bootloader is a small firmware that runs before the application. After reset, the MCU starts from the bootloader because the reset vector points to the bootloader address.
> 
> The bootloader initializes basic hardware, checks whether the application firmware is valid, and decides whether to update firmware or jump to the application.
> 
> To jump to the application, the bootloader disables interrupts, de-initializes peripherals, updates the vector table using VTOR, loads the application's stack pointer from the vector table, and finally jumps to the application's reset handler.
> 
> After that, the application takes full control of the MCU.

# MCU Boot Sequence - Interview Speaking Answer

## What happens when MCU is powered on?

When the MCU is powered on or reset, it does not directly execute the `main()` function.

The startup behavior depends on whether the system has a bootloader or not.

---

## Case 1: MCU without Bootloader

If there is no bootloader, the MCU starts directly from the application firmware.

After reset, the Cortex-M processor reads the vector table located at the beginning of the application Flash address.

The first entry of the vector table contains the initial Main Stack Pointer (MSP) value, and the second entry contains the Reset Handler address.

The CPU first loads the MSP, then loads the Reset Handler address into the Program Counter and starts executing the startup code.

The startup code runs before `main()` and prepares the runtime environment.

First, it usually calls `SystemInit()` to configure basic hardware, such as the system clock.

Then, it initializes memory sections.

For the `.data` section, initialized global and static variables are stored in Flash, so the startup code copies their initial values from Flash to RAM.

For the `.bss` section, which contains uninitialized global and static variables, the startup code clears this memory area and initializes all values to zero.

After that, it initializes the C runtime environment, such as library initialization or C++ constructors if required.

Finally, it calls `main()`, and the application starts running.

The flow is:

```text
Power On / Reset
    |
    v
Application Vector Table
    |
    v
Load MSP
    |
    v
Jump to Reset_Handler
    |
    v
System Initialization
    |
    v
Initialize .data and .bss
    |
    v
C Runtime Initialization
    |
    v
main()
```





# What is Dual Bank Update?

Dual bank update is a firmware update mechanism that uses two separate Flash regions.

One bank stores the current running firmware, and the other bank is used to store the new firmware image.

During an update, the bootloader downloads the new firmware into the inactive bank instead of overwriting the current firmware.

After the download is completed, the bootloader verifies the firmware integrity using methods like CRC, checksum, or digital signature.

If the firmware is valid, the bootloader switches the boot address, and the MCU starts running the new firmware after reset.

The main advantage of dual bank update is that the old firmware is still available. If something goes wrong during the update, the device can recover instead of becoming unusable.

---

# What is Rollback?

Rollback is the mechanism to return to the previous working firmware when the new firmware fails.

For example, after updating to a new firmware version, if the system cannot boot correctly, crashes, or fails a health check, the bootloader can detect the failure and switch back to the old firmware.

Usually, the system uses a firmware status flag or boot confirmation mechanism.

For example, after the new firmware boots successfully, it sends a confirmation signal or updates a status flag.

If the MCU resets before the confirmation, the bootloader assumes the new firmware is not stable and performs rollback.

---

# What is Watchdog?

A watchdog timer is a hardware timer used to detect software failures.

The application needs to periodically refresh or feed the watchdog.

If the software runs normally, it keeps refreshing the watchdog, so the MCU continues running.

However, if the software gets stuck, crashes, or enters an infinite loop, it cannot refresh the watchdog.

When the watchdog timer expires, it automatically resets the MCU.

After reset, the bootloader can check the reset reason and decide whether the firmware is healthy or needs rollback.
