Đã gộp phần **3-4-5-6-7-8** thành một mục lớn **3. Current Project Overview — LG LUPA/LCP SILS** và chỉ dùng `##` cho các mục con.

---

# 1. Introduction

Hi all, my name is Toan. I'm a Senior Embedded Software Engineer with over 4 years of experience specializing in C/C++, RTOS, and low-level system development.

I hold a degree from the Mechatronics and Robotics Talent Program at Hanoi University of Science and Technology.

Currently, I work at LG Electronics as the Function Owner of SILS, which stands for Software-in-the-Loop Simulation for Home appliance projects.

My core responsibility is simulating our firmware to run on a Linux host, which allows our engineering teams to validate product firmware without needing physical ARM boards.

Prior to LG, I worked at Bosch, where I developed the AUTOSAR Communication module and built rigorous unit tests for automotive projects like Audi.

I'm really excited about our conversation today, and I look forward to sharing more about my technical background and learning about your team's engineering challenges.

---

# 2. Why do you want to seek another opportunity after just 3 years at LG?

I've really enjoyed my three years at LG Electronics and learned a lot, especially in embedded software development, RTOS, and low-level system programming.

However, I feel I've reached a point where I'm looking for new technical challenges, broader responsibilities, and opportunities to work on different products and technologies.

That's why I'm exploring new opportunities.

---

# 3. Current Project Overview — LG LUPA/LCP SILS

So, I currently work at LG Electronics Vietnam, in the software team behind LUPA and LCP, which are embedded platforms used in LG home appliances like washers and dryers.

Just to give some quick context:

* **LUPA (LG Upgradable Platform for Appliances)** is the main/display controller, responsible for UI and application logic, and it runs on FreeRTOS.

* **LCP (Load Control Processor)** is a separate controller that handles low-level hardware control, such as motor, heater, valves, and sensors.

## SILS Project Overview

The main project I work on is called SILS, which stands for Software-In-the-Loop Simulation.

The goal is to run the actual embedded firmware on a Linux PC without requiring real ARM hardware.

We cannot change the original firmware logic, so we only replace the hardware-dependent layer underneath it.

## My Responsibility in SILS

My main responsibility is developing and maintaining this simulation layer.

For the LCP side, I work on the stub layer that replaces MCU peripherals such as:

* NVIC
* GPIO
* Timers
* UART

Instead of accessing real hardware registers, the firmware accesses simulated registers implemented as memory variables on Linux.

This allows the firmware to run without knowing it is not running on the real MCU.

For the LUPA side, applications that normally run on the MCU are built as shared libraries (`.so`).

The SILS layer dynamically loads these applications at runtime using mechanisms like `dlopen()`.

This allows the same application code to run on Linux without modification.

I also work on:

* Mapping FreeRTOS tasks to Linux pthreads
* Maintaining simulated drivers such as display and flash storage

## LUPA and LCP Communication

For LUPA and LCP communication, instead of using a real UART connection, we simulate the communication channel on Linux.

We use linker wrapping to redirect low-level UART functions to Unix sockets, allowing both firmware sides to communicate like they do on real hardware.

## Plant Model

We also have a plant model to simulate the physical system.

For example, when the firmware turns on the heater, the model simulates temperature changes and provides feedback like a real sensor.

## Daily Work / Debugging

A big part of my daily work is debugging differences between SILS and real hardware, such as:

* Missing hardware behavior
* Timing differences between the MCU environment and Linux

I also extend the simulation layer when new hardware features are added, while keeping the original firmware unchanged.

Overall, my role is to build a realistic virtual hardware environment where the same embedded firmware can run consistently on both real hardware and Linux simulation.

---

# 4. Why Use Two Chips (LUPA + LCP)?

Using two separate chips instead of one mainly comes from four reasons.

## 1. Safety

LCP directly controls the motor and heater, so separating it from LUPA prevents UI or application issues from affecting hardware control.

## 2. Different Requirements

They have different requirements:

* LUPA needs more resources for UI, applications, and connectivity.
* LCP focuses on real-time hardware control.

## 3. Cost

LCP only needs a small MCU for its control tasks.

## 4. Independent Development

LUPA and LCP teams can develop separately as long as they follow the communication interface between the two processors.

---

# 5. FreeRTOS GCC POSIX Port

The key difference between FreeRTOS running on real ARM hardware and the GCC POSIX port used in SILS is how task switching is implemented.

## Real ARM Hardware

On real ARM hardware, FreeRTOS runs directly on the MCU without an OS.

Each task has its own stack, and context switching is done by saving and restoring CPU registers through the PendSV exception.

The SysTick interrupt provides the RTOS tick and triggers the scheduler.

## Linux POSIX Port

In the Linux POSIX port, FreeRTOS cannot directly control the CPU stack pointer because Linux manages the hardware.

Therefore, each FreeRTOS task is mapped to a Linux pthread.

The port uses synchronization mechanisms like:

* Mutex
* Condition variables

to simulate the single-core behavior, ensuring only one task is running at a time from the FreeRTOS perspective.

The timer tick is also simulated by a dedicated pthread that periodically triggers the RTOS tick handler instead of using the hardware SysTick interrupt.

## Summary

In summary:

* On ARM, task switching is a real context switch using CPU registers and stack pointers.

* On Linux, FreeRTOS task switching is simulated on top of pthreads to provide a similar execution model.

---

# 6. How to Increase Simulation Speed

In the POSIX port, FreeRTOS tick is simulated using the Linux `ITIMER_REAL` timer.

Normally, the timer interval is configured based on the FreeRTOS tick period, for example 1ms.

When tick acceleration is enabled, we divide this interval by an acceleration ratio, so the timer interrupt happens faster and the simulated system runs faster than real time.

---

# 7. Do you still focus on the hardware layer, like motor control or HAL, in your current project?

You can answer naturally:

> Not directly on the real hardware control logic like motor algorithms or the production HAL.
> My main focus is on the simulation layer underneath the firmware.
>
> For example, I work on simulating the hardware-dependent parts such as MCU peripherals, registers, timers, UART, and drivers, so that the original firmware can run on Linux without modification.
>
> I also need to understand the hardware behavior, because the simulation layer has to provide the same behavior as the real hardware.
>
> For example, when the firmware controls a heater or reads a sensor, the simulation needs to provide realistic feedback through the plant model.
>
> So my role is more on the boundary between embedded firmware and the virtual hardware environment, rather than developing the actual motor control algorithm or hardware driver itself.
