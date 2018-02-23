/*
 * Puzzle.cpp
 *
 *  Created on: Nov 9, 2015
 *      Author: Erika
 */

#include "Puzzle.h"
#include <iostream>
#include <fstream>
#include "Dictionary.h"
#include <vector>


using namespace std;

//the constructor
Puzzle::Puzzle() {

	//initializes player points
	humanPoints = 0;

	//allows for the puzzle to be read in
	string fileName;
	ifstream puzzleStream;

	//read in the puzzle file name
	do {
		cout << "Enter puzzle file name:";
		cin >> fileName;
		cout<<endl;
		puzzleStream.open(fileName.c_str(), ios::in);

	} while (!puzzleStream.good());  //keep trying until this works

	string line;
	//make sure the file has some stuff in it
	if (!getline(puzzleStream, line)) {
		cerr << "Error: puzzle file is empty" << endl;
		numRows = 0;
		numCols = 0;
		return;
	}

	//put the first puzzle line in theBoard
	numCols = line.size();
	for (int i = 0; i < numCols; i++) {
		theBoard[0][i] = tolower(line.at(i));
	}

	//put the rest of the puzzle lines in theBoard
	for (numRows = 1; getline(puzzleStream, line); numRows++) {

		//make sure it's rectangular
		if (line.size() != numCols) {
			cerr << "Error: puzzle is not rectangular" << endl;
			numRows = 0;
			numCols = 0;
			return;
		}
		//put this puzzle line in theBoard
		for (int i = 0; i < numCols; i++) {
			theBoard[numRows][i] = tolower(line.at(i));
		}
	}

	//prints out the puzzle
	for (int i = 0; i < numRows; i++) {


		for (int j = 0; j < numCols; j++) {

			char c = toupper(theBoard[i][j]);
			if (j == numCols - 1) {

				cout << c << endl;
			} else {
				cout << c << " ";
			}
		}
	}

	cout << endl;

	//searches for all the words in the puzzle
	findWords();
}

void Puzzle:: findWords(){

	//creates a copy of the board
	char boardCopy[MAX_ROWS][MAX_COLUMNS];

	//checks for words starting at every row and every column
	for (int r = 0; r<numRows; r++){
		for (int c = 0; c<numCols; c++){

			//fills the boardCopy with the letters in the puzzle
			for (int i = 0; i<numRows; i++){
				for (int j = 0; j<numCols; j++){
					boardCopy[i][j] = theBoard[i][j];
				}
			}

			//looks for the words in the puzzle
			string word = "";
			checkWord(word, boardCopy, r, c);

		}
	}

	//alphabetizes all the words found
	sort(wordsFound.begin(), wordsFound.end());

}

void Puzzle :: checkWord(string word, char board[MAX_ROWS][MAX_COLUMNS], int row, int col){

	//adds the next letter to the potential word
	word = word + board[row][col];

	//the row or column are not legal indices
	if (row<0 || row>= numRows || col<0 || col>= numCols){
		return;
	}

	//the next letter is a blank space
	if (board[row][col] == ' '){
		return;
	}

	//the segment created so far is not the start of a word
	if (!dictionary.isPrefix(word)){
		return;
	}

	//if the word is more than 3 letters and is in the dictionary
	if (word.size()>=MIN_WORD_LENGTH && dictionary.binarySearch(word)){

		//ensures that the word has not already been found
		bool alreadyUsed = false;

		for (vector <string>::iterator i = wordsFound.begin(); i < wordsFound.end(); i++){
			if(word == *i){
				alreadyUsed = true;
			}
		}

		//puts the word on the vector if it is not already there
		if (!alreadyUsed){
			wordsFound.push_back(word);
		}
	}

	//sets the place you are checking to be blank so that this letter is not used again
	//for the same word
	board[row][col] = ' ';

	//checks for words in every direction
	for (int rOff = -1; rOff <= 1; rOff++){
		for (int cOff = -1; cOff <= 1; cOff++){

			//make sure it is not staying in place
			if(rOff != 0 || cOff != 0){

				char boardCopy[MAX_ROWS][MAX_COLUMNS];

				//creates a new copy of the board with any changes
				for (int i = 0; i<numRows; i++){
					for (int j = 0; j<numCols; j++){
						boardCopy[i][j] = board[i][j];
					}
				}

				//recursively calls check word to continue finding a word
				checkWord(word, boardCopy, row + rOff, col + cOff);

			}
		}
	}
}

