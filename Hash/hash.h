/*
 * Title: hash.h
 *
 * Description:
 * Creation of Hash object to obtain a key based on a given string under the conditions:
 * - Integer n determines the number of columns of the matrix to store the characters of the file content (including line spaces and spaces)
 * - If number of chars in content is not multiple of n, the last line is filled with the value of n.
 * - Within an array of length n calculate in each space = (sum of the ASCII chars per column) % 256.
 * - The exit is generated with the hexadecimal representation per two digits in each position of the array.
 * - The length of the final string will be n/4. 
 *
 * Implementation for the subject- Analysis and Design of Advanced
 * Algorithms
 *
 * Author: Alexis Yaocalli Berthou Haas - A01713458 & Rodrigo Alejandro Hurtado Cortes - A01713854
 * Date: Septiembre 3, 2026
 */

#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <sstream>
#include "printer.h"

using namespace std;

/*
In order to analyze complecity:
rows = ceil(text.length() / n).
n = number determined by the user.
*/

class Hash{
    private:

    string text;
    Printer printer;
    int n;

    public:

    /**
     * Builds a Hash object over the given text with n columns.
     * Complexity: O(m) time and O(m) auxiliary space, where m is the length of the text.
     */
    Hash(string text_, int n_){
        text = text_;
        n = n_;
        printer = Printer();
    };

    /**
     * obtainKey()
     * Assembles the character matrix, adds up each column mod 256, converts to HEX and returns
     * a new key based on text and n.
     * 
     * Complexity: 
     *   Time:  O(rows*n)
     *   Space: O(rows*n)
     * (dominated by assembleMatrix).
     * 
     * Parameters: None
     * Returns: the key of the given text.
     */
    string obtainKey(){
        vector<vector<char>> matrix = assembleMatrix();
        vector<int> sum = addColumns(matrix);

        printMatrix(matrix);
        printer.printArray(sum);
        
        string key = convertToHex(sum);
        cout<<key<<endl;
        
        return key;
    }

    /**
     * printMatrix()
     * Prints the character matrix row by row, space-separated, with a
     * trailing blank line.
     * 
     * Complexity: 
     *  Time: O(rows*n)  
     *  Space: O(1)
     * 
     * Params:
     *  matrix is a vector of vectors char with contains the given text
     *  inside a structure of n columns.
     * Returns: none
     */
    void printMatrix(vector<vector<char>>& matrix){
        for(int i = 0; i < matrix.size(); i++){
            for(int j = 0; j < matrix[0].size(); j++){
                cout<<matrix[i][j]<<" ";
            }
            cout<<""<<endl;
        }
        cout<<""<<endl;
    }

    /**
     * assebleMatrix()
     * Lays text out into a matrix of n columns (row-major, left to right,
     * top to bottom).
     * 
     * Complexity: 
     *  Time: O(rows*n) 
     *  Space: O(rows*n)
     * 
     * Params: none
     * Returns: a vector of vectors char with contains the given text
     *  inside a structure of n columns.
     */
    vector<vector<char>> assembleMatrix(){
        vector<vector <char>> matrix (ceil(text.length() / (float) n), vector<char>(n));
        int counter = 0;
        
        for(int i = 0; i < matrix.size(); i++){
            for(int j = 0; j < matrix[0].size(); j++){
                if(counter < text.length()){
                    matrix[i][j] = text[counter];
                    counter ++;
                }
                else{
                    matrix[i][j] = n;
                }
            }
        }

        return matrix;
    }

    /**
     * addColumns()
     * Sums each column of the matrix into a single value modulo 256.
     * 
     * Complexity: 
     *  Time: O(rows*n) 
     *  Space: O(n) 
     * 
     * Params: 
     * matrix is a vector of vectors char with contains the given text
     *  inside a structure of n columns.
     * Returns:
     * a vector with the addition and module of the respective characters of
     * each column.
     */
    vector<int> addColumns(vector<vector<char>>& matrix){
        vector<int> sum(n);

        for(int i = 0; i<sum.size(); i++){
            for(int row = 0; row < matrix.size(); row++){
                sum[i] += matrix[row][i];
            }
            sum[i] = sum[i]%256;
        }

        return sum;
    }

    /**
     * convertToHex()
     * Converts the given vector of integers to a string of hexadecimal
     * representation, two digits per integer.
     * 
     * Complexity: 
     *  Time: O(n) 
     *  Space: O(n) 
     * 
     * Params: 
     * sum is a vector with the addition and module of the respective characters of
     * each column.
     * Returns:
     * a string with the hexadecimal representation of the given vector.
    */
    string convertToHex(vector<int>& sum){
        stringstream result;

        for(int i = 0; i < sum.size(); i++){
            result<<uppercase<<hex<<setw(2)<<setfill('0')<<sum[i];
        }

        return result.str();
    }    

};

#endif