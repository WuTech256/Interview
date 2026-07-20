// Count digits in number
#include <stdio.h>

int main() {
    int n;
    scanf("%d", &n);
    
    int cnt = 0;
    do {
        cnt++;
        n /= 10;
    } while(n != 0);
    
    printf("Digits = %d\n", cnt);
    return 0;
}
