// Find length of string without strlen()
#include <stdio.h>

int stringLength(char* str)
{
    int len = 0;
    
    while(*str != '\0')
    {
        len++;
        str++;
    }
    
    return len;
}

int main() {
    char str[] = "Hello toan.vu";
    
    int len = stringLength(str);
    
    printf("Length = %d\n", len);
    return 0;
}
