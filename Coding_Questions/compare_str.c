// Compare two strings without strcmp()
#include <stdio.h>


int stringCompare(char str1[], char str2[])
{

    int i = 0;


    while(str1[i]!='\0' || str2[i]!='\0')
    {

        if(str1[i] != str2[i])
        {
            return 0;
        }


        i++;
    }


    return 1;
}



int main()
{
    char a[]="Embedded";
    char b[]="Embedded";


    if(stringCompare(a,b))
        printf("Same string\n");
    else
        printf("Different\n");


    return 0;
}
