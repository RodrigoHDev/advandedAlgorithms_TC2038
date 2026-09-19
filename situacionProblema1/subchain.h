/*
 * Title: subchain.h
 *
 * Description:
 * Creation of Subchain object to search for malicious code inside transmission files under the conditions:
 * - Each transmission and each malicious code (mcode) is stored in a .txt file, referenced by its name (without extension).
 * - Transmission files are read from the "transmission/" folder and mcode files from the "mcode/" folder.
 * - Line breaks ('\r' and '\n') are ignored when reading a file, so the content is treated as a single string.
 * - Every mcode is searched in every transmission using the Z-function algorithm over the string mcode + '$' + transmission.
 * - For each search, all the occurrences are reported as a pair (start, end), where end is exclusive: [start, end).
 *
 * Implementation for the subject- Analysis and Design of Advanced
 * Algorithms
 *
 * Author: Alexis Yaocalli Berthou Haas - A01713458 & Rodrigo Alejandro Hurtado Cortes - A01713854
 * Date: Septiembre 3, 2026
 */

#ifndef SUBCHAIN_H
#define SUBCHAIN_H

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <tuple>

using namespace std;

/*
In order to analyze complexity:
t = number of transmission files.
m = number of malicious code files.
T = length of a transmission file (characters, without line breaks).
M = length of a malicious code file (characters, without line breaks).
n = M + T + 1 = length of the chain mcode + '$' + transmission.
k = number of occurrences found in a search.
*/

class Subchain{
     private:
     vector<string> transmission;
     vector<string> mcode;
     
     public:

     /**
      * Subchain()
      * Default constructor. Creates a Subchain object with no files to analyze.
      * 
      * Complexity: 
      *  Time: O(1) 
      *  Space: O(1)
      * 
      * Params: none
      * Returns: none
      */
     Subchain(){};

     /**
      * Subchain()
      * Builds a Subchain object with the names of the transmission files and
      * the names of the malicious code files to analyze.
      * 
      * Complexity: 
      *  Time: O(t + m) copies of the names (proportional to the total length of the names)
      *  Space: O(t + m) 
      * 
      * Params: 
      * transmission_ is a vector of strings with the names (without .txt) of the
      *  transmission files.
      * mcode_ is a vector of strings with the names (without .txt) of the
      *  malicious code files.
      * Returns: none
      */
     Subchain(vector<string> transmission_, vector<string> mcode_){
          transmission = transmission_;
          mcode = mcode_;
     }

     /**
      * runSequence()
      * Searches every malicious code in every transmission and prints, for each
      * pair, whether the code appears and, if so, the start and end position
      * of every occurrence.
      * 
      * Complexity: 
      *  Time: O(t * m * (T + M)) since search() is called once per pair and
      *        each call re-reads both files. Printing the results adds O(k) per pair.
      *  Space: O(T + M + k) (dominated by the chain built inside search).
      * 
      * Params: none
      * Returns: none
      */
     void runSequence(){
          for(int t = 0; t < transmission.size(); t++){
               for (int c = 0; c < mcode.size(); c++){
                    cout<<"Transmission: "<<transmission[t]<<" Malicious Code: "<<mcode[c]<<endl;
                    vector<tuple<int,int>> positions = search(transmission[t],mcode[c]);
                    // No occurrences found (or a file could not be read)
                    if(positions.size() == 0){
                         cout <<"false. No appearance of mcode "<<mcode[c]<<" in file "<<transmission[t]<<endl;
                    }
                    // Print every occurrence as [Start, End)
                    else{
                         cout <<"true. Appearance of mcode "<<mcode[c]<<" in file "<<transmission[t]<<" at: "<<endl;
                         for(int p = 0; p < positions.size(); p++){
                              cout<<"Start: "<<get<0>(positions[p])<<" End: "<<get<1>(positions[p])<<endl;
                         }
                    }
                    cout<<""<<endl;
               }
          }
     }

