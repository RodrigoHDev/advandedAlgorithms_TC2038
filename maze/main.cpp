/*
 * Title: main.cpp
 *
 * Description:
 * 
 *
 * Implementation for the subject - Analysis and Design of Advanced
 * Algorithms
 *
 * Author: Alexis Yaocalli Berthou Haas - A01713458 & Rodrigo Alejandro Hurtado Cortes - A01713854
 * Date: August 24, 2026
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>

#include "branchBound.h"

using namespace std;

/*
printMaze()
Function responsible for printing the maze result.

maze: vector of vector of ints which contains the maze path or maze.
name: additional text to print.
return: None.
*/
void printMaze(vector<vector<int>> maze, string name){
    if(maze[0][0]==-1){
        cout<<endl;
        cout<<"NO VALID SOLUTION"<<endl;
    }
    else{
        cout<<endl;
        cout<<"Printing: " << name <<endl;
        cout<<endl;

        for (int i = 0; i < maze.size(); i++){
            for(int j = 0; j < maze[0].size(); j++){
                cout<< to_string(maze[i][j]) << " ";
            }
            cout<<endl;
        }
    }
}


/*
main()
Function responsible for accepting the user input and
process the given maze to return a path if possible
to go from 0,0 to N-1.

Parameters: none.
Return: exit status code (0 on successful execution).
*/
int main(){
	// Read input from file
    int columns;
    int rows;
    string row;
    cin>>rows>>columns;
    cin.ignore();

    //Declaration of maze size based on given rows and columns.
    vector<vector<int>> maze(rows, vector<int>(columns));

    //Fill up of the maze with the following lines of the input.
    for (int i = 0; i < rows; i++) {
        getline(cin, row);
        stringstream ss(row);
        for (int j = 0; j < columns; j++) {
            ss >> maze[i][j];
        }
    }

    //Printing of the original maze
    printMaze(maze, "Original");

    //Process though the Brand & Bound algorithm
    branchBound b;
    vector<vector<int>> final = b.travelMaze(maze, rows, columns);
    
    //Printing of the obtained maze from Brand & Bound algorithm
    printMaze(final, "Poda");

	return 0;
}