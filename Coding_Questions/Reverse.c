// Reverse a character array without using library functions

// Solution:
/*
  I use two pointers:
  - One pointer points to the beginning of the array
  - Another pointer points to the end of the string
  - Swap the characters while moving both pointers toward the center
  The algorithm is:
  1. Set start pointer to the first character.
  2. Set end pointer to the last character.
  3. Swap *start and *end
  4. Increment start and decrement end.
  5. Stop when start >= end
*/

#include <stdio.h>

void reverse(char* s) {
    char* start = s;
    char* end = s + strlen(s) - 1;
    
    while(start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        
        start++;
        end--;
    }
}

int main() {
    char str[] = "toanvu";
    reverse(str);
    printf("%s", str);
    return 0;
}
