#ifndef __TSDK_SOLVE_H
#define __TSDK_SOLVE_H

#include "common.h"

/**
 * Given a one dimentional grid of size SDK_LEN*SDK_LEN (= 9*9), solve the
 * sudoku problem and fill out the answer.
 * If the sudoku grid is unsolvable, return ERR.
 */
int solve(char *, int);

/**
 * Return OK if the row does not have duplicates (excluding '0')
 */
int checkrow(const char *, int);

/**
 * Return OK if the column does not have duplicates (excluding '0')
 */
int checkcol(const char *, int);

/**
 * Return OK if the box does not have duplicates (excluding '0')
 */
int checkbox(const char *, int);

#endif
