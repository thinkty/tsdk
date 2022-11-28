#ifndef __TSDK_SUDOKU_H
#define __TSDK_SUDOKU_H

#include "solve.h"

/**
 * Various error messages.
 */
extern const char * SDK_ERRS[];
#define SDK_ERR_NUM  (7) /* Number of error messages */
#define SDK_ERR_INIT (1)
#define SDK_ERR_MENW (2)
#define SDK_ERR_MENK (3)
#define SDK_ERR_GRID (4)
#define SDK_ERR_GRDK (5)
#define SDK_ERR_MSGW (6)
#define SDK_ERR_SLVS (7)

/**
 * Display internal messages to display window.
 */
extern const char * SDK_MSGS[]; 

/**
 * Display difficulty options
 */
extern const char * SDK_MENU[];

/**
 * Sudoku grid/cell dimension.
 * Padding is due to the character height being nearly double the width.
 */
#define CELL_PAD    (4)
#define CELL_WIDTH  (3+CELL_PAD)
#define CELL_HEIGHT (3)
#define GRID_WIDTH  (CELL_WIDTH*3+2)
#define GRID_HEIGHT (CELL_HEIGHT*3+2)
#define GRID_BORDER (2)

/**
 * Translate the following cursorx and cursory value to the actual
 * position in the sudoku grid.
 *
 * x  -> 0 1 2 3  4  5  6  7  8
 * tx -> 2 4 6 10 12 14 18 20 22
 *
 * y  -> 0 1 2 3 4 5 6  7  8
 * ty -> 2 3 4 6 7 8 10 11 12
 */
#define TRANSX(X) (1+(1+(X/3)*(CELL_WIDTH+1))+((X%3)*2))
#define TRANSY(Y) (Y+1+(Y/3))

/**
 * Display the menu for the user to select difficulty.
 */
int init_game(diff_t *);

/**
 * Handle user input in the menu window
 */
int handle_menu(WINDOW *, diff_t *);

/**
 * Load sudoku file and start the game.
 * It returns EOF to end the game, ERR on error, and OK to iterate the main
 * game loop again to select the difficulty.
 */
int start_game(diff_t);

/**
 * Given a pointer to a window and the row index, print the string at the
 * center of the window.
 * If the string is longer than the window, it will be clipped.
 * The third argument takes an attribute to apply to the text printed.
 * Returns ERR on error and OK if ok.
 */
void cprintw(WINDOW *, int, const char *, int);

/**
 * Erase contents of the window and free the window.
 */
void erasew(WINDOW *);

/**
 * Create the menu window and display in the center of the screen.
 */
int create_menu(WINDOW **);

/**
 * Create the sudoku grid window and display on the given row.
 */
int create_grid(WINDOW **);

/**
 * Create the message window and display SDK_MSGS on screen.
 */
int create_msgw(WINDOW **);

/**
 * Fill in the grid with the default values in bold.
 * First array contains the entire grid and the next array contains the
 * original sudoku grid.
 */
void fill_grid(WINDOW *, const char *, const char *);

/**
 * Handle user input in the grid window.
 */
int handle_grid(WINDOW *, WINDOW *, char *, const char *, const char *);

/**
 * Check the validity of the sudoku by comparing it with the answer.
 * If wrong highlight. If correct, just print as normal.
 * If everything is correct, return OK.
 */
int check_grid(WINDOW *, const char *, const char *, const char *);

#endif
