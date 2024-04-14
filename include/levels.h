#ifndef __SDK_LEVELS_H
#define __SDK_LEVELS_H

#define DIFF_EXPRT (0)
#define DIFF_HARD  (1)
#define DIFF_MEDM  (2)
#define DIFF_EASY  (3)
#define DIFF_TITLE "-Difficulty-"
#define MASK_LEFT  (0xF0)
#define MASK_RIGHT (0x0F)

extern const char * SDK_LEVEL_STR[];

/**
 * Display the level menu for the user to select level and handle user input.
 */
int select_level(int *);

/**
 * Given a difficulty level, load a random sudoku grid from the level.
 */
void load_level(int, char []);

#endif