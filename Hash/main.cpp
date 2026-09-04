/*
 * Title: main.cpp
 *
 * Description:
 * Enter description here *
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
* Enter description here *

Parameters: none.
Return: exit status code (0 on successful execution).
*/
int main(){
    string fileName = "";
    int n = 0;

    cout<<"Please enter name of the file without extention: "<<endl;
    cin >> fileName;
    cout<<endl;

    cout<<"Please enter number n which must be multiple of 4 within 16 and 64: "<<endl;
    cin>>n;
    cout<<endl;

    ifstream file(fileName+".txt");
    
    if (!file.is_open()) {
        cout << "Failed to open the file.\n";
        return 1;
    }

    
    string file_contents = "";
    char c;

    while (file.get(c)) {
        if (c == '\r') {
            continue;
        }
        else if (c == '\n') {
            file_contents += '-'; // Si es salto de línea, lo convertimos en '-'
        } else {
            file_contents += c;   // Si es cualquier otro carácter, lo dejamos igual
        }
    }

    cout<<file_contents;
    cout<<endl;

    Hash hash = Hash(file_contents, n);
    hash.obtainKey();

	return 0;
}