void Puzzle :: humanPlay(){

	cout << "Enter as many words as you want, one to a line."<<endl;
	cout << "Enter an exclamation point when you are done." << endl;
	cout << endl;

	string humanWord;

	//the human keeps playing until they decide to be done
	while (humanWord != "!"){
		cin >> humanWord;

		//the human is done playing
		if (humanWord == "!"){
			break;
		}

		bool inPuzzle = false;

		//the word is in the puzzle
		for (vector <string>::iterator i = wordsFound.begin(); i < wordsFound.end(); i++){

			if(humanWord == *i){
				inPuzzle = true;
			}

		}

		bool inDictionary = false;

		//the word is in the dictionary
		if (dictionary.binarySearch(humanWord)){
			inDictionary = true;
		}

		bool alreadyUsed = false;

		//the word has not already been entered
		for (vector <string>::iterator i = humanWordsList.begin(); i < humanWordsList.end(); i++){

			if(humanWord == *i){
				alreadyUsed = true;
			}

		}

		bool tooSmall = false;

		//the word is the proper length to count
		if (humanWord.size()<MIN_WORD_LENGTH){
			tooSmall = true;
		}

		//if the word is invalid, these are the reasons why
		if (!inPuzzle){
			cout << "     invalid word: not in the board"<<endl;
		}

		if (!inDictionary){
			cout << "     invalid word: not in the dictionary"<<endl;
		}

		if (alreadyUsed){
			cout << "     invalid word: already used"<<endl;
		}

		if (tooSmall){
			cout << "     invalid word: words must have 3 or more characters"<<endl;
		}

		//if the word is in the puzzle and hasn't already been entered it is a good word
		if (inPuzzle && !alreadyUsed){
			humanWordsList.push_back(humanWord);
			cout << "     Good word!"<<endl;

		}
	}

	//displays all the human words found
	cout<<endl;
	cout<< "Human Words:"<<endl;

	cout<< "============" <<endl;

	//alphabetizes the human words found
	sort(humanWordsList.begin(), humanWordsList.end());

	for (vector <string>::iterator i = humanWordsList.begin(); i < humanWordsList.end(); i++){
		cout << *i <<endl;
	}

	//determines the human's score
	humanPoints = findScore(humanWordsList);

	cout << endl;
	cout << "Total Score: "<< humanPoints << endl;

}

void Puzzle ::computerPlay(){

	//displays the computer words found
	cout<<endl;
	cout<< "Computer Words:"<<endl;

	cout<< "============" <<endl;

	for (vector <string>::iterator i = wordsFound.begin(); i < wordsFound.end(); i++){

		//if the human found the word
		bool humanGotIt = false;

		for (vector <string>::iterator j = humanWordsList.begin(); j < humanWordsList.end(); j++){
			if (*j == *i){
				humanGotIt = true;
			}
		}

		//if the human found it, it does not count, otherwise it does
		if (humanGotIt){
			cout<< *i << "     disallowed: found by human"<<endl;
		}else{
			cout<< *i << endl;
		}

	}

	//finds the score of all the words found
	int score = findScore(wordsFound);

	//displays the computer score minus the human score
	cout << endl;
	cout << "Total Score: "<< (score - humanPoints) << endl;
}

int Puzzle :: findScore(vector <string> wordList){

	int numPoints = 0;

	//finds the score for each word size and adds them together to find the total score
	for (vector <string>::iterator i = wordList.begin(); i < wordList.end(); i++){

		if ((*i).size() == 3  ||(*i).size() == 4){
			numPoints++;
		}else if((*i).size() == 5){
			numPoints = numPoints + 2;
		}else if((*i).size() == 6){
			numPoints = numPoints + 3;
		}else if((*i).size() == 7){
			numPoints = numPoints + 5;
		}else if((*i).size() >= 8 ){
			numPoints = numPoints + 11;
		}

	}

	return numPoints;
}


