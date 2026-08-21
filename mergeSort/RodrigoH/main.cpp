/*
 * Title: main.cpp
 * 
 * Description: 
 * Merge Sort algorithm implementation for any kind of vector. 
 * The program asks for a number of elements to sort, afterwards
 * the elements, one per line.
 * Once given the inputs, the program returns the array sorted in
 * descending order.
 * 
 * Implementation for the subject - Analysis and Design of Advanced
 * Algorithms
 * 
 * Author: Rodrigo Alejandro Hurtado Cortes - A01713854
 * Date: August 19, 2026
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>

#include "mergeSort.h"

using namespace std;

/*
printSortedArray()
Auxiliar function that prints the given vector as a
comma-separated list with 2 decimal places.

ordered: vector by reference which content is printed.
return: none.
*/
void printSortedArray(const vector<double> &ordered) {
	cout << "Sorted array:" << endl;
	for (size_t i = 0; i < ordered.size(); i++) {
		cout << fixed << setprecision(2) << ordered[i];
		if (i != ordered.size() - 1) {
			cout << ", ";
		}
	}
	cout << endl;
}

/*
sortNumbers()
Applies the mergeSort algorithm to the given vector.

numbers: vector with the elements to be sorted.
return: a new vector with the elements of numbers sorted
in descending order.
*/
vector<double> sortNumbers(const vector<double> &numbers) {
	return mergeSort(numbers);
}

/*
readNumbersFromFile()
Opens the given file, ignores comment lines ("//") and blank
lines, reads the element count followed by that many values,
and returns them as a vector<double>. Returns an empty vector
if the file could not be opened.

filename: string with the testCase filename and relative direction.
return: a double vector which contains the given elements by the testCase
or an empty vector in case this could not be opened.
*/
vector<double> readNumbersFromFile(const string &filename) {
	vector<double> numbers = {};

	ifstream file(filename);
	if (!file.is_open()) {
		cout << "The file could not open: " << filename << endl;
		return numbers;
	}

	int elements = 0;
	string line = "";

	while (getline(file, line)) {
		bool isComment = (line.find("//") != string::npos);
		bool isEmpty = line.empty();

		if (!isComment && !isEmpty) {
			stringstream ss(line);

			if (elements == 0) {
				ss >> elements; // first useful line = element count
			} else {
				double value = 0.0;
				ss >> value;
				numbers.push_back(value);
			}
		}
	}

	return numbers;
}

/*
readNumbersFromInput()
Manual collection of the number of elements and elements
to use the mergeSort algorithm.

return: double vector or empty vector depending on the number
of elements given by the user.
*/
vector<double> readNumbersFromInput() {
	vector<double> numbers = {};
	int elements = 0;
	double element = 0.0;

	cout << "Enter number of elements to sort:" << endl;
	cin >> elements;
	cin.ignore();

	if (elements < 1) {
		cout << "No elements to sort" << endl;
		return numbers;
	}

	cout << "Enter the elements, one per line:" << endl;
	for (int i = 0; i < elements; i++) {
		cin >> element;
		cin.ignore();
		numbers.push_back(element);
	}

	return numbers;
}

/*
printMenu()
Prints the option menu.

return: none.
*/
void printMenu() {
	cout << "MergeSort Implementation: " << endl;
	cout << "" << endl;
	cout << "Enter one of the following options:" << endl;
	cout << "1. Run all test cases." << endl;
	cout << "2. Run manually. Insert number and elements." << endl;
	cout << "3. Run one file in specific." << endl;
	cout << "4. Exit." << endl;
	cout << "Election: " << endl;
}

/*
main()
Main function responsible for the menu loop, dispatching to
the shared read/sort/print helpers for each option.

return 0
*/
int main() {

	vector<string> testFiles = {
		"testCase0.txt", "testCase1.txt", "testCase100.txt",
		"testCase1000.txt", "testCase10000.txt"
	};

	bool finished = false;

	while (!finished) {
		string option = "";
		printMenu();
		cin >> option;
		int opt = stoi(option);

		if (opt == 1) {
			for (const string &filename : testFiles) {
				vector<double> numbers = readNumbersFromFile(filename);
				cout << "Test case: " << filename << endl;
				if (!numbers.empty()) {
					vector<double> ordered = sortNumbers(numbers);
					printSortedArray(ordered);
				}
			}
		}
		else if (opt == 2) {
			vector<double> numbers = readNumbersFromInput();
			if (!numbers.empty()) {
				vector<double> ordered = sortNumbers(numbers);
				printSortedArray(ordered);
			}
		}
		else if (opt == 3) {
			cout << "Available files:" << endl;
			for (size_t i = 0; i < testFiles.size(); i++) {
				cout << i + 1 << ". " << testFiles[i] << endl;
			}
			cout << "Select a file number: " << endl;

			string fileOption = "";
			cin >> fileOption;
			int fileIdx = stoi(fileOption) - 1;

			if (fileIdx < 0 || fileIdx >= (int)testFiles.size()) {
				cout << "Invalid selection." << endl;
			} else {
				vector<double> numbers = readNumbersFromFile(testFiles[fileIdx]);
				if (!numbers.empty()) {
					vector<double> ordered = sortNumbers(numbers);
					printSortedArray(ordered);
				}
			}
		}
		else if (opt == 4) {
			finished = true;
		}
		else {
			cout << "Invalid option." << endl;
		}
	}

	return 0;
}