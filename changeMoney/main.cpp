/*
 * Title: main.cpp
 *
 * Description:
 * Entry point of the program. Reads coin denominations, price, and payment
 * from standard input, calculates the change amount, and computes the
 * minimum coin distribution using both the Greedy and Dynamic Programming
 * algorithms implemented in MoneyChange.
 *
 * Implementation for the subject - Analysis and Design of Advanced
 * Algorithms
 *
 * Author: Alexis Yaocalli Berthou Haas - A01713458 & Rodrigo Alejandro Hurtado Cortes - A01713854
 * Date: August 21, 2026
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>

#include "moneyChange.h"

using namespace std;

/*
main()
Reads the coin denominations, price, and payment from standard input,
calculates the change to be returned, and prints the coin distribution
computed by both the Greedy and Dynamic Programming algorithms.

Parameters: none.
Return: exit status code (0 on successful execution).
*/
int main(){
	// Read input from file
	int coinTypes = 0;
	cin >> coinTypes;

	vector<int> coins(coinTypes);
	for (int i = 0; i < coinTypes; i++) {
		cin >> coins[i];
	}

	int price = 0;
	cin >> price;

	int payment = 0;
	cin >> payment;

	int changeAmount = payment - price;
	if(changeAmount < 0){
		cout << "Amount to pay is greater than money. Invalid" << endl;
		cout << endl;
	}
	else{
		// Initialize MoneyChange object
		MoneyChange moneyChange;

		// Sort coins in descending order for the greedy algorithm and output consistency
		moneyChange.sortCoins(coins);

		// Calculate change using Dynamic Programming and Greedy Algorithm
		cout << "Greedy Algorithm Result:" << endl;
		vector<int> resultGreedy = moneyChange.giveChangeGreedy(changeAmount, coins);
		moneyChange.printResult(resultGreedy, coins);

		cout << "Dynamic Programming Result:" << endl;
		vector<int> resultDP = moneyChange.giveChangeDP(changeAmount, coins);
		moneyChange.printResult(resultDP, coins);
	}

	return 0;
}