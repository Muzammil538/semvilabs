#include <stdio.h>
#include <stdlib.h>

void merge(int arr[], int low, int mid, int high) {
    int i = low, j = mid + 1, k = 0;  // k starts from 0 for temp array
    int temp[high - low + 1];
    
    while (i <= mid && j <= high) {
        if (arr[i] < arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }
    
    while (i <= mid) {
        temp[k++] = arr[i++];
    }
    while (j <= high) {
        temp[k++] = arr[j++];
    }
    
    // Copy back to original array
    for (i = low, k = 0; i <= high; i++, k++) {
        arr[i] = temp[k];
    }
}

void mergesort(int arr[], int low, int high) {
    if (low < high) {
        int mid = low + (high - low) / 2;
        mergesort(arr, low, mid);
        mergesort(arr, mid + 1, high);
        merge(arr, low, mid, high);
    }
}

int main() {
    int arr[] = {5, 10, 8, 2, 1, 7};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    printf("Original array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    
    mergesort(arr, 0, n - 1);
    
    printf("\nSorted array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    return 0;
}