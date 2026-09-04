/*
 * Title: hash.h
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

#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class Hash{
    private:

    string text;
    int n;

    public:

    Hash(string text_, int n_){
        text = text_;
        n = n_;
    };

    void obtainKey(){
        vector<vector <char>> matrix (ceil(text.length() / (float) n), vector<char>(n));
        vector<int> sum (n);

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

        //Add the columns
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


        //Print matrix
        for(int i = 0; i < matrix.size(); i++){
            for(int j = 0; j < matrix[0].size(); j++){
                cout<<matrix[i][j]<<" ";
            }
            cout<<""<<endl;
        }

        for(int i = 0; i< sum.size(); i++){
            cout<< to_string(sum[i])<<" ";
        }
        cout<<""<<endl;
    }


};

#endif