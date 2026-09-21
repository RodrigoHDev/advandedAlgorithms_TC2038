/**
 * Title: main.cpp
 *
 * Description:
 * Main program to analyze the fixed transmission and malicious code files.
 * - The five files are read automatically from their respective folders.
 * - Line breaks are ignored so each file is treated as a single string.
 * - Every mcode is searched inside every transmission.
 * - The output of this program corresponds to part 1 of the situation problem.
 *
 * Implementation for the subject - Analysis and Design of Advanced Algorithms
 *
 * Author: Alexis Yaocalli Berthou Haas - A01713458 & Rodrigo Alejandro Hurtado Cortes - A01713854
 * Date: Septiembre 3, 2026
 */

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "subchain.h"
#include "substring.h"

using namespace std;

/*
In order to analyze complexity:
t = number of transmission files.
m = number of malicious code files.
T = length of a transmission file (characters, without line breaks).
M = length of a malicious code file (characters, without line breaks).
L = length of the file being read.
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
string getFile(const string &path){
     ifstream file(path);
     if(!file.is_open()){
          return "";
     }

     string fileContent = "";
     char c;
     while(file.get(c)){
          if(c != '\r' && c != '\n'){
               fileContent += c;
          }
     }
     file.close();
     return fileContent;
}

/**
 * main()
 * Reads the fixed transmission and malicious code files, then displays the
 * required result for every transmission and mcode pair.
 *
 * Complexity:
 *  Time: O(t * m * (T + M)) since Subchain::search() is called once per pair.
 *  Space: O(T + M) (dominated by the chain built inside Subchain::search()).
 *
 * Params: none
 * Returns: 0 when the program finishes.
 */
int main(){
     vector<string> transmissionPaths = {"transmission/transmission1.txt","transmission/transmission2.txt"};
     vector<string> mcodePaths = {"mcode/mcode1.txt","mcode/mcode2.txt","mcode/mcode3.txt"};

     vector<string> transmissions;
     vector<string> mcodes;

     for(const string &path : transmissionPaths){
          transmissions.push_back(getFile(path));
     }

     for(const string &path : mcodePaths){
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


     
     // Part 3: Search for the longest common substring between two trasmissions and display the result
     cout << "\nPART 3: TRANSMISSION SIMILARITY TEST\n\n";
     
     
     Substring substring;
     pair<int, int> result = substring.search(transmissions[0], transmissions[1]);
     if(result.first == -1){
          cout << "No common substring found." << endl;
     } else {
          cout << "LONGEST COMMON SUBSTRING found at: " << result.first + 1 << " - " << result.second + 1 << endl;
     }

     return 0;
}
