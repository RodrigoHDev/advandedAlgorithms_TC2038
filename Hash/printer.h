/*
 * Title: printer.h
 *
 * Description:
 * Helper class responsible for printing any kind of vector independently of its content.
 *
 * Implementation for the subject- Analysis and Design of Advanced
 * Algorithms
 *
 * Author: Alexis Yaocalli Berthou Haas - A01713458 & Rodrigo Alejandro Hurtado Cortes - A01713854
 * Date: Septiembre 3, 2026
 */

#ifndef PRINTER_H
#define PRINTER_H

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;


class Printer {
public:
    
    template <typename T>
    void printArray(vector<T>& array) {
        for (const T& element : array) {
            cout << element << " ";
        }
        cout<<""<<endl;
    }
};

#endif