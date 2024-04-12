#include "tui.h"
#include "common.h"

#include <string.h>
#include <ncurses.h>

int tui_init()
{
	initscr();

	if (cbreak() != OK || noecho() != OK || keypad(stdscr, TRUE) != OK) {
		return SDK_ERR_INIT;
	}
	return OK;
}

void tui_end()
{
	endwin();
}

void tui_panic(int erri)
{
	endwin();
	eprintf("error: %s", SDK_ERR_MSGS[erri-1]);
}

int getchtrans(WINDOW * w, int i)
{
	wmove(w, TRANSY(i / SDK_LEN), TRANSX(i % SDK_LEN));
	return wgetch(w);
}

void cprintw(WINDOW * w, int row, const char * str, int attr)
{
	if (w == NULL || str == NULL) {
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

void putcwtrans(WINDOW * w, int i, char c, attr_t attr)
{
	if (w == NULL) {
		return;
	}

	wattron(w, attr);
	mvwaddch(w, TRANSY(i / SDK_LEN), TRANSX(i % SDK_LEN), c);
	wattroff(w, attr);
}

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

int create_menu(WINDOW ** w, const char * title, const char * options[], int size)
{
	int center = (getmaxy(stdscr) - size - 1) / 2;
	if ((*w = newwin(size + 1, getmaxx(stdscr), center, 0)) == NULL) {
		return SDK_ERR_MENW;
	}

	/* Allow keypad input for arrow keys */
	if (keypad(*w, TRUE) != OK) {
		return SDK_ERR_MENK;
	}

	/* Print the title and options */
	cprintw(*w, 0, title, A_NORMAL);
	for (int i = 0; i < size; i++) {
		cprintw(*w, i + MENU_PAD, options[i], A_NORMAL);
	}
	return OK;
}

int handle_menu(WINDOW ** w, const char * title, const char * options[], int size, int * option)
{
	int ret, i = size - 1; /* Use the last option as default */

	for (;;) {
		cprintw(*w, i + MENU_PAD, options[i], A_STANDOUT);
		int ch = wgetch(*w);
		cprintw(*w, i + MENU_PAD, options[i], A_NORMAL);

		switch (ch) {
		case KEY_DOWN:
			/* Go down if not at the last option */
			if (i < size - 1) {
				i++;
			}
			break;
		case KEY_UP:
			/* Go up if not at the first option */
			if (i > 0) {
				i--;
			}
			break;
		case 'q':
			*option = QUIT;
			return OK;
		case KEY_ENTER:
		case '\n':
			*option = i;
			return OK;
		default:
			/* Other unexpected input such as screen size change */
			erasew(*w);
			w = NULL;
			if ((ret = create_menu(w, title, options, size)) != OK) {
				return ret;
			}
			break;
		}
	}

	return OK;
}

int create_grid(WINDOW ** w)
{
	int height = GRID_HEIGHT + GRID_BORDER;
	int width = GRID_WIDTH + GRID_BORDER;
	int starty = (getmaxy(stdscr) - height) / 2;
	int startx = (getmaxx(stdscr) - width) / 2;
	if ((*w = newwin(height, width, starty, startx)) == NULL) {
		return SDK_ERR_GRID;
	}

	/* Allow arrow and function keys to be inputted. */
	/* Drawing the box border for the grid.          */
	if (keypad(*w, TRUE) != OK || box(*w, 0, 0) != OK) {
		return SDK_ERR_GRDK;
	}

	/* Draw the vertical/horizontal lines */
	for (int i = 1; i <= GRID_HEIGHT; i++) {
		for (int j = 1; j <= GRID_WIDTH; j++) {
			if ((j % (CELL_WIDTH+1)) == 0) {
				mvwaddch(*w, i, j, ACS_VLINE);
			}
			if ((i % (CELL_HEIGHT+1)) == 0) {
				mvwaddch(*w, i, j, ACS_HLINE);
			}
			if (((j % (CELL_WIDTH+1)) | (i % (CELL_HEIGHT+1))) == 0) {
				mvwaddch(*w, i, j, ACS_ULCORNER | ACS_LRCORNER);
			}
		}
	}

	return OK;
}

int create_msgw(WINDOW ** msgw, const char * msgs[], int size)
{
	/* Create the window right below the grid */
	int starty = GRID_BORDER + (getmaxy(stdscr) + GRID_HEIGHT) / 2;
	if ((*msgw = newwin(size, getmaxx(stdscr), starty, 0)) == NULL) {
		return SDK_ERR_MSGW;
	}

	/* Print out the messages */
	for (int i = 0; i < size; i++) {
		cprintw(*msgw, i, msgs[i], A_NORMAL);
	}

	return OK;
}
