
#include "sudoku.h"

const char * SDK_MSGS[] = {
	"quit : q     ",
	"move : arrow ",
	"set  : 0 ~ 9 ",
	"del  : bckspc",
	"chck : enter ",
};

const char * SDK_MENU[] = {
	"EXPERT",
	"HARD",
	"MEDIUM",
	"EASY"
};

/**
 * Display the menu for the user to select difficulty.
 */
int init_game(diff_t * diff)
{
	int ret;
	WINDOW * menuw = NULL;

	/* Display menu to choose difficulty */
	if ((ret = create_menu(&menuw)) != OK) {
		return ret;
	}

	/* Disable cursor temporarily and handle user input */
	curs_set(0);
	ret = handle_menu(menuw, diff);
	curs_set(1);
	return ret;
}

/**
 * Handle user input in the menu window.
 */
int handle_menu(WINDOW * menuw, diff_t * diff)
{
	int ret;
	int size = sizeof(SDK_MENU) / sizeof(SDK_MENU[0]);
	int pos = size-1;
	*diff = dmap[pos];
	cprintw(menuw, 1+pos, SDK_MENU[pos], A_STANDOUT);

	for (;;) {
		int ch = wgetch(menuw);

		switch (ch) {
			case KEY_DOWN:
				if (pos < size-1) {
					cprintw(menuw, 1+pos, SDK_MENU[pos], A_NORMAL);
					*diff = dmap[++pos];
					cprintw(menuw, 1+pos, SDK_MENU[pos], A_STANDOUT);
				}
				break;

			case KEY_UP:
				if (pos > 0) {
					cprintw(menuw, 1+pos, SDK_MENU[pos], A_NORMAL);
					*diff = dmap[--pos];
					cprintw(menuw, 1+pos, SDK_MENU[pos], A_STANDOUT);
				}
				break;

			/* Quit game */
			case 'q':
				erasew(menuw);
				menuw = NULL;
				return EOF;

			/* Select difficulty and proceed */
			case KEY_ENTER:
			case '\n':
				erasew(menuw);
				menuw = NULL;
				return OK;

			/* Other unexpected input such as screen dim. change */
			default:
				erasew(menuw);
				menuw = NULL;
				if ((ret = create_menu(&menuw)) != OK) {
					return ret;
				}
				cprintw(menuw, 1+pos, SDK_MENU[pos], A_STANDOUT);
				break;
		}
	}

	return OK;
}

/**
 * Given a pointer to a window and the row index, print the string at the
 * center of the window.
 * If the string exceeds the window size, it will be clipped.
 * On success, return OK.
 */
void cprintw(WINDOW * w, int row, const char * str, int attr)
{
	if (w == NULL) {
		return;
	}
	int col = getmaxx(w);

	/* Erase the line first just in case there are leftovers*/
	for (int i = 0; i < col; i++) {
		mvwaddch(w, row, i, ' ');
	}

	int len = strnlen(str, col);

	/* Turn on attribute and print characters */
	wattron(w, attr);
	for (int i = 0; i < len; i++) {
		mvwaddch(w, row, 2+i+(col-len)/2, str[i]);
	}
	wattroff(w, attr);
	wrefresh(w);
}

/**
 * Erase contents of the window and free the window.
 */
void erasew(WINDOW * w)
{
	if (w == NULL) {
		return;
	}

	/* Overwrite with blanks */
	int width = getmaxx(w), height = getmaxy(w);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			mvwaddch(w, i, j, ' ');
		}
	}
	wrefresh(w);

	/* Unless the window is a parent window, it will be OK. */
	delwin(w);
}

/**
 * Create window for the menu to choose difficulty and display it in the
 * center of the screen.
 * diff_t : EXPERT, HARD, MEDIUM, EASY
 */
int create_menu(WINDOW ** menuw)
{
	int size = sizeof(SDK_MENU) / sizeof(SDK_MENU[0]);
	int y = (getmaxy(stdscr) - size - 1) / 2;
	if ((*menuw = newwin(size + 1, getmaxx(stdscr), y, 0)) == NULL) {
		return SDK_ERR_MENW;
	}

	/* Allow keypad input for arrow keys */
	if (keypad(*menuw, TRUE) != OK) {
		return SDK_ERR_MENK;
	}

	/* Print the difficulty options */
	cprintw(*menuw, 0, "-Difficulty-", A_NORMAL);
	for (int i = 0; i < size; i++) {
		cprintw(*menuw, i+1, SDK_MENU[i], A_NORMAL);
	}
	return OK;
}

