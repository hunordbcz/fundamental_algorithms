/**
 * Author: Hunor Debreczeni
 * Group: 30423
 *
 *  Assignment Specification:   Implement and Compare 3 Direct Sorting Methods in the Best, Worst and Average cases
 *                              Discussion, interpretations, efficiency, compare, stability
 *
 *  Conclusions:
 *      *Bubble Sort*
 *          Slowest one in Worst and Average Cases -> Not Efficient Enough, but does the job
 *          Fastest one in the Best Case, because it has almost no assignments and arraySize comparisons only
 *          Running time is O(n) for the Best Case, otherwise O(n^2)
 *          Stable, because the comparison uses strict inequality
 *          Algorithm:
 *              Compares each pair and swaps if the order is not okay, otherwise passes on.
 *              With each loop, we get the i+1 biggest elements to the end of the array
 *
 *      *Selection Sort*
 *          Fastest one in the Average and Worst Cases -> Most efficient compared to the others
 *          Slowest one in the Best Case, because even if the array is sorted, it has to check for each element if it
 *           really is the smallest one in the unsorted part of the array
 *          Running time is always O(n^2) due to the comparisons described above
 *          Stable, because the comparison uses strict inequality, so the equal elements won't be swapped
 *          Algorithm:
 *              We start from the left and divide the array to 2 sub-arrays, the sorted one and the unsorted one.
 *              We focus on finding the minimum from the unsorted array and swapping it with the first element from the
 *               unsorted array -> the sorted array grows until the full array becomes a sorted one.
 *
 *      *Insertion Sort*
 *          Tends to be between the two other sorting algorithms or on the same place as the Bubble Sort
 *          It's definitely faster than the Bubble-Sort, but slower than the Selection Sort because it does a lot
 *           more assignments. In the Worst Case it does better than the Selection Sort because it doesn't need to go
 *           to the end of the array to find the minimum, it just compares the i-th element with the elements before it.
 *          Although, in the Best Case it has an O(n) running time, otherwise O(n^2)
 *          Stable algorithm for the same reason as the others
 *          Algorithm:
 *              It's similar to the Selection Sort, but this one extracts the current minimum and shift the elements
 *               before it to the right until the extracted element can be placed in the sorted part of the array, so
 *               that the sub-array stays sorted.
 *
 *      *Final Notes*
 *          Personally I would choose the Insertion Sort, due to the fact, that it does better than the Bubble Sort in
 *           the Worst Case, but also in the Best Case takes a good running time.
 *
 *
 *      Comparing the cases according to the graphs: ( Most operations -> Least operations )
 *
 *          *Worst Case*
 *              Comparisons: Almost the same
 *              Assignments: Bubble > Insertion > Selection
 *              Total:       Bubble > Insertion > Selection
 *
 *          *Best Case*
 *              Comparisons: Selection > ( Insertion = Bubble )
 *              Assignments: Selection > Insertion > ( Bubble ~ 0 )
 *              Total:       Selection > Insertion > Bubble
 *
 *          *Average Case*
 *              Comparisons: ( Bubble ~ Selection ) > Insertion
 *              Assignments: Bubble > Insertion > Selection
 *              Total:       Bubble > Insertion > Selection
 *
 */

#include <iostream>
#include "Profiler.h"

#define MAX_SIZE 10000

Profiler profiler("Direct_Sorting_Method_Comparisons_Best_Case");

using namespace std;


void printArray(int array[], int arraySize) {
    for (int i = 0; i < arraySize; i++) {
        cout << array[i] << " ";
    }
    cout << endl;
}


/** Sorts a given algorithm in increasing order
 *  Method implemented: Bubble Sort
 *
 * @param array - Array of type Int
 * @param arraySize - Size of Array
 */
