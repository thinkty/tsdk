#include "sudoku.h"
#include "common.h"
#include "tui.h"

#include <string.h>

const char * SDK_HELP_MSGS[] = {
	"quit  : q     ",
	"move  : arrow ",
	"set   : 0 ~ 9 ",
	"del   : bckspc",
	"check : enter ",
};

int start_game(char grid[], char answ[])
{
	int ret;
	char orig[SDK_SIZ];
	memcpy(orig, grid, SDK_SIZ);

	WINDOW * gridw = NULL, * msgw = NULL;

	/* Display the 3x3 grid with borders */
	if ((ret = create_grid(&gridw)) != OK) {
		return ret;
	}

	/* Display the help messages */
	int help_msgw_size = sizeof(SDK_HELP_MSGS) / sizeof(SDK_HELP_MSGS[0]);
	if ((ret = create_msgw(&msgw, SDK_HELP_MSGS, help_msgw_size)) != OK) {
		return ret;
	}

	/* Fill in the grid with default values in bold */
	fill_grid(gridw, grid, orig);

	/* Start taking user input */
	return handle_grid(gridw, msgw, grid, orig, answ);
}

void fill_grid(WINDOW * gridw, const char * grid, const char * orig)
{
	for (int i = 0; i < SDK_LEN * SDK_LEN; i++) {
		if (grid[i] != '0') {
			putcwtrans(gridw, i, grid[i], A_NORMAL);
		}
		if (orig[i] != '0') {
			putcwtrans(gridw, i, grid[i], A_BOLD);
		}
	}
}

int handle_grid(WINDOW * gridw, WINDOW * msgw, char * grid, const char * orig, const char * answ)
{
	int ret, ch, wrong = 1; /* Set initial value of wrong answers as 1 */
	int index = 0;  /* current logical location in the grid */

	for (;;) {
		ch = getchtrans(gridw, index);

		/* Allow input/delete only if game is not done and not original */
		if (wrong != 0 && orig[index] == '0') {
			/* Fill in number */
			if (ch >= '1' && ch <= '9') {
				putcwtrans(gridw, index, ch, A_NORMAL);
				grid[index] = ch;
				continue;
			}

			/* Delete number */
			if (ch == KEY_BACKSPACE || ch == KEY_DC || ch == 127 || ch == 'd') {
				putcwtrans(gridw, index, ' ', A_NORMAL);
				grid[index] = '0';
				continue;
			}
		}

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
			return wrong == OK ? OK : QUIT;

		/* Submit */
		case KEY_ENTER:
		case '\n':
			wrong = check_answer(gridw, grid, orig, answ);
			if (wrong < 0) {
				return -wrong;
			}
			/* If all correct, highlight all numbers */
			if (wrong == 0) {
				fill_grid(gridw, answ, answ);
			}
			break;

		/* Unhandled key presses, just refresh */
		default:
			erasew(gridw);
			gridw = NULL;
			if ((ret = create_grid(&gridw)) != OK) {
				return ret;
			}
			erasew(msgw);
			msgw = NULL;
			int help_msgw_size = sizeof(SDK_HELP_MSGS) / sizeof(SDK_HELP_MSGS[0]);
			if ((ret = create_msgw(&msgw, SDK_HELP_MSGS, help_msgw_size)) != OK) {
				return ret;
			}
			
			/* If done, display answer */
			if (wrong == 0) {
				fill_grid(gridw, answ, answ);
			} else {
				fill_grid(gridw, grid, orig);
			}
			break;
		}
	}

	return OK;
}

int check_answer(WINDOW * w, const char * grid, const char * orig, const char * answ)
{
	if (w == NULL || grid == NULL || orig == NULL) {
		return -SDK_ERR_ARGS;
	}

	/* Go over the grid and check with the answer */
	int wrong = 0;
	for (int i = 0; i < SDK_LEN * SDK_LEN; i++) {
		/* Skip if it's an original number or empty */
		if (orig[i] != '0') {
			continue;
		}
		if (grid[i] == '0') {
			wrong++;
			continue;
		}

		/* Highlight if wrong answer */
		if (grid[i] != answ[i]) {
			wrong++;
			putcwtrans(w, i, grid[i], A_BLINK);
			continue;
		}

		putcwtrans(w, i, grid[i], A_NORMAL);
	}

	wrefresh(w);
	return wrong;
}
