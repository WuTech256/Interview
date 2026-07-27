RTOS stands for Real-Time Operating System. It is an operating system designed for embedded systems where tasks need to be executed within a predictable time constraint.

Unlike a general-purpose operating system, the main goal of an RTOS is not high throughput but deterministic response time. It makes sure that important tasks can run on time, especially for time-critical applications like motor control, automotive systems, and industrial devices.

# Can you explain Real-time OS scheduling?

In a Real-Time Operating System, the scheduler is responsible for deciding which task should run on the CPU at any moment. The main goal of the scheduler is not only to maximize CPU usage but also to ensure that time-critical tasks can meet their deadlines.

In most embedded RTOS systems, the scheduler is usually priority-based preemptive scheduling. Each task is assigned a priority. The scheduler always selects the highest-priority ready task to run.

For example, if we have three tasks:

- Motor control task: priority 3
- Communication task: priority 2
- Logging task: priority 1

Normally, the motor control task runs because it has the highest priority. If the communication task is running and a higher-priority motor control task becomes ready, the scheduler will perform a context switch and immediately switch CPU execution to the motor control task.

The scheduler maintains different task states, such as:

- Running: the task currently using the CPU.
- Ready: the task is able to run but waiting for CPU time.
- Blocked: the task is waiting for an event, such as a delay, semaphore, mutex, or queue data.

A context switch happens when the scheduler changes from one task to another. The RTOS saves the current task context, such as CPU registers and stack pointer, into its TCB (Task Control Block), then restores the context of the next task.

In a Cortex-M based system like STM32 or Toshiba TMPM380 running FreeRTOS, the scheduler is usually triggered by system interrupts:

- SysTick interrupt provides the RTOS time base and updates task delays.
- PendSV interrupt performs the actual context switch.
- SVC interrupt is used when starting the first task.

For example, when a task calls vTaskDelay(), the scheduler moves that task from the Ready state to the Blocked state. After the delay time expires, the task is moved back to the Ready state, and the scheduler decides whether it should run based on priority.

So, in summary, an RTOS scheduler manages CPU time among multiple tasks by using priorities, task states, and context switching mechanisms to guarantee that important real-time tasks can execute on time.

# How do you communicate between tasks in an RTOS?

In an RTOS, tasks usually communicate with each other through inter-task communication mechanisms provided by the RTOS. The most common methods are queues, semaphores, mutexes, event flags, and task notifications.

The most common one is a queue. A queue allows one task to send data to another task safely. For example, in an embedded system, an ADC task can periodically read sensor data and send the samples to a processing task through a queue. The receiver task can block on the queue and wake up automatically when new data arrives.

Another mechanism is a semaphore, which is mainly used for synchronization rather than transferring data. For example, an ISR can give a binary semaphore to notify a task that an interrupt event has occurred.

A mutex is used to protect shared resources. For example, if multiple tasks access the same UART or shared memory, a mutex ensures that only one task can access the resource at a time and prevents race conditions.

Event flags are useful when a task needs to wait for one or multiple events. For example, a task can wait until both WiFi initialization and sensor initialization are completed before starting its main operation.

In FreeRTOS, there is also a lightweight mechanism called task notification, which is faster than a semaphore because it directly communicates with a specific task.

So, the choice depends on the purpose:

- Queue → transfer data between tasks.
- Semaphore → signal or synchronize tasks.
- Mutex → protect shared resources.
- Event group → wait for multiple events.
- Task notification → lightweight task signaling.

In my project, for example, tasks can communicate through CMSIS-RTOS v2 APIs, which are wrappers around FreeRTOS mechanisms. A producer task can put data into a message queue, and a consumer task can receive it without directly sharing memory.

## Priority Inversion

Priority inversion happens when a high-priority task is blocked by a lower-priority task that holds a shared resource.

For example, a low-priority task takes a mutex. Then a high-priority task needs the same mutex and becomes blocked. If a medium-priority task keeps running, the low-priority task cannot release the mutex, causing the high-priority task to wait longer than expected.

The common solution is priority inheritance. The low-priority task temporarily inherits the high priority, finishes its critical section, releases the mutex, and then returns to its original priority.

This mechanism helps RTOS maintain predictable response time in real-time systems.
