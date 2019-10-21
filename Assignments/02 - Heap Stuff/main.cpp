/**
 * Author: Hunor Debreczeni
 * Group: 30423
 *
 *  Assignment Specification:   Implement correctly and efficiently two methods for building a heap,
 *                               namely the bottom­up and the top­down strategies.
 *                              Additionally, you have to implement heapsort.
 *                                  Implement and exemplify correctness
 *                                  Comparative analysis of the two build heap methods, in the average case
 *                                  Interpretations, advantages/disadvantages of each approach
 *
 *  Conclusions:
 *
 *      
 */

#include <iostream>
#include "Profiler.h"

#define MAX_HEAP_SIZE 10000

Profiler profiler("Heap Approaches");

using namespace std;

bool verifyIndex(int i, int n) {
    if (i < 0) return false;
    return i < n;
}

int getParentIndex(int i) {
    if (i == 0) return -1;
    if (i % 2 == 1) {
        return i >> 1;
    }
    return --i >> 1;
}

int getLeftChildIndex(int i) {
    int key = i << 1;
    return key + 1;
}

int getRightChildIndex(int i) {
    int key = i << 1;
    return key + 2;
}

void printArray(int array[], int n) {
    for (int i = 0; i < n; i++) {
        cout << i << " \t";
    }
    cout << endl;
    for (int i = 0; i < n; i++) {
        cout << array[i] << " \t";
    }
    cout << endl;
}

void maxHeapifyBottom(int *a, int i, int n, Operation op) {
    int leftChildIndex = getLeftChildIndex(i);
    int rightChildIndex = getRightChildIndex(i);
    op.count(2);
    if (verifyIndex(rightChildIndex, n) && (a[rightChildIndex] > a[i]) && (a[rightChildIndex] > a[leftChildIndex])) {
        op.count(1);
        swap(a[rightChildIndex], a[i]);
        op.count(3);
        maxHeapifyBottom(a, rightChildIndex, n, op);
    } else if (verifyIndex(leftChildIndex, n) && a[leftChildIndex] > a[i]) {
        op.count(1);
        swap(a[leftChildIndex], a[i]);
        op.count(3);
        maxHeapifyBottom(a, leftChildIndex, n, op);
    }
    //op.count(2);
}

void buildMaxHeapBottomUp(int a[], int n, Operation op) {
    for (int i = n / 2; i >= 0; i--) {
        maxHeapifyBottom(a, i, n, op);
    }
}

void heapSort(int a[], int n, Operation op) {
    buildMaxHeapBottomUp(a, n, op);
    for (int i = n - 1; i > 0; i--) {
        swap(a[0], a[i]);
        n--;
        maxHeapifyBottom(a, 0, n, op);
    }
}

void maxHeapifyTop(int *array, int i, Operation op) {
    int parentIndex = getParentIndex(i);
    op.count();
    while (parentIndex != -1 && array[parentIndex] < array[i]) {
        op.count(5);
        swap(array[parentIndex], array[i]);
        i = parentIndex;
        parentIndex = getParentIndex(i);
        op.count(2);
    }
    if(parentIndex != -1){
        op.count();
    }
}

void buildMaxHeapTopDown(int source[], int n, Operation op) {
    int result[n];
    for (int i = 0; i < n; i++) {
        result[i] = source[i];
        op.count();
        maxHeapifyTop(result, i, op);
    }
    for (int i = 0; i < n; i++) {
        source[i] = result[i];
    }
}

void exemplifyCorrectness(int n) {
    int testArray[MAX_HEAP_SIZE];
    FillRandomArray(testArray, n, 1, 10, true, 1);
    int a[MAX_HEAP_SIZE];
    int b[MAX_HEAP_SIZE];
    int c[MAX_HEAP_SIZE];
    CopyArray(a, testArray, MAX_HEAP_SIZE);
    CopyArray(b, testArray, MAX_HEAP_SIZE);
    CopyArray(c, testArray, MAX_HEAP_SIZE);

    Operation dummy = profiler.createOperation("Dummy", 0);

    cout << "Bottom-Up Heap Creation:" << endl;
    printArray(a, n);
    buildMaxHeapBottomUp(a, n, dummy);
    printArray(a, n);

    cout << endl << "Heap Sort:" << endl;
    printArray(a, n);
    heapSort(a, n, dummy);
    printArray(a, n);

    cout << endl << "Top-Down Heap Creation:" << endl;
    printArray(b, n);
    buildMaxHeapTopDown(b, n, dummy);
    printArray(b, n);
}

void worstCase(int n) {
    int testArray[MAX_HEAP_SIZE];
    FillRandomArray(testArray, n, 1, 100, false, 1);
    int a[MAX_HEAP_SIZE];
    int b[MAX_HEAP_SIZE];
    CopyArray(a, testArray, MAX_HEAP_SIZE);
    CopyArray(b, testArray, MAX_HEAP_SIZE);
    Operation worstCaseBottomUp = profiler.createOperation("Worst Case Bottom Up", n);
    Operation worstCaseTopDown = profiler.createOperation("Worst Case Top Down", n);
    buildMaxHeapBottomUp(a, n, worstCaseBottomUp);
    buildMaxHeapTopDown(b, n, worstCaseTopDown);
}

void averageCase(int n) {
    int testArray[MAX_HEAP_SIZE];
    int a[MAX_HEAP_SIZE];
    int b[MAX_HEAP_SIZE];
    for (int i = 0; i < 5; i++) {
        FillRandomArray(testArray, n, 1, 100, false, 0);
        CopyArray(a, testArray, MAX_HEAP_SIZE);
        CopyArray(b, testArray, MAX_HEAP_SIZE);
        Operation averageCaseBottomUp = profiler.createOperation("Average Case Bottom Up", n);
        Operation averageCaseTopDown = profiler.createOperation("Average Case Top Down", n);
        buildMaxHeapBottomUp(a, n, averageCaseBottomUp);
        buildMaxHeapTopDown(b, n, averageCaseTopDown);
    }
}

void bestCase(int n){
    int testArray[MAX_HEAP_SIZE];
    FillRandomArray(testArray, n, 1, 100, false, 2);
    int a[MAX_HEAP_SIZE];
    int b[MAX_HEAP_SIZE];
    CopyArray(a, testArray, MAX_HEAP_SIZE);
    CopyArray(b, testArray, MAX_HEAP_SIZE);
    Operation bestCaseBottomUp = profiler.createOperation("Best Case Bottom Up", n);
    Operation bestCaseTopDown = profiler.createOperation("Best Case Top Down", n);
    buildMaxHeapBottomUp(a, n, bestCaseBottomUp);
    buildMaxHeapTopDown(b, n, bestCaseTopDown);
}

void runTests() {
    for (int i = 100; i <= 10000; i += 100) {
        worstCase(i);
        averageCase(i);
        bestCase(i);
    }
    profiler.divideValues("Average Case Bottom Up", 5);
    profiler.divideValues("Average Case Top Down", 5);
    profiler.createGroup("Average Case", "Average Case Bottom Up", "Average Case Top Down");
    profiler.createGroup("Worst Case", "Worst Case Bottom Up", "Worst Case Top Down");
    profiler.createGroup("Best Case", "Best Case Bottom Up", "Best Case Top Down");
    profiler.showReport();
}

int main() {
    runTests();
    exemplifyCorrectness(10);
    return 0;
}