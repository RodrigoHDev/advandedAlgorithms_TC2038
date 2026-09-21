/**
 * Title: main.cpp
 *
 * Description:
 * Main program that runs the three analyses of the situation problem over the
 * fixed transmission and malicious code files:
 * - Part 1: searches every mcode inside every transmission (Subchain).
 * - Part 2: finds the longest palindrome of every transmission (Palindrome).
 * - Part 3: finds the longest common substring between the two transmissions
 *   (Substring).
 * The files are read automatically from their respective folders and line
 * breaks are ignored so each file is treated as a single string.
 *
 * Implementation for the subject - Analysis and Design of Advanced Algorithms
 *
 * Author: Alexis Yaocalli Berthou Haas - A01713458 & Rodrigo Alejandro Hurtado Cortes - A01713854
 * Date: Septiembre 3, 2026
 */

#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include "subchain.h"
#include "substring.h"
#include "palindrome.h"

using namespace std;

/*
In order to analyze complexity:
t = number of transmission files.
m = number of malicious code files.
T = length of a transmission file (characters, without line breaks).
M = length of a malicious code file (characters, without line breaks).
L = length of the file being read.
S = cost of Substring::search() over the two transmissions.
*/

/**
 * getFile()
 * Reads a file and returns its content as a single string, ignoring the line
 * breaks '\r' and '\n'.
 *
 * Complexity:
 *  Time: O(L) where L is the number of characters in the file.
 *  Space: O(L)
 *
 * Params:
 * path is the relative path of the file to read.
 * Returns:
 * a string with the content of the file without line breaks. It returns an
 * empty string if the file cannot be opened.
 */
string getFile(const string &path) {
	ifstream file(path);
	if (!file.is_open()) {
		return "";
	}

	string fileContent = "";
	char currentChar = ' ';
	while (file.get(currentChar)) {
		if (currentChar != '\r' && currentChar != '\n') {
			fileContent += currentChar;
		}
	}
	file.close();
	return fileContent;
}

/**
 * main()
 * Reads the fixed transmission and malicious code files, then displays the
 * result of each part: the mcode search for every transmission and mcode
 * pair, the longest palindrome of every transmission and the longest common
 * substring between the two transmissions.
 *
 * Complexity:
 *  Time: O(t * m * (T + M) + t * T + S) since Subchain::search() is called
 *        once per pair, Palindrome::palindrome() once per transmission and
 *        Substring::search() once.
 *  Space: O(T + M) (dominated by the chains built inside Subchain::search()
 *         and Palindrome::palindrome(), plus the space used by Substring).
 *
 * Params: none
 * Returns: 0 when the program finishes.
 */
int main() {
	vector<string> transmissionPaths = {"transmission/transmission1.txt", "transmission/transmission2.txt"};
	vector<string> mcodePaths = {"mcode/mcode1.txt", "mcode/mcode2.txt", "mcode/mcode3.txt"};

	vector<string> transmissions;
	vector<string> mcodes;

	for (const string &path : transmissionPaths) {
		transmissions.push_back(getFile(path));
	}

	for (const string &path : mcodePaths) {
		mcodes.push_back(getFile(path));
	}



     // Start of the program
     cout << "ADVANCED ALGORITHMS - SITUATION PROBLEM 1\n";
     cout << "ANAYZING TRANSMISSIONS AND MALICIOUS CODE IN FILES...\n";


     // Part 1: Search for every mcode inside every transmission and display the result
     // If the mcode is found, every occurrence is displayed after true.
     cout << "\nPART 1:SUBCHAIN TEST\n";

     Subchain subchain;

     for(int t = 0; t < transmissions.size(); t++){
          cout << "\nT R A N S M I S S I O N " << t + 1 << "\n\n";

          for(int m = 0; m < mcodes.size(); m++){
               cout << "mcode " << m + 1 << endl;

               vector<int> positions = subchain.search(transmissions[t], mcodes[m]);

               if(positions.empty()){
                    cout << "(false) Chain not found in the transmission\n\n";
               } else {
                    for(int position : positions){
                         cout << "(true) Initial position: " << position + 1 << " Final position: " << position + mcodes[m].length() << endl;
                    }
                    cout << endl;
               }
          }
     }

	// Part 2: search the longest palindrome inside every transmission and display the result.
	// If a palindrome is found, its start and end indexes are displayed.
	cout << "\nPALINDROME TEST\n";

	Palindrome palindrome;
	for (const string &transmission : transmissions) {
		tuple<int,int> position = palindrome.palindrome(transmission);
		// An end index of -1 means the transmission is empty, so there is no palindrome
		if (get<1>(position) == -1) {
			cout << "No poligon found." << endl;
		} else {
			cout << "Longest Polindrom at start: " << get<0>(position) << " end: " << get<1>(position) << endl;
		}
	}

	// Part 3: search the longest common substring between the two transmissions and display the result.
	// If it exists, its start and end positions are displayed.
	cout << "\nLONGEST COMMON SUBSTRING TEST\n";

	Substring substring;
	pair<int, int> result = substring.search(transmissions[0], transmissions[1]);
	if (result.first == -1) {
		cout << "No common substring found." << endl;
	} else {
		// Indexes are zero-based, the output is one-based
		cout << result.first + 1 << " " << result.second + 1 << endl;
	}

	return 0;
}