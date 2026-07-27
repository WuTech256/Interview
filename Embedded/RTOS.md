# Can you explain Real-time OS scheduling?

In a Real-Time Operating System, the scheduler is responsible for deciding which task should run on the CPU at any moment. The main goal of the scheduler is not only to maximize CPU usage but also to ensure that time-critical tasks can meet their deadlines.

In most embedded RTOS systems, the scheduler is usually priority-based preemptive scheduling. Each task is assigned a priority. The scheduler always selects the highest-priority ready task to run.

For example, if we have three tasks:

Motor control task: priority 3
Communication task: priority 2
Logging task: priority 1

Normally, the motor control task runs because it has the highest priority. If the communication task is running and a higher-priority motor control task becomes ready, the scheduler will perform a context switch and immediately switch CPU execution to the motor control task.

The scheduler maintains different task states, such as:

Running: the task currently using the CPU.
Ready: the task is able to run but waiting for CPU time.
Blocked: the task is waiting for an event, such as a delay, semaphore, mutex, or queue data.

A context switch happens when the scheduler changes from one task to another. The RTOS saves the current task context, such as CPU registers and stack pointer, into its TCB (Task Control Block), then restores the context of the next task.

In a Cortex-M based system like STM32 or Toshiba TMPM380 running FreeRTOS, the scheduler is usually triggered by system interrupts:

SysTick interrupt provides the RTOS time base and updates task delays.
PendSV interrupt performs the actual context switch.
SVC interrupt is used when starting the first task.

For example, when a task calls vTaskDelay(), the scheduler moves that task from the Ready state to the Blocked state. After the delay time expires, the task is moved back to the Ready state, and the scheduler decides whether it should run based on priority.

So, in summary, an RTOS scheduler manages CPU time among multiple tasks by using priorities, task states, and context switching mechanisms to guarantee that important real-time tasks can execute on time.
