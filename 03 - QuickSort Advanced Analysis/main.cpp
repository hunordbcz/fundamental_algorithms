/**
 *  Author: Hunor Debreczeni
 *  Group: 30423 - TUCN - CS
 *
 *      Assignment Specification:   Implement correctly and efficiently Quicksort and Quick­Select
 *                                  Analyze the complexity of Quicksort and Heapsort comparatively
 *                                  Compare the two sorting procedures in the average case
 *                                  Evaluate Quicksort in the best and worst cases
 *                                  For QuickSelect only the correctness needs to be demonstrated on sample inputs
 *
 *      *Quick-Sort*
 *          Algorithm:
 *              It takes smaller parts of the array at the start and compares it to the last element of the sub-array,
 *               if it is smaller than that element, the algorithm puts it to the left part of it, otherwise to the right.
 *              By performing this method repeatedly, the array will eventually become sorted.
 *          Run Time:
 *              In the BEST case, it should take as the pivot element ( to which we compare the other elements while
 *               partitioning ) the middle one from the sub-array, so that there would be no swaps in a sorted array.
 *              Thus it runs recursively n times and each time on sqrt(n) size parts, it has an O(n*log n) complexity.
 *
 *              In the WORST case, when it's ordered in any way, and the pivot is the smallest/greatest element, it has
 *               to do for every element a swap, so each element get's swapped many times.
 *              It's complexity is already O(n^2), which is the worst one compared to almost every other sort method.
 *
 *              In the AVERAGE case, it's almost the same as the best case, it does a little bit more swaps, but not too
 *               much, so we can consider the complexity O(n*log n).
 *          Comparison between cases:
 *              While the Quick-Sort method is the fastest one for unsorted arrays, the worst case takes a lot of time to
 *               be done. In addition the best case is also hard to be implemented, my conclusion is that it should be only
 *               used when we have an unsorted array ( In the average case ).
 *
 *      *Heap-Sort and Quick-Sort comparison in the average case*
 *          According to charts:
 *              In the average case, we can see, that the Quick-Sort algorithm does the job faster with less
 *               comparisons + assignments than the Heap-Sort algorithm. We can say that the Quick-Sort algorithm is better
 *               for unsorted arrays.
 *          Complexity:
 *              Although the complexity of each algorithm in the average case is the same ( O(n*log n) ), there's a slight
 *               performance difference between the two.
 *          Conclusion:
 *              Use Quick-Sort if we don't much about our array, otherwise consider other algorithms.
 */

#include <iostream>
#include <random>
#include "Profiler.h"

#define MAX_SIZE 10000

using namespace std;

Profiler profiler("QuickSort Advanced Analysis");
random_device rd;
mt19937 generator(rd());

int partition(int array[], int low, int high, int pivotIndex, Operation op) {
    if (pivotIndex != high) {
        swap(array[pivotIndex], array[high]);
        op.count(3);
    }
    int pivot = array[high];
    op.count();
    int i = low - 1;
    for (int j = low; j < high; j++) {
        op.count();
        if (array[j] < pivot) {
            i++;
            op.count(3);
            swap(array[i], array[j]);
        }
    }
    op.count(3);
    swap(array[i + 1], array[high]);
    return i + 1;
}

void quickSort(int array[], int low, int high, Operation op) {
    op.count();
    if (low >= high) {
        return;
    }
    int pivot = partition(array, low, high, high, op);
    quickSort(array, low, pivot - 1, op);
    quickSort(array, pivot + 1, high, op);
}

void quickSortBestCase(int array[], int low, int high, Operation op) {
    op.count();
    if (low >= high) {
        return;
    }
    int pivotIndex = (low + high) / 2;
    int pivot = partition(array, low, high, pivotIndex, op);
    quickSort(array, low, pivot - 1, op);
    quickSort(array, pivot + 1, high, op);
}

void quickSortRandom(int array[], int low, int high, Operation op) {
    op.count();
    if (low >= high) {
        return;
    }
    uniform_int_distribution<int> distribution(low, high);
    int pivotIndex = distribution(generator);
    int pivot = partition(array, low, high, pivotIndex, op);
    quickSort(array, low, pivot - 1, op);
    quickSort(array, pivot + 1, high, op);
}

int quickSelectFind(int array[], int low, int high, int k, Operation op) {
    if (low == high) {
        return array[low];
    }
    uniform_int_distribution<int> distribution(low, high);
    int pivotIndex = distribution(generator);
    pivotIndex = partition(array, low, high, pivotIndex, op);
    if (k == pivotIndex) {
        return array[k];
    } else if (k < pivotIndex) {
        return quickSelectFind(array, low, pivotIndex - 1, k, op);
    } else {
        return quickSelectFind(array, pivotIndex + 1, high, k, op);
    }
}

int quickSelect(int array[], int low, int high, int k, Operation op) {
    return quickSelectFind(array, low, high, k - 1, op);
}

