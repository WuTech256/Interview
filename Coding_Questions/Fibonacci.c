// Print Fibonacci series (Interative, recursive)

// #include <stdio.h>

// int main() {
//     int n;
//     printf("Input n: ");
//     scanf("%d", &n);
    
//     int a = 0, b = 1, next;
    
//     for(int i = 0; i < n; i++) {
//         printf("%d ", a);
//         next = a + b;
//         a = b;
//         b = next;
//     }
    
//     return 0;
// }

#include <stdio.h>

int fib(int n) {
    if(n <= 1)
        return n;
    return fib(n-1) + fib(n-2);
}

int main() {
    int n;
    scanf("%d", &n);
    
    for(int i = 0; i < n; i++) {
        printf("%d ", fib(i));
    }
}