/**
 * Load the sudoku file and start the game loop to handle inputs
 * and game logic.
 */
int start_game(diff_t diff)
{
	int ret;
	char grid[SDK_LEN*SDK_LEN];
	char orig[SDK_LEN*SDK_LEN];
	char answ[SDK_LEN*SDK_LEN];
	
	/* Load a sudoku file from assets */
	if ((ret = load_sdk(diff, grid)) != FIL_OK) {
		return ret + SDK_ERR_NUM;
	}
	memcpy(orig, grid, SDK_LEN * SDK_LEN);
	memcpy(answ, grid, SDK_LEN * SDK_LEN);

	/* Generate answer to the sudoku problem */
	if ((ret = solve(answ, 0)) != OK) {
		return SDK_ERR_SLVS;
	}

	WINDOW * gridw = NULL, * msgw = NULL;

	if ((ret = create_grid(&gridw)) != OK) {
		return ret;
	}
	if ((ret = create_msgw(&msgw)) != OK) {
		return ret;
	}

	/* Fill in the grid with default values in bold */
	fill_grid(gridw, grid, orig);

	return handle_grid(gridw, msgw, grid, orig, answ);
}

/**
 * Create window for the sudoku grid on the row.
 */
int create_grid(WINDOW ** gridw)
{
	int height = GRID_HEIGHT + GRID_BORDER;
	int width = GRID_WIDTH + GRID_BORDER;
	int starty = (getmaxy(stdscr) - height) / 2;
	int startx = (getmaxx(stdscr) - width) / 2;
	if ((*gridw = newwin(height, width, starty, startx)) == NULL) {
		return SDK_ERR_GRID;
	}

	/* Allow arrow and function keys to be inputted. */
	/* Drawing the box border for the grid.          */
	if (keypad(*gridw, TRUE) != OK || box(*gridw, 0, 0) != OK) {
		return SDK_ERR_GRDK;
	}

	/* Draw the vertical/horizontal lines */
	for (int i = 1; i <= GRID_HEIGHT; i++) {
		for (int j = 1; j <= GRID_WIDTH; j++) {
			if ((j % (CELL_WIDTH+1)) == 0) {
				mvwaddch(*gridw, i, j, ACS_VLINE);
			}
			if ((i % (CELL_HEIGHT+1)) == 0) {
				mvwaddch(*gridw, i, j, ACS_HLINE);
			}
			if (((j % (CELL_WIDTH+1)) | (i % (CELL_HEIGHT+1))) == 0) {
				mvwaddch(*gridw, i, j, ACS_ULCORNER | ACS_LRCORNER);
			}
		}
	}

	return OK;
}

/**
 * Create the message window and display SDK_MSGS on screen.
 */
int create_msgw(WINDOW ** msgw)
{
	int starty = GRID_BORDER + (getmaxy(stdscr) + GRID_HEIGHT) / 2;
	int rows = sizeof(SDK_MSGS) / sizeof(SDK_MSGS[0]);
	if ((*msgw = newwin(rows, getmaxx(stdscr), starty, 0)) == NULL) {
		return SDK_ERR_MSGW;
	}

	/* Print out the help messages */
	for (int i = 0; i < rows; i++) {
		cprintw(*msgw, i, SDK_MSGS[i], A_NORMAL);
	}

	return OK;
}

/**
 * Fill in the grid with the default values in bold.
 */
void fill_grid(WINDOW * gridw, const char * grid, const char * orig)
{
	/* Should not happen */
	if (gridw == NULL || grid == NULL || orig == NULL) {
		return;
	}

	for (int i = 0; i < SDK_LEN*SDK_LEN; i++) {
		if (grid[i] != '0') {
			/* Translate one dimensional array to a matrix */
			mvwaddch(gridw, TRANSY(i/SDK_LEN), TRANSX(i%SDK_LEN), grid[i]);
		}

		if (orig[i] != '0') {
			wattron(gridw, A_BOLD);
			mvwaddch(gridw, TRANSY(i/SDK_LEN), TRANSX(i%SDK_LEN), grid[i]);
			wattroff(gridw, A_BOLD);
		}
	}
}

