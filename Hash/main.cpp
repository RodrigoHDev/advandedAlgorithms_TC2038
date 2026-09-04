/*
 * Title: main.cpp
 *
 * Description:
 * Implementation of the obtention of a key for Hash based on a given
 * string through a txt file name. 
 * Division over input n and insertion into a matrix of n columns.
 * The obtetion process is composed of sum, module and pass to HEX.
 *
 * Implementation for the subject - Analysis and Design of Advanced
 * Algorithms
 *
 * Author: Alexis Yaocalli Berthou Haas - A01713458 & Rodrigo Alejandro Hurtado Cortes - A01713854
 * Date: Septiembre 3, 2026
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>

#include "hash.h"

using namespace std;

/*
main()
Function responsible for the reception of the inputs: fileName and number n, and 
afterwards process to obtain a key based on the file string.
Where:
- fileName = the name of the file to access a concrete string.
- n = number of columns of the matrix to insert the string

Parameters: none.
Return: exit status code (0 on successful execution).
*/
int main(){
    string fileName = "";
    int n = 0;

    //Obtention of the fileName
    cout<<"Please enter name of the file without extention: "<<endl;
    cin >> fileName;
    cout<<endl;

    //Obtention of the number n
    cout<<"Please enter number n which must be multiple of 4 within 16 and 64: "<<endl;
    cin>>n;
    cout<<endl;

    //Access and save the file content into string fileContent
    ifstream file(fileName+".txt");
    
    if (!file.is_open()) {
        cout << "Failed to open the file.\n";
        return 1;
    }
    
    string fileContent = "";
    char c;

    while (file.get(c)) {
        if (c == '\r') {
            continue;
        }
        else if (c == '\n') {
            fileContent += '-'; // Si es salto de línea, lo convertimos en '-'
        } else {
            fileContent += c;   // Si es cualquier otro carácter, lo dejamos igual
        }
    }

    cout<<"Conteido del archivo: "<<endl;
    cout<<fileContent;
    cout<<endl;

    //Creation of the Hash object and call to obtainKey() function
    Hash hash = Hash(fileContent, n);
    hash.obtainKey();

	return 0;
}