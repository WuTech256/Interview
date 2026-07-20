// Rotate array left/right by N positions
#include <stdio.h>

void reverse(int arr[], int left, int right) {
    while(left < right)
    {
        int temp = arr[left];
        arr[left] = arr[right];
        arr[right] =  temp;
        
        left++;
        right--;
    }
}

void rotateRight(int arr[], int n, int k) {
    k %= n;
    reverse(arr, 0, n - 1);
    reverse(arr, 0, k - 1);
    reverse(arr, k, n - 1);
}
int main() {
    int n, k;
    scanf("%d %d", &n, &k);
    
    int arr[n];
    for(int i = 0; i < n; ++i) scanf("%d", &arr[i]);
    
    rotateRight(arr, n, k);
    
    for(int i = 0; i < n; ++i)
        printf("%d ", arr[i]);

    return 0;
}
