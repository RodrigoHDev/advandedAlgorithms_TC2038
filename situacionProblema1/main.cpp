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


     // Part 1: Search for every mcode inside every transmission and display the result
     // If the mcode is found, the position is displayed as true followed by the index of the first character.
     
     Subchain subchain;
     for(const string &transmission : transmissions){
          for(const string &mcode : mcodes){
               int position = subchain.search(transmission, mcode);
               if(position == -1){
                    cout << "false" << endl;
               }
               else{
                    cout << "true " << position + 1 << endl;
               }
          }
     }

     return 0;
}
