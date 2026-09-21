/*
 * Title: substring.h
 *
 * Description:
 * Creation of Substring object to find the longest common substring between
 * two transmissions using dynamic programming. The class only compares the
 * strings that it receives; reading files and displaying results are handled
 * by the main program.
 *
 * Implementation for the subject- Analysis and Design of Advanced
 * Algorithms
 *
 * Author: Alexis Yaocalli Berthou Haas - A01713458 & Rodrigo Alejandro Hurtado Cortes - A01713854
 * Date: Septiembre 3, 2026
 */

#ifndef SUBSTRING_H
#define SUBSTRING_H

#include <string>
#include <utility>
#include <vector>

using namespace std;

/*
In order to analyze complexity:
T1 = length of the first transmission.
T2 = length of the second transmission.
*/

class Substring{
     public:

     /**
      * search()
      * Finds the longest common substring between two transmissions using
      * dynamic programming. M[i][j] stores the length of the common substring
      * that ends at transmission1[i - 1] and transmission2[j - 1].
      *
      * Complexity:
      *  Time: O(T1 * T2)
      *  Space: O(T1 * T2)
      *
      * Params:
      * transmission1 is the content of the first transmission file.
      * transmission2 is the content of the second transmission file.
      * Returns:
      * a pair with the zero-based initial and final positions, both inclusive,
      * of the longest common substring in transmission1. Returns {-1, -1} if
      * either transmission is empty.
      */
      
     pair<int, int> search(const string &transmission1, const string &transmission2) {
          int size1 = transmission1.length();
          int size2 = transmission2.length();
          vector<vector<int>> matrix(size1 + 1, vector<int>(size2 + 1, 0));

          int maximum = 0;
          int endPosition = -1;

          for(int i = 1; i <= size1; i++){
               for(int j = 1; j <= size2; j++){
                    if(transmission1[i - 1] == transmission2[j - 1]){
                         matrix[i][j] = 1 + matrix[i - 1][j - 1];

                         if(matrix[i][j] > maximum){
                              maximum = matrix[i][j];
                              endPosition = i - 1;
                         }
                    } else {
                         matrix[i][j] = 0;
                    }
               }
          }

          int startPosition = endPosition - maximum + 1;
          return {startPosition, endPosition};
     }
};

#endif