/*
 * main.cpp

 *
 *  Created on: Nov 9, 2015
 *      Author: Erika
 */


#include <cstdlib>
#include <iostream>
#include "Dictionary.h"
#include "Puzzle.h"

using namespace std;

int main(void){

	cout << "Welcome to Boggle!"<<endl<<endl;

	//creates a boggle puzzle
	Puzzle boggle;

	//Finds all the possible words in the puzzle
	boggle.findWords();

	cout << "Would you like to play Boggle? (y or n)"<< endl;

	string playerResponse;
	cin >> playerResponse;
	cout << endl;

	//if the player wants to play, let them play, then the computer
	if (playerResponse == "y"){
		boggle.humanPlay();
		boggle.computerPlay();
	}

	//if the player does not want to play, the computer will play
	if (playerResponse == "n"){
		boggle.computerPlay();
	}

	return EXIT_SUCCESS;
}

