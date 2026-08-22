
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>

#include "moneyChange.h"

using namespace std;

int main(){
    int coinTypes;
    cin >> coinTypes;

    vector<int> coins(coinTypes);
    for (int i = 0; i < coinTypes; ++i) {
        cin >> coins[i];
    }

    int price;
    cin >> price;

    int payment;
    cin >> payment;

    int changeAmount = payment - price;

    MoneyChange moneyChange;

    cout << "Dynamic Programming Result:" << endl;
    vector<int> resultDP = moneyChange.giveChangeDP(changeAmount, coins);
    moneyChange.printResult(resultDP, coins);

    cout << "Greedy Algorithm Result:" << endl;
    vector<int> resultGreedy = moneyChange.giveChangeGreedy(changeAmount, coins);
    moneyChange.printResult(resultGreedy, coins);

    return 0;
}
