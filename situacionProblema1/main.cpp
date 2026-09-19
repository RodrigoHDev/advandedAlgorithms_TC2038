

/**
 * Title: main.cpp
 *
 * Description:
 * 
 * 
 * Implementation for the subject - Analysis and Design of Advanced Algorithms
 *
 * Author: Alexis Yaocalli Berthou Haas - A01713458 & Rodrigo Alejandro Hurtado Cortes - A01713854
 * Date: 
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "subchain.h"

using namespace std;


void printFile(vector<string> files, string type){
     for(int i = 0; i < files.size(); i++){
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
          if(fileName.length() != 1){
               transmission[i] = fileName;
          }
     }
     
    // Obtention of the number n
     cout<<"Please enter malicious code file's names without extension:"<<endl;
     cout<<"Expected "<<numMco<<endl;
     for(int i = 0; i < numMco; i++){
          cout<<"File No. "<<i+1<<" :";
          cin>>fileName;
          cout<<endl;
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
     cout<<"M A T C H I N G   S U B S E Q U E N C E S"<<endl;
     Subchain subchain = Subchain(transmission, mcode);
     subchain.runSequence();

     cout<<"G R E A T E S T   P A L I N D R O M"<<endl;


     cout<<"C O M M O N   S U B S T R I N G"<<endl;


     return 0;
}

