/**
 * Title: main.cpp
 *
 * Description:
 * Main program to analyze transmission files against malicious code files under the conditions:
 * - The user enters the names (without extension) of the transmission files and of the malicious code (mcode) files.
 * - If the user enters a name of a single character, the default file for that position is kept.
 * - The content of every file is printed before the analysis.
 * - The Subchain object searches every mcode inside every transmission and prints the positions of each occurrence.
 * - The sections for the greatest palindrome and the common substring are reserved for the next algorithms.
 * 
 * Implementation for the subject - Analysis and Design of Advanced Algorithms
 *
 * Author: Alexis Yaocalli Berthou Haas - A01713458 & Rodrigo Alejandro Hurtado Cortes - A01713854
 * Date: Septiembre 3, 2026
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "subchain.h"

using namespace std;

/*
In order to analyze complexity:
t = number of transmission files.
m = number of malicious code files.
T = length of a transmission file (characters).
M = length of a malicious code file (characters).
L = length of the file being read.
*/

/**
 * printFile()
 * Reads every file of the given list and prints its content to the console,
 * preceded by the name of the file. Only '\r' is removed, so the line breaks
 * of the file are kept in the output.
 * 
 * Complexity: 
 *  Time: O(sum of L) over all the files in the list
 *  Space: O(L) (the content of one file at a time)
 * 
 * Params: 
 * files is a vector of strings with the names (without .txt) of the files to print.
 * type is "transmission" to read from the "transmission/" folder; any other
 *  value reads from the "mcode/" folder.
 * Returns: none
 */
void printFile(vector<string> files, string type){
     for(int i = 0; i < files.size(); i++){
          // Path depends on the type of file
          string path;
          if(type == "transmission"){
               path = "transmission/"+files[i]+".txt";
          }
          else{
               path = "mcode/"+files[i]+".txt";
          }

          ifstream file(path);
          if(!file.is_open()){
               cout<<"Failed to open the file."<<endl;
          }

          // Copy every character except '\r'
          string fileContent = "";
          char c;
          while(file.get(c)){
               if(c != '\r'){
                    fileContent += c;
               }
          }
          file.close();
          cout<<"Contenido del archivo:"<<files[i]<<endl;
          cout<<fileContent<<endl;
          cout<<" "<<endl;
     }
}

/**
 * main()
 * Asks for the names of the transmission and malicious code files, prints
 * their content and runs the subchain search over every pair of files.
 * 
 * Complexity: 
 *  Time: O(t * m * (T + M)) (dominated by Subchain::runSequence)
 *  Space: O(T + M) (dominated by the chain built inside Subchain::search)
 * 
 * Params: none
 * Returns: 0 when the program finishes.
 */
int main(){
     string fileName = "";
     //Default cases in case the user does not provide any name;
     vector<string> transmission = {"transmission01","transmission02"};
     vector<string> mcode = {"mcode01","mcode02","mcode03"};
     int numTra = 2;
     int numMco = 3;

    // Obtention of the transmission file names
     cout<<"Please enter transmission file's names without extension:"<<endl;
     cout<<"Expected "<<numTra<<endl;
     for(int i = 0; i < numTra; i++){
          cout<<"File No. "<<i+1<<" :";
          cin>>fileName;
          cout<<endl;
          // A name of one character keeps the default file
          if(fileName.length() != 1){
               transmission[i] = fileName;
          }
     }
     
    // Obtention of the malicious code file names
     cout<<"Please enter malicious code file's names without extension:"<<endl;
     cout<<"Expected "<<numMco<<endl;
     for(int i = 0; i < numMco; i++){
          cout<<"File No. "<<i+1<<" :";
          cin>>fileName;
          cout<<endl;
          // A name of one character keeps the default file
          if(fileName.length() != 1){
               mcode[i] = fileName;
          }
     }
     
     
     //Print all texts.
     printFile(transmission, "transmission");
     printFile(mcode, "mcode");

     //Process is handled automatically inside each class.
     //3 Classes = Each one handles one of the algorithms.

    // Access to the text file
     // Algorithm 1: search every mcode inside every transmission (Z-function)
     cout<<"M A T C H I N G   S U B S E Q U E N C E S"<<endl;
     Subchain subchain = Subchain(transmission, mcode);
     subchain.runSequence();

     // Algorithm 2: pending
     cout<<"G R E A T E S T   P A L I N D R O M"<<endl;


     // Algorithm 3: pending
     cout<<"C O M M O N   S U B S T R I N G"<<endl;


     return 0;
}