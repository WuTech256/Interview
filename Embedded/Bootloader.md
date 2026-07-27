# Bootloader

A bootloader is a small firmware that runs before the main application. It is responsible for system initialization, firmware update, and validating the application image.

After MCU reset, the Cortex-M core loads the initial stack pointer and reset handler address from the vector table, then starts executing the bootloader.

The bootloader checks whether an update is needed. If not, it jumps to the application.

To switch to the application, the bootloader disables interrupts, updates the VTOR register to point to the application's vector table, loads the application's stack pointer, and jumps to the application's Reset_Handler.

After that, the application starts running normally.


Why do we need to change VTOR before jumping to application?
→ Because interrupts after jumping must use the application's interrupt vector table, not the bootloader's.
Why must we set MSP before jumping?
→ Because each firmware has its own stack region and the application expects the correct initial stack pointer.
What happens if bootloader does not disable interrupts?
→ A pending interrupt may execute the bootloader ISR after jumping to the application, causing system crash.
