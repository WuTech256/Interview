// Reverse number
#include <stdio.h>

int main() {
    int n;
    scanf("%d", &n);
    int res = 0;
    while(n > 0) {
        res = res * 10 + (n % 10);
        n /= 10;
    }
    
    printf("%d", res);
    return 0;
}
