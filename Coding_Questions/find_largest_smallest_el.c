// Find Largest and Smallest Element
#include <stdio.h>

void findMinMax(int a[], int n, int* min, int* max) 
{
    *min = a[0];
    *max = a[0];
    
    for(int i = 0; i < n; ++i) {
        if(a[i] > *max) {
            *max = a[i];
        } else {
            *min = a[i];
        }
    }
}

int main() {
    int a[] = {4, 5, 1, 2, 6, 7};
    
    int n = sizeof(a)/sizeof(a[0]);
    
    int min, max;
    
    findMinMax(a, n, &min, &max);
    
    printf("Min = %d\n", min);
    printf("Max = %d\n", max);
    return 0;
}
