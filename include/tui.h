#ifndef __TSDK_TUI_H
#define __TSDK_TUI_H

#include <ncurses.h>

#define MENU_PAD    (1)

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
 * Start curses mode and disable line buffering, disable echoing on getch,
 * enable keypad keys (F1, F2, arrow keys, etc..)
 */
int tui_init();

/**
 * Just an endwin() wrapper.
 */
void tui_end();

/**
 * Print the error message, and end curses mode.
 */
void tui_panic(int);

/**
 * Given a pointer to the grid window and the index (position) of the cursor,
 * move the cursor and get user input.
 */
int getchtrans(WINDOW *, int);

/**
 * Given a pointer to a window and the row index, print the string at the
 * center of the window. If the string is longer than the window, it will be
 * clipped. The third argument takes an attribute to apply to the text printed.
 */
void cprintw(WINDOW *, int, const char *, int);

/**
 * Given a pointer to the grid window and the index (position) of the cursor,
 * print the given character at the appropriate row and column. The last
 * argument is the attribute to apply when printing.
 */
void putcwtrans(WINDOW *, int, char, unsigned int);

/**
 * Erase contents of the window and free the window.
 */
void erasew(WINDOW *);

/**
 * Create a window and display in the center of the screen.
 */
int create_menu(WINDOW **, const char *, const char * [], int);

/**
 * Handle user input in the menu window. If the user selects an option from the
 * menu, return OK and set the given option. If the user quits, return QUIT. On
 * error, return a positive error number.
 */
int handle_menu(WINDOW **, const char *, const char * [], int, int *);

/**
 * Create a window and display the 3x3 cells with borders in the center of the
 * screen.
 */
int create_grid(WINDOW **);

/**
 * Create the message window below the grid and display the given messages on
 * screen. Returns OK on success.
 */
int create_msgw(WINDOW **, const char * [], int);

#endif