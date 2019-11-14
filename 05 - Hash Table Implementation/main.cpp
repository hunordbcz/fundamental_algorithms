/**
 *  Author: Hunor Debreczeni
 *  Group: 30423
 *
 *  Assignment:     Implement correctly and efficiently the insert and search operations in a hash table using open
 *                      addressing and quadratic probing
 *
 *       *Hash Table*
 *          Insert
 *              We get the hash value of the given entry and check if the given position in the hash table is occupied or not.
 *              We do this until we successfully insert the entry or until we tried it for hashTable.size times.
 *          Search
 *              We get the hash value of the given entry and check if the given position in the hash table is equal to
 *               the searched entry. We also stop if the current hashIndex is null, because that means, that the searched
 *               entry is surely not present in the hash table. We do this two checks until we either find one of them or
 *               we reach hashTable.size.
 *
 *      *Testing*
 *          Uniform Selects
 *              I shuffle the original array, from which we inserted the elements and take m/2 elements from there to
 *               search for them in the hash table.
 *              For the not found elements I do the same, but I increment each value of the searched element with max_range,
 *               so that I know for surely that those elements will not appear in the hash table.
 */

#include <iostream>
#include <string>
#include <math.h>
#include "Profiler.h"

#define HASH_CONSTANT_ONE 13
#define HASH_CONSTANT_TWO 51
#define HASH_CONSTANT_THREE 83
#define HASH_CONSTANT_FOUR 19
#define HASH_CONSTANT_FIVE 37
#define HASH_SIZE 9973
#define SEARCH_ARRAY_SIZE 3000

using namespace std;

Profiler profiler("Insert and Search operations in a hash table");

typedef struct {
    int id;
    string name;
} Entry;

Entry null = {-1, "null"};

bool equals(Entry a, Entry b) {
    return (a.id == b.id) && (a.name.compare(b.name) == 0);
}

int hashConstant(Entry obj, int n) {
    return (obj.id * HASH_CONSTANT_ONE + HASH_CONSTANT_TWO) % n;
}

int hashLinear(Entry obj, int i, int n) {
    return (hashConstant(obj, n) + i * HASH_CONSTANT_THREE) % n;
}

int hashQuadratic(Entry obj, int i, int n) {
    return (hashLinear(obj, i, n) + HASH_CONSTANT_FOUR * i + HASH_CONSTANT_FIVE * i * i) % n;
}

int insertIntoHash(vector<Entry> &hashTable, Entry obj) {
    int hashValue, i = 0;
    do {
        hashValue = hashQuadratic(obj, i, hashTable.size()) % hashTable.size();
        if (equals(hashTable.at(hashValue), null)) {
            hashTable[hashValue].id = obj.id;
            hashTable[hashValue].name = obj.name;
            return hashValue;
        }
        i++;
    } while (i < hashTable.size());
    return -1;
}

int findInHash(vector<Entry> hashTable, Entry obj, int &effort) {
    int hashValue, i = 0;
    do {
        hashValue = hashQuadratic(obj, i, hashTable.size()) % hashTable.size();
        if (equals(hashTable.at(hashValue), obj)) {
            effort = i + 1;
            return hashValue;
        } else if (equals(hashTable.at(hashValue), null)) {
            effort = i + 1;
            return -1;
        }
        i++;
    } while (i < hashTable.size());
    effort = i + 1;
    return -1;
}

void runTests() {
    int effort, totalEffortFound, maxEffortFound, totalEffortNotFound, maxEffortNotFound, n;
    int range_min = 0;
    int range_max = 100000;
    float loadPercentage[5] = {0.8, 0.85, 0.9, 0.95, 0.99};
    cout << "Filling Factor\tAvg. Effort Found\tMax Effort Found\tAvg. Effort Not Found\tMax Effort Not Found" << endl;
    for (int i = 0; i < 5; i++) {
        cout << loadPercentage[i] << "\t\t";

        totalEffortFound = 0;
        maxEffortFound = -1;
        totalEffortNotFound = 0;
        maxEffortNotFound = -1;
        n = floor(HASH_SIZE * loadPercentage[i]);
        int data[n];
        for (int repeat = 0; repeat < 5; repeat++) {
            vector<Entry> hashTable(HASH_SIZE, null);
            FillRandomArray(data, n, range_min, range_max, true, 0);
            for (int k = 0; k < n; k++) {
                insertIntoHash(hashTable, {data[k], "Name"});
            }
            random_shuffle(data, data + n);
            for (int k = 0; k < SEARCH_ARRAY_SIZE / 2; k++) {
                if (findInHash(hashTable, {data[k], "Name"}, effort) > 0) {
                    totalEffortFound += effort;
                    if (maxEffortFound < effort) {
                        maxEffortFound = effort;
                    }
                }
            }
            for (int k = SEARCH_ARRAY_SIZE / 2; k < SEARCH_ARRAY_SIZE; k++) {
                if (findInHash(hashTable, {data[k] + range_max, "Name"}, effort) == -1) {
                    totalEffortNotFound += effort;
                    if (maxEffortNotFound < effort) {
                        maxEffortNotFound = effort;
                    }
                }
            }
        }
        cout << (float) totalEffortFound / 5 / SEARCH_ARRAY_SIZE << "\t\t\t";
        cout << maxEffortFound << "\t\t\t";
        cout << (float) totalEffortNotFound / 5 / SEARCH_ARRAY_SIZE << "\t\t\t";
        cout << maxEffortNotFound << "\t\t" << endl;
    }
}

void find(vector<Entry> hashTable, Entry obj) {
    int dummy;
    int elementIndex = findInHash(hashTable, obj, dummy);
    if (elementIndex > 0) {
        cout << hashTable[elementIndex].name << " at:" << elementIndex << endl;
    } else {
        cout << obj.name << " not found" << endl;
    }
}

void demo() {
    vector<Entry> hashTable(10, null);  //The HashTable is filled up with null Entry types
    Entry hunor = {2, "Hunor"};
    Entry david = {2, "David"};
    Entry john = {11, "John"};
    Entry tom = {9, "Tom"};
    insertIntoHash(hashTable, hunor);
    insertIntoHash(hashTable, david);
    insertIntoHash(hashTable, john);
    find(hashTable, hunor);
    find(hashTable, david);
    find(hashTable, john);
    find(hashTable, tom);
}

int main() {
    demo();
    runTests();
    return 0;
}

/**
*       Output Example:
*
*            Hunor at:7
*            David at:6
*            John at:4
*            Tom not found
*            Filling Factor  Avg. Effort Found       Max Effort Found        Avg. Effort Not Found   Max Effort Not Found
*            0.8             1.04747                 29                      2.72947                 44
*            0.85            1.15287                 49                      3.60187                 69
*            0.9             1.32027                 49                      5.2572                  84
*            0.95            1.67527                 113                     10.2963                 191
*            0.99            2.5492                  437                     49.5995                 735
*
*/