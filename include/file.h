#ifndef __TSDK_FILE_H
#define __TSDK_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <ncurses.h>

#define SDK_LEN (9)  /* Length of the logical sudoku grid. */
#define FIL_OK  (0)  /* Successfully loaded sudoku file.   */

/* Make sure to include error messages in src/main.c */
#define FIL_ERR_DTOA (1) /* Error while translating diff_t to dir name. */
#define FIL_ERR_MLLC (2) /* Error while allocating path for directory.  */
#define FIL_ERR_OPND (3) /* Error while opening directory.              */
#define FIL_ERR_DMTY (4) /* Error while reading directory: empty.       */
#define FIL_ERR_DNTN (5) /* Error while reading directory: random.      */
#define FIL_ERR_OPNF (6) /* Error while opening file.                   */
#define FIL_ERR_IVLD (7) /* Error while reading file: invalid char.     */
#define FIL_ERR_WRNL (8) /* Error while reading file: invalid length.   */

// TODO: this is based on cwd (relative)
// TODO: how do they handle path in other c programs?
// TODO: does it depend on package manager?
#define FIL_BASE "./sdk/" /* Directory that stores sudoku files by diff. */
extern const char * FIL_DIFF_PATH[];

/**
 * Difficulty of the game.
 */
typedef enum {
	EXPERT,
	HARD,
	MEDIUM,
	EASY
} diff_t;

extern const diff_t dmap[];

/**
 * A function to read a sudoku asset file (*.sdk) and fill in the grid.
 * Read a random file based on the difficulty.
 */
int load_sdk(diff_t, char *);

/**
 * Open a random file in the given difficulty directory.
 */
int open_sdk(diff_t, FILE **);

#endif
