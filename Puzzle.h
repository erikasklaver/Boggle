/*
 * Puzzle.h
 *
 *  Created on: Nov 9, 2015
 *      Author: Erika
 */

#ifndef PUZZLE_H_
#define PUZZLE_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Dictionary.h"


class Puzzle {

private:

	//the board is 4x4 and the smallest word can be 3 letters
	static const int MAX_ROWS = 4;
	static const int MAX_COLUMNS = 4;
	static const int MIN_WORD_LENGTH = 3;

	//the number of columns and rows, the dictionary, the puzzle board, the list of computer
	//words found, the list of player words found and the number of player points
	int numRows;
	int numCols;
	Dictionary dictionary;
	char theBoard[MAX_ROWS][MAX_COLUMNS];
	vector <string> wordsFound;
	vector <string> humanWordsList;
	int humanPoints;

public:

	//the constructor
	Puzzle();

	//the functions needed to play the game
	void findWords();
	void checkWord(string word, char board[MAX_ROWS][MAX_COLUMNS], int row, int col);
	void humanPlay();
	void computerPlay();
	int findScore(vector <string> wordList);

};

#endif /* PUZZLE_H_ */
