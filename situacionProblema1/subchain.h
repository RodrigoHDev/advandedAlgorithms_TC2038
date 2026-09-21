/*
 * Title: subchain.h
 *
 * Description:
 * Creation of Subchain object to search for a malicious code inside a
 * transmission using the Z-function algorithm. The class only compares the
 * strings that it receives; reading files and displaying results are handled
 * by the main program.
 *
 * Implementation for the subject- Analysis and Design of Advanced
 * Algorithms
 *
 * Author: Alexis Yaocalli Berthou Haas - A01713458 & Rodrigo Alejandro Hurtado Cortes - A01713854
 * Date: Septiembre 3, 2026
 */

#ifndef SUBCHAIN_H
#define SUBCHAIN_H

#include <algorithm>
#include <string>
#include <vector>
#include <tuple>

using namespace std;

/*
In order to analyze complexity:
T = length of a transmission file (characters, without line breaks).
M = length of a malicious code file (characters, without line breaks).
n = M + T + 1 = length of the chain mcode + '$' + transmission.
*/

class Subchain{
     public:

     /**
      * zFunction()
      * Computes the Z-array of a string. z[i] is the length of the longest
      * substring starting at i that is also a prefix of s. Uses a Z-box [l, r]
      * to reuse previously computed values instead of comparing from scratch.
      *
      * Complexity:
      *  Time: O(n) (each character is compared at most twice: once extending
      *        a Z-box and once when it is reused)
      *  Space: O(n)
      *
      * Params:
      * s is the string to analyze (in this class, mcode + '$' + transmission).
      * Returns:
      * a vector of integers with the Z-array of s. z[0] is left as 0.
      */
     vector<int> zFunction(const string &s) const {
          int n = static_cast<int>(s.length());
          vector<int> z(n);
          int l = 0, r = 0;

          for(int i = 1; i < n; i++){
               // Reuse previous calculations inside the current Z-box
               if(i <= r){
                    int k = i - l;
                    z[i] = min(r - i + 1, z[k]);
               }
               // Compare characters while the prefix continues matching
               while(i + z[i] < n && s[z[i]] == s[i + z[i]]){
                    z[i]++;
               }
               // Update the Z-box when the current match reaches farther
               if(i + z[i] - 1 > r){
                    l = i;
                    r = i + z[i] - 1;
               }
          }
          return z;
     }

     /**
      * search()
      * Finds the first occurrence of a malicious code inside a transmission.
      * Builds the chain mcode + '$' + transmission and returns the first
      * position where the Z-value equals the length of the mcode.
      *
      * Complexity:
      *  Time: O(T + M) (building the chain, zFunction and the final scan are
      *        all linear in n)
      *  Space: O(T + M) (the chain and the Z-array)
      *
      * Params:
      * transmission is the content of a transmission file without line breaks.
      * mcode is the content of a malicious code file without line breaks.
      * Returns:
      * the zero-based position of the first occurrence in transmission, or -1
      * if mcode is empty or does not appear in transmission.
      */
     int search(const string &transmission, const string &mcode) const {
          if(transmission.empty() || mcode.empty()){
               return -1;
          }

          // '$' separates the pattern from the text because it is not a valid input character
          string chain = mcode + '$' + transmission;
          vector<int> zIndex = zFunction(chain);
          int size = static_cast<int>(mcode.length());

          // Only the part of the chain that belongs to the transmission is checked
          for(int i = size + 1; i < static_cast<int>(zIndex.size()); i++){
               if(zIndex[i] == size){
                    return i - size - 1;
               }
          }
          return -1;
     }




     tuple<int,int> palindrome(const string &transmission) const {
          string chain = expandedChain(transmission);
          cout<<chain<<endl;
          vector<int> palindromeIndex = searchPalindrome(chain);

          for(int i = 0; i<palindromeIndex.size(); i++){
               cout<<palindromeIndex[i]<<" ";
          }
          cout<<" "<<endl;

          int maxPalindromePosition = -1;
          int maxPalindromeValue = -1;
          for(int i = 0; i < palindromeIndex.size(); i++){
               if(palindromeIndex[i] > maxPalindromeValue){
                    maxPalindromePosition = i;
                    maxPalindromeValue = palindromeIndex[i];
               }
          }
          tuple<int,int> response = {maxPalindromePosition-maxPalindromeValue, maxPalindromePosition+maxPalindromeValue};
          return response;
     }

     string expandedChain(const string &transmission) const{
          string chain = "|";
          for(int i = 0; i<transmission.length(); i++){
               chain+=transmission[i]+"|";
          }
          return chain;
     }

     vector<int> searchPalindrome(string chain) const{
          vector<int> palindrome(chain.length());
          int center = 0;
          int right = 0;
          for(int i = 1; i < chain.length(); i++){
               int iMirror = center - (i-center);
               // Validation through mirror property of palindrome
               if(right > i){
                    palindrome[i] = min(right-i, palindrome[iMirror]);
               }
               // Addition to palindrome number from the i index.
               while(palindrome[i+1+chain[i]] == palindrome[i-1-chain[i]]){
                    palindrome[i] += 1;
               }
               // Modification of range based on greater analyzed palindrome
               if(i+palindrome[i] > right){
                    center = i;
                    right = i+palindrome[i];
               }
          }
          return palindrome;
     };
};

#endif
