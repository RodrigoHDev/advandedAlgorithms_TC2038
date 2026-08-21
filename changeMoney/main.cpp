
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>

#include "moneyChangeDP.h"

using namespace std;

int main(){

    int number = 0;
    int coins = 0;
    int amount = 0;
    vector<int> coinsVec = {};

    cout<<"Enter quantity: "<<endl;
    cin >> number;
    cin.ignore();

    cout<<"Enter number of coins: "<<endl;
    cin >> coins;
    cin.ignore();

    cout<< "Enter the coins values in ascending order: " <<endl;
    for(int i = 0; i< coins; i++){
        cin>>amount;
        cin.ignore();
        coinsVec.push_back(amount);
    }

    MoneyChangeDP mon = MoneyChangeDP();
    vector<int> numCoins = mon.giveChange(number, coinsVec);
    
    for(int i = 0; i < coinsVec.size(); i++){
        cout<<"Coin: "<<to_string(coinsVec[i])<<" Amount: "<<to_string(numCoins[i])<<endl;
    }

    return 0;
}
