/* 
 * Title: moneyChangeDP.h
 * 
 * Description: 
 * Dynamic Programming approach towards obtaining the minimum number
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
#include <algorithm>

using namespace std;

class MoneyChangeDP {
	
    private:
    const int INF = numeric_limits<int>::max();

    /*
    For complexity analysis take into consideration:
        C = number of coins' values.
        Q = quantity being looked for.
    */

    void sortCoins(vector<int>& coins){
        sort(coins.begin(), coins.end(), greater<int>());
    };


	public:
	/*
	MoneyChangeDP()
	Default constructor of the class MoneyChangeDP

	Return: none.
	*/
	MoneyChangeDP() {};

	/*
	giveChange()
	Function responsible for running the DP algorithm composed of:

		- Creation of the quantities matrix by the function matrixCreation()
        Time Complexity: O(CQ). 
            Due to the double loop each depending over one of the two variables.
        Space Complexity: O(CQ).
            As the matrix is composed of C x Q elements.

		- Look for the answer across the created matrix looking for the value
		of coins which minimum amount leads to total value.
		Time Complexity: O(CQ). 
			Taking into consideration the worst case with $1 coins for Q quantity,
			it would take Q times in the outer while loop and C times in the inner
			for loop given by the function findMinRow().
		Space Complexity: O(C).
			The answer is stored in a vector of the same size the values of coins.

	quantity: number to reach based on coins/bills
	coins: denomination of the available coins/bills.
	Return: vector of amounts of coins in the same order as the parameter
	coins.

	Overall Complexity:
		Temporal: O(CQ)
		Spatial: O(CQ) (as worst case always overrides)
	*/
	vector<int> giveChange(int quantity, vector<int>& coins) {
		
        sortCoins(coins);

        //Obtention of the matrix
		vector<vector<int>> coinTable = matrixCreation(quantity, coins);

        int remainingCoins = coinTable[coins.size() - 1][quantity];

        // Detección explícita de imposibilidad — sin importar si hay 1 o no.
        if (remainingCoins == INF) {
            return vector<int>{-1};
        }

        // Creation of the answer vector
		vector<int> coinCounts((int) coins.size(), 0);

		int remainingAmount = quantity;

        while(remainingAmount > 0 && remainingCoins > 0){
            int minCoinIndex = findMinRow(remainingAmount, remainingCoins, coins, coinTable);

            // Addition to coinCounts vector
            coinCounts[minCoinIndex] += 1;
            remainingCoins -= 1;
            remainingAmount -= coins[minCoinIndex];
		}

		return coinCounts;
	};

    /*
    matrixCreation()
    Creation of a matrix of no. of coins needed to reach a value, 
	per each value from 0 to looked quantity for all available coins.
	
    Time Complexity: O(CQ). 
		Due to the double loop each depending over one of the two variables.
	Space Complexity: O(CQ).
		As the matrix is composed of C x Q elements.
    
    quantity: number to reach based on coins/bills
	coins: denomination of the available coins/bills.
	Return: matrix of coins required for a specific value.
    */
    
vector<vector<int>> matrixCreation(int quantity, vector<int>& coins){
    vector<vector<int>> coinTable(coins.size(), vector<int>(quantity + 1, 0));

    for (int coinIndex = 0; coinIndex < coins.size(); coinIndex++) {
        for (int currentAmount = 0; currentAmount <= quantity; currentAmount++) {
            if (currentAmount == 0) {
                coinTable[coinIndex][currentAmount] = 0;
            }
            else if (coinIndex == 0) {
                coinTable[coinIndex][currentAmount] = 
                    (currentAmount % coins[0] == 0) 
                        ? currentAmount / coins[0] 
                        : INF;
            }
            else if (currentAmount < coins[coinIndex]) {
                coinTable[coinIndex][currentAmount] = coinTable[coinIndex - 1][currentAmount];
            }
            else {
                int sub = coinTable[coinIndex][currentAmount - coins[coinIndex]];
                int useCoin = (sub == INF) ? INF : 1 + sub;
                coinTable[coinIndex][currentAmount] = min(coinTable[coinIndex - 1][currentAmount], useCoin);
            }
        }
    }
    return coinTable;
}


    /*
    findMinRow()
    Helper function responsible for obtaining the index in the vector coins of the greatest value that has
    the minimum number of coins required.

    Time Complexity: O(C). 
		Due to the double loop each depending over one of the two variables.
	Space Complexity: O(1).
        The function does not creates any new data structure, only checks previous and returns the minimum.

    remainingAmount: amount of value unassigned to specific coins.
    remainingCoins: amount of remianing coins to assign.
    coins: denomination of the available coins/bills.
    coinTable: matrix of coins required for a specific value.
    return: index of the value of the greatest coin with the minimum number of coins to assign.
    */
    int findMinRow(int remainingAmount, int remainingCoins, vector<int>& coins, vector<vector<int>>& coinTable){
        // Taking greater quantity of all
        int minCoinsAtAmount = remainingCoins;
        int minCoinIndex = coins[coins.size() - 1];

        // Looking for the minimum in the quantity column
        for (int coinLookupIndex = coins.size() - 1; coinLookupIndex >= 0; coinLookupIndex--) {
            if (coinTable[coinLookupIndex][remainingAmount] <= minCoinsAtAmount) {
                // Saving coin value with min coin quantity
                minCoinsAtAmount = coinTable[coinLookupIndex][remainingAmount];
                minCoinIndex = coinLookupIndex;
            }
        }
        return minCoinIndex;
    };


    void printResult(const vector<int>& result, const vector<int>& coins) {
        cout << "Coin Distribution:" << endl;
        for (size_t i = 0; i < result.size(); ++i) {
            cout << "Coin " << coins[i] << ": " << result[i] << endl;
        }
    }
};