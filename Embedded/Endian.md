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
