/*
 * Title: 
 *
 * Description:

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
In order to analyze complecity:
rows = ceil(text.length() / n).
n = number determined by the user.
*/

class Subchain{
     private:
     vector<string> transmission;
     vector<string> mcode;
     
     public:
     Subchain(){};

     Subchain(vector<string> transmission_, vector<string> mcode_){
          transmission = transmission_;
          mcode = mcode_;
     }

     void runSequence(){
          for(int t = 0; t < transmission.size(); t++){
               for (int c = 0; c < mcode.size(); c++){
                    cout<<"Transmission: "<<transmission[t]<<" Malicious Code: "<<mcode[c]<<endl;
                    vector<tuple<int,int>> positions = search(transmission[t],mcode[c]);
                    if(positions.size() == 0){
                         cout <<"false. No appearance of mcode "<<mcode[c]<<" in file "<<transmission[t]<<endl;
                    }
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


     // Function to find all occurrences of pattern in text
     vector<tuple<int,int>> search(string fileNameTransmission, string fileNameCode) {
          vector<tuple<int,int>> pos;
          string transmission = getFile(fileNameTransmission, "transmission");
          string mcode = getFile(fileNameCode, "mcode");
          if(transmission == "" || mcode == ""){
               return pos;
          }
          string chain = mcode + '$' + transmission;
          vector<int> zIndex = zFunction(chain);
          int size = mcode.size();

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

     
     /*Auxiliar function getFile*/
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