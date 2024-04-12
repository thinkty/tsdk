#ifndef __TSDK_SUDOKU_H
#define __TSDK_SUDOKU_H

#include <ncurses.h>

extern const char * SDK_HELP_MSGS[];

/**
 * Given a grid and the answer to that sudoku board, start the game. Returns
 * QUIT to end the game, ERR on error, and OK to play again.
 */
int start_game(char [], char []);

/**
 * Fill in the grid with the default values in bold. First array contains the entire grid and the next array contains the
 * original sudoku grid.
 */
void fill_grid(WINDOW *, const char *, const char *);

/**
 * Handle user input in the grid window. Returns QUIT to end the game, ERR on
 * error, and OK to play again. Unlike handle_menu, this part is handled here
 * just because it is cumbersome to put it in tui.
 */
int handle_grid(WINDOW *, WINDOW *, char *, const char *, const char *);

/**
 * Check the validity of the sudoku by comparing it with the answer. If wrong,
 * highlight. If correct, just print as normal. If everything is correct, return
 * OK. If some are wrong, return the number of wrong cells. Else, return the
 * appropriate error message.
 */
int check_answer(WINDOW *, const char *, const char *, const char *);

#endif
