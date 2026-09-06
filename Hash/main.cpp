/**
 * Title: main.cpp
 *
 * Description:
 * Implementation of the obtention of a key for Hash based on a given
 * string obtained through a txt file.
 *
 * The file content is inserted into a matrix of n columns.
 * The hash generation process consists of adding the ASCII values
 * of each column, applying modulo 256 and converting the result to HEX.
 *
 * Implementation for the subject - Analysis and Design of Advanced Algorithms
 *
 * Author: Alexis Yaocalli Berthou Haas - A01713458 & Rodrigo Alejandro Hurtado Cortes - A01713854
 * Date: September 3, 2026
 */

#include <iostream>
#include <fstream>
#include <string>
#include "hash.h"

using namespace std;

/**
 * main()
 *
 * Function responsible for receiving the inputs fileName and n.
 *
 * Where:
 * - fileName = name of the file containing the text.
 * - n = number of columns of the matrix.
 *
 * The value n must be a multiple of 4 and must be between 16 and 64.
 *
 * The function reads every character of the file, including line breaks,
 * and sends the complete content to the Hash object.
 *
 * Parameters: none
 *
 * Returns:
 * 0 on successful execution.
 * 1 if the value of n is invalid or the file cannot be opened.
 */
int main(){
    string fileName = "";
    int n = 0;

    // Obtention of the file name
    cout<<"Please enter name of the file without extension:"<<endl;
    cin>>fileName;
    cout<<endl;

    // Obtention of the number n
    cout<<"Please enter number n which must be multiple of 4 within 16 and 64:"<<endl;
    cin>>n;
    cout<<endl;

    // Validation of n
    if(n < 16 || n > 64 || n%4 != 0){
        cout<<"Invalid value of n."<<endl;
        return 1;
    }

    // Access to the text file
    ifstream file("tests/"+fileName+".txt");

    if(!file.is_open()){
        cout<<"Failed to open the file."<<endl;
        return 1;
    }

    // Save the complete file content into a string
    string fileContent = "";
    char c;

    while(file.get(c)){
        if(c != '\r'){
            fileContent += c;
        }
    }

    file.close();

    // Display the original content of the file
    cout<<"Contenido del archivo:"<<endl;
    cout<<fileContent<<endl;

    // Creation of the Hash object and generation of the key
    Hash hash = Hash(fileContent, n);
    hash.obtainKey();

    return 0;
}