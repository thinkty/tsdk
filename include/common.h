#ifndef __TSDK_COMMON_H
#define __TSDK_COMMON_H

#include <stdio.h>

#define eprintf(fmt, ...) \
	do { fprintf(stderr, fmt "\n" __VA_OPT__(,) __VA_ARGS__); } while (0)

#define OK    (0)
#define ERR   (-1)
#define QUIT  (-2)

#define SDK_LEN (9)  /* Length of the sudoku board */
#define SDK_SIZ (81) /* Size of the sudoku board */

#define SDK_ERR_NUM  (8) /* Number of error messages */
#define SDK_ERR_INIT (1) /* 0 is reserved to OK */
#define SDK_ERR_MENW (2)
#define SDK_ERR_MENK (3)
#define SDK_ERR_GRID (4)
#define SDK_ERR_GRDK (5)
#define SDK_ERR_MSGW (6)
#define SDK_ERR_SLVS (7)
#define SDK_ERR_ARGS (8)

extern const char * SDK_ERR_MSGS[];

#endif