bool verifyIndex(int i, int n) {
    if (i < 0) return false;
    return i < n;
}

int getLeftChildIndex(int i) {
    int key = i << 1;
    return key + 1;
}

int getRightChildIndex(int i) {
    int key = i << 1;
    return key + 2;
}

void maxHeapifyBottom(int *a, int i, int n, Operation op) {
    int leftChildIndex = getLeftChildIndex(i);
    int rightChildIndex = getRightChildIndex(i);
    op.count(2);
    if (verifyIndex(rightChildIndex, n) && (a[rightChildIndex] > a[i]) && (a[rightChildIndex] > a[leftChildIndex])) {
        swap(a[rightChildIndex], a[i]);
        op.count(3);
        maxHeapifyBottom(a, rightChildIndex, n, op);
    } else {
        op.count(1);
        if (verifyIndex(leftChildIndex, n) && a[leftChildIndex] > a[i]) {
            swap(a[leftChildIndex], a[i]);
            op.count(3);
            maxHeapifyBottom(a, leftChildIndex, n, op);
        }
    }
}

void buildMaxHeapBottomUp(int a[], int n, Operation op) {
    for (int i = n / 2; i >= 0; i--) {
        maxHeapifyBottom(a, i, n, op);
    }
}

void heapSort(int a[], int n, Operation op) {
    buildMaxHeapBottomUp(a, n, op);
    for (int i = n - 1; i > 0; i--) {
        op.count(3);
        swap(a[0], a[i]);
        n--;
        maxHeapifyBottom(a, 0, n, op);
    }
}

void printArray(int array[], int n) {
    for (int i = 0; i < n; i++) {
        cout << array[i] << " ";
    }
    cout << endl;
}

void worstCase(int n) {
    int testArray[MAX_SIZE];
    FillRandomArray(testArray, n, 1, 10000, false, 2);
    int a[MAX_SIZE];
    int b[MAX_SIZE];
    CopyArray(a, testArray, MAX_SIZE);
    CopyArray(b, testArray, MAX_SIZE);
    Operation worstCaseQuickSort = profiler.createOperation("Worst Quick Sort", n);
    Operation worstCaseHeapSort = profiler.createOperation("Worst Heap Sort", n);
    quickSort(a, 0, n - 1, worstCaseQuickSort);
    heapSort(b, n, worstCaseHeapSort);
}

void bestCase(int n) {
    int testArray[MAX_SIZE];
    FillRandomArray(testArray, n, 1, 10000, false, 1);
    int a[MAX_SIZE];
    int b[MAX_SIZE];
    CopyArray(a, testArray, MAX_SIZE);
    CopyArray(b, testArray, MAX_SIZE);
    Operation bestCaseQuickSort = profiler.createOperation("Best Quick Sort", n);
    Operation bestCaseHeapSort = profiler.createOperation("Best Heap Sort", n);
    quickSortBestCase(testArray, 0, n - 1, bestCaseQuickSort);
    heapSort(b, n, bestCaseHeapSort);

}

void averageCase(int n) {
    int testArray[MAX_SIZE];
    FillRandomArray(testArray, n, 1, 10000, false, 0);
    int a[MAX_SIZE];
    int b[MAX_SIZE];
    CopyArray(a, testArray, MAX_SIZE);
    CopyArray(b, testArray, MAX_SIZE);
    Operation averageCaseQuickSort = profiler.createOperation("Average Quick Sort", n);
    Operation averageCaseHeapSort = profiler.createOperation("Average Heap Sort", n);
    quickSortRandom(testArray, 0, n - 1, averageCaseQuickSort);
    heapSort(b, n, averageCaseHeapSort);
}

void runTests() {
    for (int i = 100; i <= 10000; i += 100) {
        worstCase(i);
        bestCase(i);
        for (int j = 0; j < 5; j++) {
            averageCase(i);
        }
    }
    profiler.divideValues("Average Quick Sort", 5);
    profiler.divideValues("Average Heap Sort", 5);
    profiler.createGroup("Average Case", "Average Quick Sort", "Average Heap Sort");
    profiler.createGroup("Best Case", "Best Quick Sort", "Best Heap Sort");
    profiler.createGroup("Worst Case", "Worst Quick Sort", "Worst Heap Sort");
    profiler.showReport();
}

void exemplifyCorrectness(int n) {
    int testArray[MAX_SIZE];
    FillRandomArray(testArray, n, 1, 10, true, 2);
    Operation dummy = profiler.createOperation("Dummy", 0);
    cout<<"Quick-Sort:"<<endl;
    printArray(testArray, n);
    quickSortBestCase(testArray, 0, n - 1, dummy);
    printArray(testArray, n);
    cout<<"Quick-Select (5th smallest element):"<<endl;
    cout << quickSelect(testArray, 0, n - 1, 5, dummy);
}

int main() {
    runTests();
    exemplifyCorrectness(10);
    return 0;
}