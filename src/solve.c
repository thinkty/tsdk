
#include "solve.h"

/**
 * Given a one dimentional grid of size SDK_LEN*SDK_LEN (=9*9), solve the
 * sudoku problem and fill out the answer.
 * If the sudoku grid is unsolvable, return ERR.
 * This algorithm uses backtracking (bruteforce) to solve the sudoku.
 */
int solve(char * answ, int index)
{
	if (answ == NULL || index < 0) {
		return ERR;
	}

	/* Base case where all the cells have been checked */
	if (index >= SDK_SIZ) {
		return OK;
	}

	/* If the cell at the given index has already been filled, move on */
	if (answ[index] != '0') {
		return solve(answ, index+1);
	}

	/* Try 1 ~ 9 and check if there are no duplicates */
	int i;
	for (i = 1; i <= SDK_LEN; i++) {
		answ[index] = '0' + i;

		/* If there are duplicates, try the next number */
		if (checkrow(answ, index) != OK ||
			checkcol(answ, index) != OK ||
			checkbox(answ, index) != OK) {
			continue;
		}

		/* If the placement is valid, move on to next cell */
		if (solve(answ, index+1) == OK) {
			break;
		}
	}

	/* Not a single number is valid, reset the field and return ERR */
	if (i > SDK_LEN) {
		answ[index] = '0';
		return ERR;
	}

	return OK;
}

/**
 * Return OK if the row does not have duplicates (excluding '0')
 */
int checkrow(const char * answ, int index)
{
	if (answ == NULL) {
		return ERR;
	}

	/* Given the index number, get the first index in the row */
	int row = (index / SDK_LEN) * SDK_LEN;

	/* Iterate through the row and check for duplicates */
	char temp[SDK_LEN] = {0};
	char num;
	for (int i = 0; i < SDK_LEN; i++) {
		num = answ[row + i];
		if (num == '0') {
			continue;
		}
		if (++temp[num-'1'] > 1) {
			return ERR;
		}
	}

	return OK;
}

/**
 * Return OK if the column does not have duplicates (excluding '0')
 */
int checkcol(const char * answ, int index)
{
	if (answ == NULL) {
		return ERR;
	}

	/* Given the index number, get the column number */
	int col = index % SDK_LEN;

	/* Iterate through the column and check for duplicates */
	char temp[SDK_LEN] = {0};
	char num;
	for (int i = 0; i < SDK_LEN; i++) {
		num = answ[(i * SDK_LEN) + col];
		if (num == '0') {
			continue;
		}
		if (++temp[num-'1'] > 1) {
			return ERR;
		}
	}

	return OK;
}

/**
 * Return OK if the box does not have duplicates (excluding '0')
 */
int checkbox(const char * answ, int index)
{
	if (answ == NULL) {
		return ERR;
	}

	/* Given the index number, get the upper row and col */
	/* ex:  0,1,2,9,10,11,18,19,20     -> 0,0            */
	/*      3,4,5,12,13,14,21,22,23    -> 0,3            */
	/*      27,28,29,36,37,38,45,46,47 -> 1,0            */
	int row = ((index / SDK_LEN) / 3) * 3;
	int col = ((index % SDK_LEN) / 3) * 3;

	char temp[SDK_LEN] = {0};
	char num;
	for (int i = row; i < row+3; i++) {
		for (int j = col; j < col+3; j++) {
			num = answ[(i * SDK_LEN) + j];
			if (num == '0') {
				continue;
			}
			if (++temp[num-'1'] > 1) {
				return ERR;
			}
		}
	}

	return OK;
}

