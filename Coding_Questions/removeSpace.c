// remove Space
#include <stdio.h>

void removeSpaces(char *str)
{
    char *read = str;
    char *write = str;

    while (*read != '\0')
    {
        if (*read != ' ')
        {
            *write = *read;
            write++;
        }

        read++;
    }

    *write = '\0';
}


int main()
{
    char str[] = "Hello World Embedded";

    removeSpaces(str);

    printf("%s\n", str);

    return 0;
}
