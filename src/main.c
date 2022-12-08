
#include "sudoku.h"

const char * usage =
	"\n\tUsage: sudoku"
	"\n\tHave fun."
	"\n";

/* FIL errors must come after SDK errors */
const char * SDK_ERRS[] = {

	/* SDK_ERR */
	"Error while initializing ncurses",
	"Error while creating menu window",
	"Error while setting keypad for menu window",
	"Error while creating new window for grid",
	"Error while enabling keyboard for grid window",
	"Error while creating message window",
	"Unable to solve sudoku problem",

	/* FIL_ERR */
	"Error while converting diff_t to string",
	"Error while allocating base path string",
	"Error while opening directory",
	"Error while reading directory: empty",
	"Error while reading directory: random",
	"Error while opening file",
	"Error while reading file: invalid char",
	"Error while reading file: invalid length"
};

void panic(int);
int init_curses();

/**
 * Entry point.
 */
int main(int argc, char ** argv)
{
	/* Check arguments */
	if (argc > 1) {
		printf("%s", usage);
		return ERR;
	}

	int ret;
	if ((ret = init_curses()) != OK) {
		panic(ret);
		return ERR;
	}

	/* Main game loop */
	diff_t diff;
	for (;;) {

		/* Let the user select difficulty*/
		ret = init_game(&diff);
		if (ret == EOF) {
			break;
		} else if (ret != OK) {
			panic(ret);
			return ERR;
		}

		/* Start sudoku with the selected difficulty */
		ret = start_game(diff);
		if (ret == EOF) {
			break;
		} else if (ret != OK) {
			panic(ret);
			return ERR;
		}
	}

	endwin();
	return OK;
}

/**
 * Start curses mode and initialize the following:
 * - disable line buffering
 * - disable echoing on getch
 * - enable keypad keys (F1, F2, arrow keys, etc..)
 */
int init_curses()
{
	initscr();

	if (cbreak() != OK || noecho() != OK || keypad(stdscr, TRUE) != OK) {
		return SDK_ERR_INIT;
	}

	return OK;
}

/**
 * Print the error message and return ERR.
 */
void panic(int erri)
{
	endwin();
	fprintf(stderr, "\nerror: %s\n\n", SDK_ERRS[erri-1]);
}

