#include "levels.h"
#include "common.h"
#include "tui.h"

#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

const char * SDK_LEVEL_STR[] = {
	"EXPERT",
	"HARD",
	"MEDIUM",
	"EASY"
};

const int SDK_EXPERT_01[] = {0x00, 0x00, 0x10, 0x09, 0x00, 0x00, 0x03, 0x60, 0x00, 0x00, 0x90, 0x00, 0x75, 0x00, 0x07, 0x04, 0x00, 0x00, 0x40, 0x00, 0x03, 0x08, 0x00, 0x16, 0x00, 0x03, 0x00, 0x04, 0x00, 0x60, 0x00, 0x12, 0x00, 0x00, 0x10, 0x00, 0x03, 0x00, 0x00, 0x80, 0x70};

const int * SDK_EXPERT[] = {
    SDK_EXPERT_01
};

const int SDK_HARD_01[] = {0x05, 0x00, 0x00, 0x00, 0x00, 0x03, 0x09, 0x00, 0x62, 0x00, 0x00, 0x60, 0x53, 0x89, 0x00, 0x00, 0x13, 0x40, 0x00, 0x00, 0x00, 0x70, 0x00, 0x30, 0x20, 0x00, 0x00, 0x00, 0x09, 0x05, 0x08, 0x05, 0x00, 0x00, 0x70, 0x00, 0x10, 0x96, 0x00, 0x00, 0x40};

const int * SDK_HARD[] = {
    SDK_HARD_01
};

const int SDK_MEDIUM_01[] = {0x00, 0x04, 0x07, 0x50, 0x00, 0x70, 0x00, 0x50, 0x02, 0x54, 0x06, 0x00, 0x00, 0x70, 0x00, 0x06, 0x02, 0x10, 0x62, 0x05, 0x98, 0x70, 0x00, 0x90, 0x00, 0x06, 0x00, 0x05, 0x00, 0x71, 0x90, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x80, 0x06, 0x00, 0x30};

const int * SDK_MEDIUM[] = {
    SDK_MEDIUM_01
};

const int SDK_EASY_01[] = {0x02, 0x63, 0x50, 0x00, 0x18, 0x01, 0x06, 0x00, 0x00, 0x00, 0x08, 0x19, 0x50, 0x63, 0x02, 0x00, 0x01, 0x05, 0x50, 0x09, 0x00, 0x00, 0x00, 0x90, 0x24, 0x08, 0x63, 0x00, 0x54, 0x97, 0x00, 0x00, 0x34, 0x12, 0x09, 0x50, 0x00, 0x05, 0x80, 0x07, 0x00};

const int * SDK_EASY[] = {
    SDK_EASY_01
};

const int ** SDK_LEVELS[] = {
    SDK_EXPERT,
    SDK_HARD,
    SDK_MEDIUM,
    SDK_EASY
};

int select_level(int * diff)
{
	int ret, size = sizeof(SDK_LEVEL_STR) / sizeof(SDK_LEVEL_STR[0]);

	/* Create menu window and draw the menu options */
	WINDOW * menuw = NULL;
	if ((ret = create_menu(&menuw, DIFF_TITLE, SDK_LEVEL_STR, size)) != OK) {
		return ret;
	}

    /* Disable cursor temporarily and handle user input */
	curs_set(0);
	ret = handle_menu(&menuw, DIFF_TITLE, SDK_LEVEL_STR, size, diff);
	curs_set(1);

    /* Cleanup */
	erasew(menuw);
	menuw = NULL;
	return ret;
}

void load_level(int lvl, char grid[])
{
    int number_of_lvls = 0;
    switch (lvl) {
    case DIFF_EXPRT:
        number_of_lvls = sizeof(SDK_EXPERT) / sizeof(SDK_EXPERT[0]);
        break;
    case DIFF_HARD:
        number_of_lvls = sizeof(SDK_HARD) / sizeof(SDK_HARD[0]);
        break;
    case DIFF_MEDM:
        number_of_lvls = sizeof(SDK_MEDIUM) / sizeof(SDK_MEDIUM[0]);
        break;
    case DIFF_EASY:
        number_of_lvls = sizeof(SDK_EASY) / sizeof(SDK_EASY[0]);
        break;
    default:
        return;
    }

    srand(time(NULL));
    int sublvl = rand() % number_of_lvls;

    for (int i = 0; i < SDK_SIZ / 2; i++) {
        grid[i * 2] = '0' + ((SDK_LEVELS[lvl][sublvl][i] & MASK_LEFT) >> 4);
        grid[i * 2 + 1] = '0' + (SDK_LEVELS[lvl][sublvl][i] & MASK_RIGHT);
    }
    grid[SDK_SIZ - 1] = '0' + ((SDK_LEVELS[lvl][0][SDK_SIZ / 2] & MASK_LEFT) >> 4); 
}