void bubbleSort(int array[], int arraySize) {
    bool isSorted;
    int i = 0;
    int k = arraySize;
    do {
        profiler.countOperation("Bub_Sort_Assig", arraySize);
        isSorted = true;
        for (i = 0; i < k - 1; i++) {
            profiler.countOperation("Bub_Sort_Comp", arraySize);
            if (array[i] > array[i + 1]) {
                profiler.countOperation("Bub_Sort_Assig", arraySize, 4);
                isSorted = false;
                swap(array[i], array[i + 1]);
            }
        }
        k--;
        profiler.countOperation("Bub_Sort_Assig", arraySize);
    } while (!isSorted);
}

/** Sorts a given algorithm in increasing order
 *  Method implemented: Selection Sort
 *
 * @param array
 * @param arraySize
 */
void selectionSort(int array[], int arraySize) {
    int currentMinimum, i, j;
    for (i = 0; i < arraySize - 1; i++) {
        profiler.countOperation("Sel_Sort_Assig", arraySize);
        currentMinimum = i;
        for (j = i + 1; j < arraySize; j++) {
            profiler.countOperation("Sel_Sort_Comp", arraySize);
            if (array[j] < array[currentMinimum]) {
                profiler.countOperation("Sel_Sort_Assig", arraySize);
                currentMinimum = j;
            }
        }
        profiler.countOperation("Sel_Sort_Assig", arraySize, 3);
        swap(array[i], array[currentMinimum]);
    }
}

/** Sorts a given algorithm in increasing order
 *  Method implemented: Insertion Sort
 *
 * @param array
 * @param arraySize
 */
void insertionSort(int array[], int arraySize) {
    int temp, i, j;
    for (i = 1; i < arraySize; i++) {
        temp = array[i];
        j = i - 1;
        profiler.countOperation("Ins_Sort_Assig", arraySize, 2);
        while (j >= 0 && array[j] > temp) {
            profiler.countOperation("Ins_Sort_Comp", arraySize);
            profiler.countOperation("Ins_Sort_Assig", arraySize, 2);
            array[j + 1] = array[j];
            j--;
        }
        profiler.countOperation("Ins_Sort_Comp", arraySize);
        profiler.countOperation("Ins_Sort_Assig", arraySize);
        array[j + 1] = temp;
    }
}

/** Copies an array to another
 *
 * @param origin        Original array which contains data
 * @param destination   The array which will be filled with the data from origin
 * @param size          Size of the origin array
 */
void copyArray(int origin[], int destination[], int size) {
    memcpy(destination, origin, size * sizeof(int));
}

void runTests(int a[], int b[], int c[], int n, int sorted) {
    insertionSort(a, n);
    bubbleSort(b, n);
    selectionSort(c, n);
    profiler.addSeries("Selection_Sort", "Sel_Sort_Assig", "Sel_Sort_Comp");
    profiler.addSeries("Insertion_Sort", "Ins_Sort_Assig", "Ins_Sort_Comp");
    profiler.addSeries("Bubble_Sort", "Bub_Sort_Assig", "Bub_Sort_Comp");
    if (sorted == 1) {
        profiler.createGroup("Assignments_Best", "Sel_Sort_Assig", "Ins_Sort_Assig", "Bub_Sort_Assig");
        profiler.createGroup("Comparisons_Best", "Sel_Sort_Comp", "Ins_Sort_Comp", "Bub_Sort_Comp");
        profiler.createGroup("Best_Case_Total", "Selection_Sort", "Insertion_Sort", "Bubble_Sort");
    } else if (sorted == 2) {
        profiler.createGroup("Assignments_Worst", "Sel_Sort_Assig", "Ins_Sort_Assig", "Bub_Sort_Assig");
        profiler.createGroup("Comparisons_Worst", "Sel_Sort_Comp", "Ins_Sort_Comp", "Bub_Sort_Comp");
        profiler.createGroup("Worst_Case_Total", "Selection_Sort", "Insertion_Sort", "Bubble_Sort");
    } else {
        profiler.createGroup("Assignments_Avg", "Sel_Sort_Assig", "Ins_Sort_Assig", "Bub_Sort_Assig");
        profiler.createGroup("Comparisons_Avg", "Sel_Sort_Comp", "Ins_Sort_Comp", "Bub_Sort_Comp");
        profiler.createGroup("Avg_Case_Total", "Selection_Sort", "Insertion_Sort", "Bubble_Sort");
    }


}

