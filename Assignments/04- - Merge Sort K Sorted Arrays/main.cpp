/**
 * Author: Hunor Debreczeni
 * Group: 30423
 *
 *
 *      *Merge K Sorted Arrays*
 *       N   - Length of final Array
 *       K   - Number of Arrays
 *       N/K - Length of initial Arrays
 *
 *          Algorithm:
 *              In the first part, we take the first element from the k arrays and make a min-heap from it, so we can
 *               easily get the smallest value and insert it into our result array.
 *              We fill up the heap with double pairs, so we have 3 informations about a value:
 *                  The value
 *                  The index of the Array in the Vector
 *                  The index of the Value in the Array
 *              They are compared according to the value.
 *              In case we insert one value in the result array, we have to remove it, but to make the program run faster,
 *               we replace it with the next value and run a heapify on it. With this method, we don't have to run heapify
 *               twice, only once.
 *              The next value comes from the same array, but the next smallest one. If there are no more values in the given
 *               array, then we just simply delete it and run heapify on the array.
 *              After each iteration, it's checked if the heap isn't empty, and if it is, then all the elements are already
 *               inside the result array, and also sorted, so the program is done.
 *
 *          Run Time:
 *              Each insert/ delete is done in O(log k) time, due to the heapify algorithm and due to we work with k elements
 *              We have k arrays with n/k length, so that makes k*n/k inserts/ deletes for the heap -> it takes O(n) time
 *              For almost each element we make only one insert and heapify, because it swaps the old minimum with itself,
 *               by that we make our program 2 times faster, otherwise the insertions/ deletes would take O(2n) time.
 *               It equals with O(n), but it's a nice thing to have.
 *              In conclusion we have O(n * log(k))
 *
 *
 *
 */

#include <iostream>
#include <vector>
#include "Profiler.h"

Profiler profiler("QuickSort Advanced Analysis");

using namespace std;

/** .first          -> Value
 *  .second         -> Index of Array in Vector
 *  .second.second  -> Index of Value in Array
 */
typedef pair<int, pair<int, int>> doublePair;

int getParentIndex(int i) {
    return (i - 1) / 2;
}

int getLeftChildIndex(int i) {
    return (2 * i + 1);
}

int getRightChildIndex(int i) {
    return (2 * i + 2);
}

void heapifyDown(vector<doublePair> &data, int i, Operation op) {
    int leftChildIndex = getLeftChildIndex(i);
    int rightChildIndex = getRightChildIndex(i);
    op.count(1);
    if (rightChildIndex < data.size() && (data[rightChildIndex] < data[i]) &&
        (data[rightChildIndex] < data[leftChildIndex])) {
        data[rightChildIndex].swap(data[i]);
        op.count(3);
        heapifyDown(data, rightChildIndex, op);
    } else {
        if (leftChildIndex < data.size() && data[leftChildIndex] < data[i]) {
            op.count(1);
            data[leftChildIndex].swap(data[i]);
            op.count(3);
            heapifyDown(data, leftChildIndex, op);
        } else if (leftChildIndex < data.size()) {
            op.count(1);
        }
    }
}

void pop(vector<doublePair> &data, Operation op) {
//    doublePair pair = data[0];
//    op.count();
    data[0].swap(data[data.size() - 1]);
    op.count(3);
    data.pop_back();
    heapifyDown(data, 0, op);
//    return pair;
}

void heapifyUp(vector<doublePair> &data, int i, Operation op) {
    int parentIndex = getParentIndex(i);
    if (parentIndex != -1 && data[parentIndex] > data[i]) {
        op.count(1);
        data[parentIndex].swap(data[i]);
        op.count(3);
        heapifyUp(data, parentIndex, op);
    } else if (parentIndex == -1) {
        op.count(1);
    }
}

void switchFirst(vector<doublePair> &data, doublePair what, Operation op) {
    data[0].swap(what);
    heapifyDown(data, 0, op);
//    data.push_back(what);
//    heapifyUp(data, data.size() - 1, op);
}

void printVector(vector<int> data) {
    for (int i : data) {
        cout << i << " ";
    }
    cout << endl;
}

vector<int> fillVector(int size, int range_min, int range_max, bool unique, int sorted) {
    int array[size];
    FillRandomArray(array, size, range_min, range_max, unique, sorted);
    vector<int> data(array, array + size);
    return data;
}

vector<doublePair> makeHeap(vector<vector<int>> arrays, int k, Operation op) {
    vector<doublePair> heap;
    heap.reserve(k);
    for (int i = 0; i < k; i++) {
        heap.push_back({arrays[i][0], {i, 0}});
    }
    for (int i = k / 2; i >= 0; i--) {
        heapifyDown(heap, i, op);
    }
    return heap;
}

vector<int> mergeKSortedArrays(vector<vector<int>> arrays, int k, Operation op) {
    vector<int> result;
    vector<doublePair> heap = makeHeap(arrays, k, op);
    op.count();
    while (!heap.empty()) {
        doublePair min = heap.at(0);
        result.push_back(min.first);
        int i = min.second.first;
        int j = min.second.second;
        if (j + 1 < arrays[i].size()) {
            switchFirst(heap, {arrays[i].at(j + 1), {i, j + 1}}, op);
        } else {
            pop(heap, op);
        }
    }
    return result;
}

void exemplifyCorrectness(int n, int k) {
    vector<vector<int>> data;
    for (int i = 0; i < k; i++) {
        vector<int> temp = fillVector(n, 1, 100, false, 1);
        printVector(temp);
        data.push_back(temp);
    }
    Operation dummy = profiler.createOperation("Dummy", 10);
    vector<int> result = mergeKSortedArrays(data, data.size(), dummy);
    cout << "Result:" << endl;
    printVector(result);
}

void generateChart(int n, int k, Operation op) {
    vector<vector<int>> data;
    for (int i = 0; i < k; i++) {
        vector<int> temp = fillVector(n, 1, 50000, false, 1);
        data.push_back(temp);
    }
    vector<int> result = mergeKSortedArrays(data, data.size(), op);
}

void runTests() {
    for (int n = 100; n <= 10000; n += 100) {
        Operation kFive = profiler.createOperation("Five Arrays", n);
        Operation kTen = profiler.createOperation("Ten Arrays", n);
        Operation kHundred = profiler.createOperation("Hundred Arrays", n);
        generateChart(n / 5, 5, kFive);
        generateChart(n / 10, 10, kTen);
        generateChart(n / 100, 100, kHundred);
//        cout << n << endl;
    }
    for (int k = 10; k <= 500; k += 10) {
        Operation nFixed = profiler.createOperation("10.000 Array Size", k);
        generateChart(10000 / k, k, nFixed);
//        cout << k << endl;
    }
    profiler.createGroup("Fixed Array Numbers - Different Array Size", "Five Arrays", "Ten Arrays", "Hundred Arrays");
    profiler.createGroup("Different Array Numbers - Fixed Array Size", "10.000 Array Size");
    profiler.showReport();
}

int main() {
    runTests();
    exemplifyCorrectness(20, 4);
    return 0;
}