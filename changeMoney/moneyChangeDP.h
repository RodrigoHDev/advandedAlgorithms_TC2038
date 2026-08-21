/* 
 * Title: moneyChangeDP.h
 * 
 * Description: 
 * Dynamic Programming approach towards obtaing the minimum number
 * of coins to return a concrete quantity of money based on an infinite
 * number of designated value coins/bills.
 * 
 * Implementation for the subject - Analysis and Design of Advanced
 * Algorithms
 * 
 * Author: Rodrigo Alejandro Hurtado Cortes - A01713854
 * Date: August 21, 2026
 */

#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

class MoneyChangeDP{
    /*
    For complexity analysis take into consideration:
        C = number of coins' values.
        Q = quantity being looked for.
    */

    public:
    /*
    MoneyChangeDP()
    Default constructor of the class MoneyChangeDP

    Return: none.
    */
    MoneyChangeDP(){};


    /*
    giveChange()
    Function responsible for running the DP algorith composed of:

        - Creation of a matrix of no. of coins needed to reach a value, 
        per each value from 0 to looked quantity for all available coins.
        Time Complexity: O(CQ). 
            Due to the double loop each depending over one of the two variables.
        Space Complexity: O(CQ).
            As the matrix is composed of C x Q elements.


        - Look for the answer across the created matrix looking for the value
        of coins which minimum amount leads to total value.
        Time Complexity: O(CQ). 
            Taking into consideration the worst case with $1 coins for Q quantity,
            it would take Q times in the outer while loop and C times in the inner
            for loop.
        Space Complexity: O(C).
            The asnwer is stored in a vector of the same size the values of coins.
    
    quantity: number to reach based on coins/bills
    coins: denomination of the available coins/bills.
    Return: vector of amounts of coins in the same order as the parameter
    coins.

    Overall Complexity:
        Temporal: O(CQ)
        Spacial: O(CQ) (as worst case always overrides)
    
    */
    vector<int> giveChange(int quantity, vector<int>& coins){

        //CREATION OF THE TABLE
        // Declaration of the values table. [num of coins][0 to total value]
        int change[coins.size()+1][quantity+1];

        // Fill up of the coins quantity
        for(int c=0; c<coins.size(); c++){
            for(int n = 0; n <= quantity; n++){
                if(c == 0)
                    change[c][n] = n;
                else if(n < coins[c])
                    change[c][n] = change[c-1][n];
                else
                    change[c][n] = min(change[c-1][n], 1+change[c][n-coins[c]]);
            }
        }

        //ANSWER OBTENTION
        // Creation of the answer vector
        vector<int> quantities ((int)coins.size(),0);
        
        //Minimum number of coins to return.
        int numCoins = change[coins.size()-1][quantity];
        int finalQuantity = quantity;

        //Obtention of quantities per each coin.
        while(finalQuantity > 0 && numCoins > 0){
            //Taking greater quantity of all
            int minQuantity = numCoins;
            int minValue = coins[coins.size()-1];

            //Loking for the minimum in the quantity column
            for(int i = coins.size()-1; i>=0; i--){
                if(change[i][finalQuantity] <= minQuantity){
                    //Saving coin value with min coin quantity
                    minQuantity = change[i][finalQuantity];
                    minValue = i;
                }
            }
            //Addition to quantites vector
            quantities[minValue] += 1;
            numCoins -= 1;
            finalQuantity -= coins[minValue];
        }

        return quantities;
    };
};