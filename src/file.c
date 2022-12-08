
#include "file.h"

const char * FIL_DIFF_PATH[] = {
	"expert/",
	"hard/",
	"medium/",
	"easy/"
};
const diff_t dmap[] = {EXPERT, HARD, MEDIUM, EASY};

/**
 * A function to read a sudoku asset file (*.sdk) and fill in the grid.
 * Read a random file based on the difficulty.
 */
int load_sdk(diff_t diff, char * grid)
{
	int ret;
	FILE * file = NULL;
	if ((ret = open_sdk(diff, &file)) != FIL_OK) {
		return ret;
	}

	/* Fill in grid with contents of the file */
	for (int i = 0; i < SDK_LEN * SDK_LEN; i++) {
		grid[i] = fgetc(file);
		if (grid[i] == '\n') {
			i--;
			continue;
		}
		if (grid[i] > '9' || grid[i] < '0') {
			fclose(file);
			return FIL_ERR_IVLD;
		}
	}

	fclose(file);
	return FIL_OK;
}

/**
 * Open a random file in the given difficulty directory.
 */
int open_sdk(diff_t diff, FILE ** file)
{
	/* Translate diff_t to directory name string */
	const char * diffstr = FIL_DIFF_PATH[diff];

	/* Append the difficulty path to get complete appropriate */
	/* path to the difficulty directory.                      */
	size_t base_len = strlen(FIL_BASE);
	size_t diff_len = strlen(diffstr);
	char * path = malloc(base_len + diff_len + 1);
	if (path == NULL) {
		return FIL_ERR_MLLC;
	}
	memcpy(path, FIL_BASE, base_len);
	memcpy(path + base_len, diffstr, diff_len);
	path[base_len + diff_len] = '\0';

	/* Open directory */
	DIR * dir;
	if ((dir = opendir(path)) == NULL) {
		return FIL_ERR_OPND;
	}

	/* Sweep once to count the number of files in directory */
	struct dirent * dent;
	int count = 0;
	errno = 0;
	while ((dent = readdir(dir)) != NULL) {
		if (strcmp(dent->d_name, ".") == 0 ||
			strcmp(dent->d_name, "..") == 0 ||
			(dent->d_type != DT_REG && dent->d_type != DT_UNKNOWN)) {
			continue;
		}
		/* Assuming that there are only sudoku files */
		count++;
	}
	if (count == 0 || errno != 0) {
		return FIL_ERR_DMTY;
	}

	/* Get a random number between 1 and count */
	srand(time(NULL));
	count = (rand() % count) + 1;

	/* Get a random file from directory */
	int i = 0;
	rewinddir(dir);
	while (i < count && (dent = readdir(dir)) != NULL) {
		if (strcmp(dent->d_name, ".") == 0 ||
			strcmp(dent->d_name, "..") == 0 ||
			(dent->d_type != DT_REG && dent->d_type != DT_UNKNOWN)) {
			continue;
		}
		i++;
	}
	closedir(dir);
	if (errno != 0) {
		free(path);
		return FIL_ERR_DNTN;
	}

	/* Append the file name to path */
	size_t path_len = strlen(path);
	size_t name_len = strlen(dent->d_name);
	path = realloc(path, path_len + name_len + 1);
	if (path == NULL) {
		return FIL_ERR_MLLC;
	}
	memcpy(path + path_len, dent->d_name, name_len + 1);

	/* Open file */
	*file = fopen(path, "r");
	free(path);
	if (*file == NULL) {
		return FIL_ERR_OPNF;
	}
	return FIL_OK;
}

