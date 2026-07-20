// Copy string without strcpy()
#include <stdio.h>


void stringCopy(char dest[], char src[])
{

    int i = 0;


    while(src[i]!='\0')
    {
        dest[i] = src[i];

        i++;
    }


    dest[i]='\0';
}



int main()
{

    char source[]="Microcontroller";

    char destination[50];


    stringCopy(destination, source);


    printf("Source      : %s\n",source);

    printf("Destination : %s\n",destination);



    return 0;
}
