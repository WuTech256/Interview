// Remove duplicates from sorted array

#include <stdio.h>
int removeDuplicates(int arr[], int n) {
    int j = 0;
    for(int i = 1; i < n; ++i) {
        if(arr[j] != arr[i]) {
            j++;
            arr[j] = arr[i];
        }
    }
    return j + 1;
}

int main() {
    int arr[] = {1, 1, 2, 2, 3, 4, 5};
    int n = sizeof(arr)/sizeof(arr[0]);
    int newSize = removeDuplicates(arr, n);
    
    for(int i = 0; i < newSize; ++i) {
        printf("%d ", arr[i]);
    }
}
