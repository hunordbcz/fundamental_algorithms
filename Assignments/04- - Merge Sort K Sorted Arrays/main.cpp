#include <iostream>
#include <vector>

#include "Profiler.h"

Profiler profiler("QuickSort Advanced Analysis");

using namespace std;

typedef pair<int, pair<int, int>> triplePair;

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

void minHeapifyBottom(vector<triplePair> &data, int i, int n, Operation op) {
    int leftChildIndex = getLeftChildIndex(i);
    int rightChildIndex = getRightChildIndex(i);
    op.count(2);
    if (verifyIndex(rightChildIndex, n) && (data[rightChildIndex].first < data[i].first) &&
        (data[rightChildIndex].first < data[leftChildIndex].first)) {
        data[rightChildIndex].swap(data[i]);
//        swap(data[rightChildIndex], data[i]);
        op.count(3);
        minHeapifyBottom(data, rightChildIndex, n, op);
    } else {
        op.count(1);
        if (verifyIndex(leftChildIndex, n) && data[leftChildIndex].first < data[i].first) {
//            swap(data[leftChildIndex], data[i]);
            data[leftChildIndex].swap(data[i]);
            op.count(3);
            minHeapifyBottom(data, leftChildIndex, n, op);
        }
    }
}

triplePair popFront(vector<triplePair> &data, Operation op) {
    triplePair pair = data[0];
    data[0].swap(data[data.size()-1]);
//    swap(data[0], data[data.size() - 1]);
    op.count(3);
    data.pop_back();
    minHeapifyBottom(data, 0, data.size() - 1, op);
    return pair;
}

void minHeapifyTop(vector<triplePair> &data, int i, int n, Operation op) {
    int parentIndex = getParentIndex(i);
    if(parentIndex != -1 && data[parentIndex].first < data[i].first){
        data[parentIndex].swap(data[i]);
        minHeapifyTop(data,parentIndex,n,op);
    }
}


void push(vector<triplePair> &data, triplePair what, Operation op) {
    data.push_back(what);
    op.count();
    minHeapifyTop(data,0,data.size()-1,op);
//    int i = data.size() - 1;
//    int parentIndex = getParentIndex(i);
//    op.count();
//    while (parentIndex != -1 && data[parentIndex].first > data[i].first) {
//        op.count(5);
//        swap(data[parentIndex], data[i]);
//        i = parentIndex;
//        parentIndex = getParentIndex(i);
//    }
//    if (parentIndex != -1) {
//        op.count();
//    }
}

void printArray(int array[], int n) {
    for (int i = 0; i < n; i++) {
        cout << array[i] << " ";
    }
    cout << endl;
}

vector<int> fillVector(int size, int range_min, int range_max, bool unique, int sorted) {
    int array[size];
    FillRandomArray(array, size, range_min, range_max, unique, sorted);
    vector<int> data(array, array + size);
    return data;
}

vector<triplePair> makeHeap(vector<vector<int>> arrays, int k, Operation op) {
    vector<triplePair> heap;
    for (int i = 0; i < k; i++) {
        heap.push_back({arrays[i][0], {i, 0}});
    }
    for (int i = k / 2; i >= 0; i--) {
        minHeapifyBottom(heap, i, k, op);
    }
    return heap;
}


vector<int> mergeKSortedArrays(vector<vector<int>> arrays, int k, Operation op) {
    vector<int> result;
    vector<triplePair> heap = makeHeap(arrays, k, op);
    for (auto &i : heap) {
        cout << i.first << " ";
    }
    cout << endl;
    while (!heap.empty()) {

        triplePair smallest = popFront(heap, op);
        result.push_back(smallest.first);
        int i = smallest.second.first;
        int j = smallest.second.second;

        if (j + 1 < arrays[i].size()){
            push(heap, {arrays[i][j + 1], {i, j + 1}}, op);
        }

        for (auto &i : heap) {
            cout << i.first << " ";
        }
        cout << endl;
    }

    return result;
}

int main() {
//    vector<int> data = fillVector(10, 1, 10, true, 2);
//    swap(data[0], data[1]);
    vector<vector<int>> data{{2,  6,  12},
                             {1,  9},
                             {10, 34, 90, 2000},
                             {9,  34, 90, 2000},
                             {8,  34, 90, 2000},
                             {7,  34, 90, 2000},
    };
    Operation dummy = profiler.createOperation("Dummy", 10);
    vector<int> result = mergeKSortedArrays(data, data.size(), dummy);
    for (auto i = result.begin(); i != result.end(); ++i) {
        cout << *i << " ";
    }
    return 0;
}