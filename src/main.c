#include "common.h"
#include "sudoku.h"
#include "levels.h"
#include "tui.h"
#include "solve.h"

#include <string.h>

/**
 * Entry point.
 */
int main(void)
{
	int ret;
	if ((ret = tui_init()) != OK) {
		tui_panic(ret);
		return ERR;
	}

	/* Main game loop */
	int lvl;
	char grid[SDK_SIZ];
	for (;;) {

		/* Let the user select level */
		ret = select_level(&lvl);
		if (lvl == QUIT) {
			break;
		} else if (ret != OK) {
			tui_panic(ret);
			return ERR;
		}

		/* Load a random sudoku grid from that level */
		load_level(lvl, grid);

		/* Get an answer to the loaded sudoku */
		char answ[SDK_SIZ];
		memcpy(answ, grid, SDK_SIZ);
		if ((ret = solve(answ, 0)) != OK) {
			tui_panic(SDK_ERR_SLVS);
			return ERR;
		}

		/* Do sudoku */
		ret = start_game(grid, answ);
		if (ret == QUIT) {
			break;
		}
		if (ret != OK) {
			tui_panic(ret);
			return ERR;
		}
	}

	tui_end();
	return OK;
}