     /**
      * zFunction()
      * Computes the Z-array of a string. z[i] is the length of the longest
      * substring starting at i that is also a prefix of s. Uses a Z-box [l, r]
      * to reuse previously computed values instead of comparing from scratch.
      * 
      * Complexity: 
      *  Time: O(n) (each character is compared at most twice: once extending
      *        a Z-box and once when it is reused)
      *  Space: O(n) 
      * 
      * Params: 
      * s is the string to analyze (in this class, mcode + '$' + transmission).
      * Returns:
      * a vector of integers with the Z-array of s. z[0] is left as 0.
      */
     vector<int> zFunction(string &s) {
          int n = s.length();
          vector<int> z(n);
          int l = 0, r = 0;

          for (int i = 1; i < n; i++) {
               // Reuse prev calc
               if (i <= r) {
                    int k = i - l;
                    
                    z[i] = min(r - i + 1, z[k]);
               }
               // Counter compared to original pattern
               while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
                    z[i]++;
               }
               // Change box
               if (i + z[i] - 1 > r) {
                    l = i;
                    r = i + z[i] - 1;
               }
          }
          return z;
     }


     /**
      * search()
      * Finds all the occurrences of a malicious code inside a transmission.
      * Reads both files, builds the chain mcode + '$' + transmission and looks
      * for every position where the Z-value equals the length of the mcode.
      * 
      * Complexity: 
      *  Time: O(T + M) (reading the files, building the chain, zFunction and
      *        the final scan are all linear in n = M + T + 1)
      *  Space: O(T + M + k) (the chain, the Z-array and the k stored positions)
      * 
      * Params: 
      * fileNameTransmission is the name (without .txt) of the transmission file.
      * fileNameCode is the name (without .txt) of the malicious code file.
      * Returns:
      * a vector of tuples (start, end) with the position of every occurrence
      * in the transmission, where end is exclusive. The vector is empty if
      * there are no occurrences or if any file is empty or cannot be opened.
      */
     vector<tuple<int,int>> search(string fileNameTransmission, string fileNameCode) {
          vector<tuple<int,int>> pos;
          string transmission = getFile(fileNameTransmission, "transmission");
          string mcode = getFile(fileNameCode, "mcode");
          if(transmission == "" || mcode == ""){
               return pos;
          }
          // '$' separates the pattern from the text so no Z-value exceeds the size of mcode
          string chain = mcode + '$' + transmission;
          vector<int> zIndex = zFunction(chain);
          int size = mcode.size();

          // Only the part of the chain that belongs to the transmission is checked
          for (int i = size + 1; i < zIndex.size(); i++) {
               if (zIndex[i] == size){
                    // pattern match starts here in text
                    int origin = i - size - 1;
                    tuple<int,int> tup(origin, origin + size); 
                    pos.push_back(tup); 
               }
          }
          return pos;
     }

     
     /**
      * getFile()
      * Auxiliar function. Reads a .txt file and returns its content as a single
      * string, ignoring the line breaks ('\r' and '\n').
      * 
      * Complexity: 
      *  Time: O(L) where L is the number of characters in the file
      *  Space: O(L) 
      * 
      * Params: 
      * name is the name of the file, without the .txt extension.
      * type is "transmission" to read from the "transmission/" folder; any other
      *  value reads from the "mcode/" folder.
      * Returns:
      * a string with the content of the file without line breaks. If the file
      * cannot be opened, prints an error message and returns an empty string.
      */
     string getFile(string name, string type){
          string path;
          if(type == "transmission"){
               path = "transmission/"+name+".txt";
               
          }
          else{
               path = "mcode/"+name+".txt";
          }
          ifstream file(path);
          if(!file.is_open()){
               cout<<"Failed to open the file."<<endl;
               return "";
          }
          string fileContent = "";
          char c;
          // Copy every character except line breaks
          while(file.get(c)){
               if(c != '\r' && c != '\n'){
                    fileContent += c;
               }
          }
          file.close();
          return fileContent;
     }
};

#endif