/**
 * Handle user input in the grid window.
 */
int handle_grid(WINDOW * gridw, WINDOW * msgw, char * grid,
	const char * orig, const char * answ)
{
	int ret, ch;
	int index = 0;  /* current location in the grid */
	int done = ERR; /* Indicating if game solved    */

	for (;;) {
		wmove(gridw, TRANSY(index / SDK_LEN), TRANSX(index % SDK_LEN));
		ch = wgetch(gridw);

		/* Allow input/delete only if game is not finished */
		if (done != OK) {
			/* Number and not original */
			if (ch >= '1' && ch <= '9' && orig[index] == '0') {
				/* Update display */
				wattron(gridw, A_NORMAL);
				mvwaddch(gridw, TRANSY(index / SDK_LEN), TRANSX(index % SDK_LEN), ch);
				wattroff(gridw, A_NORMAL);
				/* Update data */
				grid[index] = ch;
				continue;
			}

			/* Delete or backspace or ascii DEL or ascii d */
			if (ch == KEY_BACKSPACE || ch == KEY_DC || ch == 127 || ch == 'd') {
				/* Do not delete default values */
				if (orig[index] != '0') {
					continue;
				}
				/* Update display */
				mvwaddch(gridw, TRANSY(index / SDK_LEN), TRANSX(index % SDK_LEN), ' ');
				/* Update data */
				grid[index] = '0';
				continue;
			}
		} /* if (done != OK) */

		/* Various other keys such as arrow keys, q, all others */
		switch (ch) {
			case KEY_LEFT:
				if ((index % SDK_LEN) > 0) {
					index--;
				}
				break;

			case KEY_RIGHT:
				if ((index % SDK_LEN) < SDK_LEN-1) {
					index++;
				}
				break;

			case KEY_UP:
				if ((index / SDK_LEN) > 0) {
					index -= SDK_LEN;
				}
				break;

			case KEY_DOWN:
				if ((index / SDK_LEN) < SDK_LEN-1) {
					index += SDK_LEN;
				}
				break;

			/* Quit game */
			case 'q':
				erasew(gridw);
				gridw = NULL;
				erasew(msgw);
				msgw = NULL;
				return OK;

			/* Submit */
			case KEY_ENTER:
			case '\n':
				done = check_grid(gridw, grid, orig, answ);
				/* If all correct, highlight all numbers and only allow quit */
				if (done == OK) {
					fill_grid(gridw, answ, answ);
				}
				break;

			default:
				erasew(gridw);
				gridw = NULL;
				if ((ret = create_grid(&gridw)) != OK) {
					return ret;
				}
				erasew(msgw);
				msgw = NULL;
				if ((ret = create_msgw(&msgw)) != OK) {
					return ret;
				}
				/* Update display and if done, display answer */
				fill_grid(gridw, grid, orig);
				if (done == OK) {
					fill_grid(gridw, answ, answ);
				}
				break;
		}
	}

	return OK;
}

/**
 * Check the validity of the sudoku by comparing it with the answer.
 * If wrong highlight. If correct, just print as normal.
 * If everything is correct, return OK.
 */
int check_grid(WINDOW * gridw, const char * grid,
	const char * orig, const char * answ)
{
	if (gridw == NULL || grid == NULL || orig == NULL) {
		return ERR;
	}

	/* Go over the grid and check with the answer */
	int allgood = OK;
	for (int i = 0; i < SDK_LEN * SDK_LEN; i++) {
		/* Skip if nothing there or an original number */
		if (orig[i] != '0') {
			continue;
		}
		if (grid[i] == '0') {
			allgood = ERR;
			continue;
		}

		if (grid[i] != answ[i]) {
			allgood = ERR;
		}

		/* If correct number in cell, remove any attributes if exists */
		/* If wrong number in cell, make it blink */
		wattron(gridw, grid[i] == answ[i] ? A_NORMAL : A_BLINK);
		mvwaddch(gridw, TRANSY(i / SDK_LEN), TRANSX(i % SDK_LEN), grid[i]);
		wattroff(gridw, grid[i] == answ[i] ? A_NORMAL : A_BLINK);
	}

	wrefresh(gridw);
	return allgood;
}

