# Endian
Endianness is the byte order used to store multi-byte data in memory. In Big-endian, the most significant byte is stored at the lowest memory address, 
while in Little-endian, the least significant byte is stored first. For example, 
the 32-bit value 0x12345678 is stored as 12 34 56 78 in Big-endian and 78 56 34 12 in Little-endian. 
Most modern processors, such as Intel x86 and ARM, use Little-endian by default."

# EG
#include <stdio.h>

int main()
{
    unsigned int x = 1;

    if (*(char *)&x)
        printf("Little Endian\n");
    else
        printf("Big Endian\n");
}

# Volatile
volatile is a type qualifier that tells the compiler that the value of a variable may change unexpectedly outside the normal program flow. 
Therefore, the compiler must not optimize accesses to that variable. Every read and write must be performed directly from memory instead of using a cached value in a CPU register

# 

"For example, if a global flag is updated inside an ISR and the main loop keeps polling it, the compiler may optimize the code by reading the flag only once. As a result, the main loop may never detect the updated value. Declaring the flag as volatile forces the compiler to reload it from memory every time it is accessed."

Example:

volatile int flag = 0;

void UART_IRQHandler(void)
{
    flag = 1;
}

int main(void)
{
    while (flag == 0)
    {
        // Wait for interrupt
    }

    // Continue processing
}