void bestCase(int n) {
    int mainArray[MAX_SIZE];
    FillRandomArray(mainArray, n, 100, 10000, false, 1);
    int a[MAX_SIZE];
    int b[MAX_SIZE];
    int c[MAX_SIZE];
    copyArray(mainArray, a, MAX_SIZE);
    copyArray(mainArray, b, MAX_SIZE);
    copyArray(mainArray, c, MAX_SIZE);
    runTests(a, b, c, n, 1);
};

void worstCase(int n) {
    int mainArray[MAX_SIZE];
    FillRandomArray(mainArray, n, 100, 10000, false, 2);
    int a[MAX_SIZE];
    int b[MAX_SIZE];
    int c[MAX_SIZE];
    copyArray(mainArray, a, MAX_SIZE);
    copyArray(mainArray, b, MAX_SIZE);
    copyArray(mainArray, c, MAX_SIZE);
    runTests(a, b, c, n, 2);
};

void averageCase(int n) {
    int mainArray[MAX_SIZE];
    FillRandomArray(mainArray, n, 100, 10000, false, 0);
    int a[MAX_SIZE];
    int b[MAX_SIZE];
    int c[MAX_SIZE];
    copyArray(mainArray, a, MAX_SIZE);
    copyArray(mainArray, b, MAX_SIZE);
    copyArray(mainArray, c, MAX_SIZE);
    runTests(a, b, c, n, 0);
};

void exemplifyCorrectness(int n) {
    int testArray[MAX_SIZE];
    FillRandomArray(testArray, n, 1, 100, false, 0);
    int a[MAX_SIZE];
    int b[MAX_SIZE];
    int c[MAX_SIZE];
    copyArray(testArray, a, MAX_SIZE);
    copyArray(testArray, b, MAX_SIZE);
    copyArray(testArray, c, MAX_SIZE);

    cout << "Insertion Sort:" << endl;
    printArray(a, n);
    insertionSort(a, n);
    printArray(a, n);

    cout << endl << "Bubble Sort:" << endl;
    printArray(b, n);
    bubbleSort(b, n);
    printArray(b, n);

    cout << endl << "Selection Sort:" << endl;
    printArray(c, n);
    selectionSort(c, n);
    printArray(c, n);

}

int main() {
    exemplifyCorrectness(20);
    for (int i = 100; i <= 10000; i += 500) {
        bestCase(i);
//        cout << i << endl;    // Used to see the progress of report rendering
    }
    profiler.showReport();
    profiler.reset("Direct_Sorting_Method_Comparisons_Worst_Case");
    for (int i = 100; i <= 10000; i += 500) {
        worstCase(i);
//        cout << i << endl;
    }
    profiler.showReport();
    profiler.reset("Direct_Sorting_Method_Comparisons_Average_Case");
    for (int i = 100; i <= 10000; i += 500) {
        for (int j = 0; j < 5; j++) {
            averageCase(i);
        }
//        cout << i << endl;
    }
    profiler.divideValues("Sel_Sort_Assig", 5);
    profiler.divideValues("Ins_Sort_Assig", 5);
    profiler.divideValues("Bub_Sort_Assig", 5);
    profiler.divideValues("Sel_Sort_Comp", 5);
    profiler.divideValues("Ins_Sort_Comp", 5);
    profiler.divideValues("Bub_Sort_Comp", 5);
    profiler.divideValues("Selection_Sort", 5);
    profiler.divideValues("Insertion_Sort", 5);
    profiler.divideValues("Bubble_Sort", 5);
    profiler.showReport();
    return 0;
}