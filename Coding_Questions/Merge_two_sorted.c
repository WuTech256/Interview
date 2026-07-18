// Merge two sorted arrays without using extra large buffers.

/*
    The idea is to merge the two sorted arrays in-place instead of creating a third temporary array.
    
    Since the destination array has enough capcacity to store all elements, I use three pointers. One pointer starts
    at the end of first array, one pointer starts at the end of the second array, and the third pointer starts at the last
    position of the destination array.
    
    I compare the two largest elements and palce tha larger one at the end of the destination array. Then I
    move the corresponding pointer backward. I continue this process until all elements are merged.
    
    I merge from the end because merging from the beginning could overwrite the existing elements in the first array.
    
    The time complexity is O(n+m) because each element is processed once, and the extra space complexity is O(1) because no
    additional buffer is used.
*/

#include <stdio.h>
#include <stdlib.h>

void merge(int *a, int n, int *b, int m) {
    int i = n - 1;          // cuối phần dữ liệu hợp lệ của a
    int j = m - 1;          // cuối của b
    int k = n + m - 1;      // vị trí ghi cuối cùng trong a

    while (i >= 0 && j >= 0) {
        if (a[i] > b[j])
            a[k--] = a[i--];
        else
            a[k--] = b[j--];
    }

    // Chỉ cần copy phần dư của b
    while (j >= 0) a[k--] = b[j--];
}

int main(void) {
    int n = 4, m = 5;

    // Cấp phát a với đủ chỗ cho cả hai mảng
    int *a = malloc((n + m) * sizeof(int));
    if (!a) {
        perror("malloc failed");
        return 1;
    }

    // Khởi tạo n phần tử đầu của a
    int init_a[] = {1, 3, 5, 7};
    for (int i = 0; i < n; i++) a[i] = init_a[i];

    int b[] = {2, 4, 6, 8, 10};

    merge(a, n, b, m);

    for (int i = 0; i < n + m; i++) printf("%d ", a[i]);
    // Output: 1 2 3 4 5 6 7 8 10
    printf("\n");

    free(a);   // luôn nhớ free!
    return 0;
}
