// Check Palindrome Number

#include <stdio.h>

int main() {
    int n;
    scanf("%d", &n);
    
    int res = 0;
    int temp = n;
    while(n > 0) {
        res = res * 10 + (n % 10);
        n /= 10;
    }
    
    if(res == temp) {
        printf("Palindrome\n");
    } else {
        printf("Not Palindrome\n");
    }
    
    return 0;
}
