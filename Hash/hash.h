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
     * Complexity: O(1) time and O(1) auxiliary space (text is stored by value).
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
     *   Time:  TODO: Complete here
     *   Space: TODO: Complete here
     * (dominated by assembleMatrix).
     * 
     * Parameters: None
     * Returns: the key of the given text.
     */
    string obtainKey(){
        
        vector<int> sum (n);
        vector<vector <char>> matrix = assembleMatrix();
        vector<int> sum = addColumns(matrix);

        printMatrix(matrix);
        printer.printArray(sum);

        return "";
        
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
    void printMatrix(vector<vector<char>> matrix){
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
     * top to bottom). Any cell beyond the length of text is padded with '['.
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
                if(counter <= text.length()){
                    matrix[i][j] = text[counter];
                    counter ++;
                }
                else{
                    matrix[i][j] = '[';
                }
            }
        }

        return matrix;
    }

    /**
     * addColumns()
     * Sums each column of the matrix into a single value modulo 256.
     * '-' characters count as 10 and '[' padding characters count as n;
     * every other character contributes its ASCII value.
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
    vector<int> addColumns(vector<vector<char>> matrix){
        vector<int> sum(n);

        for(int i = 0; i<sum.size(); i++){
            for(int row = 0; row < matrix.size(); row++){
                if(matrix[row][i] == '-'){
                    sum[i] += 10;
                }
                else if(matrix[row][i] == '['){
                    sum[i] += n;
                }
                else{
                    sum[i] += matrix[row][i];
                }
            }
            sum[i] = sum[i]%256;
        }

        return sum;
    }

};

#endif