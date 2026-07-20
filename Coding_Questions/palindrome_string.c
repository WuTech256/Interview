// Check palindrome string
#include <stdio.h>

int isPalindrome(char* str) {
    char* l = str;
    char* r = str + strlen(str) - 1;
    
    while(l < r) {
        if(*l != *r) {
            return 0;
        }
        l++;
        r--;
    }
    return 1;
}

int main()
{
    char str[] = "level";
    if(isPalindrome(str))
        printf("Palindrome\n");
    else
        printf("Not palindrome\n");


    return 0;
}
