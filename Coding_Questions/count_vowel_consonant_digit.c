// Count vowels, consonants, digits

#include <stdio.h>


void countCharacters(char str[],
                     int *vowels,
                     int *consonants,
                     int *digits)
{

    *vowels = 0;
    *consonants = 0;
    *digits = 0;


    int i = 0;


    while(str[i] != '\0')
    {

        char c = str[i];


        if(c >= '0' && c <= '9')
        {
            (*digits)++;
        }


        else if(c=='a'||c=='e'||c=='i'||c=='o'||c=='u'||
                c=='A'||c=='E'||c=='I'||c=='O'||c=='U')
        {
            (*vowels)++;
        }


        else if((c>='a'&&c<='z')||
                (c>='A'&&c<='Z'))
        {
            (*consonants)++;
        }


        i++;
    }
}



int main()
{
    char str[]="Hello123";


    int vowels;
    int consonants;
    int digits;


    countCharacters(str,
                    &vowels,
                    &consonants,
                    &digits);



    printf("Vowels = %d\n", vowels);
    printf("Consonants = %d\n", consonants);
    printf("Digits = %d\n", digits);


    return 0;
}
