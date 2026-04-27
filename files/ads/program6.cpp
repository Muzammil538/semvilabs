#include <stdio.h>
#include <stdlib.h>

// Function to find minimum of two numbers
int min(int a, int b) {
    return (a < b) ? a : b;
}

// Fibonacci search function
int fibonacciSearch(int arr[], int n, int key) {
    // Initialize Fibonacci numbers
    int fib2 = 0;  // (m-2)th Fibonacci number
    int fib1 = 1;  // (m-1)th Fibonacci number
    int fib = fib2 + fib1;  // mth Fibonacci number
    
    // Find the smallest Fibonacci number greater than or equal to n
    while (fib < n) {
        fib2 = fib1;
        fib1 = fib;
        fib = fib2 + fib1;
    }
    
    // Marks the eliminated range from front
    int offset = -1;
    
    // While there are elements to be checked
    while (fib > 1) {
        // Check if fib2 is a valid index
        int i = min(offset + fib2, n - 1);
        
        // If key is greater than the value at index i, cut the subarray after i
        if (arr[i] < key) {
            fib = fib1;
            fib1 = fib2;
            fib2 = fib - fib1;
            offset = i;
        }
        // If key is less than the value at index i, cut the subarray before i
        else if (arr[i] > key) {
            fib = fib2;
            fib1 = fib1 - fib2;
            fib2 = fib - fib1;
        }
        // Key found
        else {
            return i;
        }
    }
    
    // Check the last element
    if (fib1 == 1 && arr[offset + 1] == key) {
        return offset + 1;
    }
    
    // Key not found
    return -1;
}

// Function to sort the array (Fibonacci search requires sorted array)
void sortArray(int arr[], int n) {
    int i, j, temp;
    
    // Simple bubble sort (students can use any sort)
    for(i = 0; i < n - 1; i++) {
        for(j = 0; j < n - i - 1; j++) {
            if(arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Function to print array
void printArray(int arr[], int n) {
    for(int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int *arr, n, key, result, i;
    
    printf("========== FIBONACCI SEARCH ==========\n\n");
    
    // Input array size
    printf("Enter number of elements: ");
    scanf("%d", &n);
    
    // Allocate memory
    arr = (int*)malloc(n * sizeof(int));
    
    // Input array elements
    printf("Enter %d elements: ", n);
    for(i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    
    // Sort the array (Fibonacci search requires sorted array)
    printf("\nOriginal array: ");
    printArray(arr, n);
    
    sortArray(arr, n);
    printf("Sorted array: ");
    printArray(arr, n);
    
    // Input key to search
    printf("\nEnter element to search: ");
    scanf("%d", &key);
    
    // Perform Fibonacci search
    result = fibonacciSearch(arr, n, key);
    
    // Display result
    if(result != -1) {
        printf("\n✓ Element %d found at index %d\n", key, result);
    } else {
        printf("\n✗ Element %d not found in the array\n", key);
    }
    
    free(arr);
    return 0;
}