/* 
 * Title: moneyChangeGreedy.h
 * 
 * Description: 
 * Greedy approach towards obtaing the minimum number of coins to return a concrete
 * quantity of money based on an infinite number of designated value coins/bills.
 * 
 * Implementation for the subject - Analysis and Design of Advanced
 * Algorithms
 * 
 * Author: Alexis Yaocalli Berthou Haas - A01713458
 * Date: August 21, 2026
 */

#ifndef MONEYCHANGEGREEDY_H
#define MONEYCHANGEGREEDY_H

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class MoneyChange{
    /*
    For complexity analysis take into consideration:
        C = number of coins' values.
        Q = quantity being looked for.
    */
    private:

    void sortCoins(vector<int>& coins){
        sort(coins.begin(), coins.end(), greater<int>());
    };

    public:
    
    /*
    MoneyChange()
    Default constructor of the class MoneyChange
    */

    MoneyChange(){};

    /*
     * Dynamic Programming
     *
     * dp[x] stores the minimum number of coins needed
     * to create the amount x.
     *
     * lastCoin[x] stores which coin was used last
     * to create the amount x.
     *   
     * The result is reconstructed by backtracking through the lastCoin array.
     *
     * Time Complexity: O(C * Q) + O(C log C) for sorting the coins.
     * Space Complexity: O(Q)
     */

     vector<int> giveChangeDP(int quantity, vector<int> coins){
        // DP does not require coins to be sorted, but we sort them for consistency in output
        sortCoins(coins);

        int numCoins = 0;

        // INF: A value greater than the maximum possible number of coins needed
        // Why? We intialize the dp array to ensure that any valid solution will 
        // be less than this value. Only works if the smallest coin is >=1.
        int INF = quantity + 1; 

        vector<int> dp(quantity + 1, INF);

        // Used to reconstruct the solution
        vector<int> lastCoin(quantity + 1, -1);

        // We initialize 0 to 0
        dp[0] = 0;

        // Caluclate best solution for all amounts from 1 to quantity
        for (int currentAmount = 1; currentAmount <= quantity; ++currentAmount) {
            // Test for every coin
            for (int coinIndex = 0; coinIndex < coins.size(); ++coinIndex) {
                
                int coinValue = coins[coinIndex];
                // Coins greater that the current amount cannot be used 
                if (coinValue <= currentAmount) {
                    // If using this coin results in a solution with fewer coins and the 
                    // previous solution is valid, update dp and lastCoin
                    if (dp[currentAmount - coinValue] + 1 < dp[currentAmount]) {
                        dp[currentAmount] = dp[currentAmount - coinValue] + 1;
                        lastCoin[currentAmount] = coinIndex;
                    }
                }
            }
        }

        vector<int> result(coins.size(), 0);

        // no solution exists
        if (dp[quantity] == INF) {
            return result; // Return a vector of zeros
        }

        // Reconstruct the solution
        int remainingAmount = quantity;

        while (remainingAmount > 0) {
            int coinIndex = lastCoin[remainingAmount];
            result[coinIndex]++;
            remainingAmount -= coins[coinIndex];
        }

        return result;
     };


    /*
    * Greedy Algorithm
    *
    * Uses the largest coin possible until the change is complete.
    *
    * quantity: amount of change to return.
    * coins: available coin denominations.
    *
    * Return: number of coins used for each denomination.
    *
    * Time Complexity: O(C)
    * Time complexity with sort: O(C log C).
    * Space Complexity: O(C)
    */

    vector<int> giveChangeGreedy(int quantity, vector<int>& coins){
        // Sort the coins in descending order
        sortCoins(coins);

        // Initialize the result vector to store the count of each coin used
        vector<int> result(coins.size(), 0);
        int remainingAmount = quantity;

        // Iterate through the sorted coins and use as many of each coin as possible
        for (int i = 0; i < coins.size(); ++i) {
            // if the coin value is less than or equal to the remaining amount, use it
            while (remainingAmount >= coins[i]) {
                remainingAmount -= coins[i];
                result[i]++;
            }
        }

        return result;
    };

    void printResult(const vector<int>& result, const vector<int>& coins) {
        cout << "Coin Distribution:" << endl;
        for (size_t i = 0; i < result.size(); ++i) {
            cout << "Coin " << coins[i] << ": " << result[i] << endl;
        }
    }

    vector<int> getSortedCoins(vector<int>& coins){
        sortCoins(coins);
        return coins;
    };
};

#endif