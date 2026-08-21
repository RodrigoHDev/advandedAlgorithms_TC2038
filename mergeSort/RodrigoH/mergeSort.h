/* 
 * Title: mergeSort.h
 * 
 * Description: 
 * Merge Sort algorithm declaration for any kind of vector in
 * descending order. This implementation is composed of 4 functions.
 * Functions are not enclosed within a class due to the lonely
 * implementation of this algorithm in the main.cpp file.
 * 
 * Implementation for the subject - Analysis and Design of Advanced
 * Algorithms
 * 
 * Author: Rodrigo Alejandro Hurtado Cortes - A01713854
 * Date: August 19, 2026
 */

#include <iomanip>
#include <vector>

using namespace std;

template <class T>
vector<T> mergeSort(const vector<T> &source);
template <class T>
void mergeSplit(vector<T> &original, vector<T> &copy, int low, int high);
template <class T>
void copyArray(vector<T> &original, vector<T> &copy, int low, int high);
template <class T>
void mergeArray(vector<T> &original, vector<T> &copy, int low, int mid, int high);


/*
mergeSort(const vector<T> &source)
Function responsible for starting the MergeSort implementation.
Complexity O(n log n)

source: original vector to be sorted, it is not modified.

return: a new vector with the elements of source sorted in descending order.
*/ 
template <class T>
vector<T> mergeSort(const vector<T> &source) {
	vector<T> ordered(source);
	vector<T> temporal(ordered.size());
	mergeSplit(ordered, temporal, 0, ordered.size() - 1);
	return ordered;
}

/*
mergeSplit(vector<T> &original, vector<T> &copy, int low, int high)
Recursively divides the given original vector references until there 
are is only one element on it. After recursive call in both halves, 
uses mergeArray to sort the elements and copy results in the auxiliar 
vector copy. 
Complexity O(n log n)

original: vector to be sorted.
copy: auxiliar vector to store correct order.
low: lower limit of the segment to sort.
high: upper limit of the segment to sort.

return: none.
*/
template <class T>
void mergeSplit(vector<T> &original, vector<T> &copy, int low, int high) {
	if ((high - low) < 1)
		return;

	int mid = (high + low) / 2;

	mergeSplit(original, copy, low, mid);
	mergeSplit(original, copy, mid + 1, high);
	mergeArray(original, copy, low, mid, high);
	copyArray(original, copy, low, high);
}

/*
copyArray(vector<T> &original, vector<T> &copy, int low, int high)
Copy the elements from vector copy to original in the given lower
and upper limits.
O(n) complexity.

original: vector to obtain the copy of results.
copy: auxiliar vector with sorted elements.
low: lower limit to copy.
high: upper limit to copy.

return: none.
*/
template <class T>
void copyArray(vector<T> &original, vector<T> &copy, int low, int high) {
	for (int i = low; i <= high; i++) {
		original[i] = copy[i];
	}
}

/*
mergeArray(vector<T> &original, vector<T> &copy, int low, int mid, int high)
Combine two sorted segments of a vector into a single sorted section.
By maintaining an overall counter per elements and counters per each sorted
section, the control over the passed and remaining elements and their comparison
guarantees the overall sort of the elements.
O(n) complexity.

original: vector with sorted segments.
copy: auxiliar vector to store overall sort.
low: lower limit of the first segment.
mid: upper limit of the first segment.
high: upper limit of the second segment.

return: none.
*/

template <class T>
void mergeArray(vector<T> &original, vector<T> &copy, int low, int mid, int high) {
	int i = low;
	int j = mid + 1;
	int k = low;

	while (i <= mid && j <= high) {
		if (original[i] > original[j]) {
			copy[k] = original[i];
			i++;
		}
		else {
			copy[k] = original[j];
			j++;
		}
		k++;
	}

	if (i > mid) {
		for (; j <= high; j++) {
			copy[k++] = original[j];
		}
	}
	else {
		for (; i <= mid; i++) {
			copy[k++] = original[i];
		}
	}
}
