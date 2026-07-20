// Swap (with and without temp variable)

// #include <stdio.h>

// int main() {
//     int a = 10;
//     int b = 20;
    
//     int temp = a;
//     a = b;
//     b = temp;
    
//     printf("a = %d b = %d\n", a, b);
//     return 0;
// }

#include <stdio.h>

int main() {
    int a = 10;
    int b = 20;
    
    a = a + b;
    b = a - b;
    a = a - b;
    
    printf("a = %d b = %d\n", a, b);
    return 0;
}
