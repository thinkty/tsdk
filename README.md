[![asciicast](https://asciinema.org/a/sfO3LFRV7c633hb33PNYm0rmU.svg)](https://asciinema.org/a/sfO3LFRV7c633hb33PNYm0rmU)

## TSDK - Terminal SuDoKu

A simple sudoku application for your terminal using the [ncurses](https://linux.die.net/man/3/ncurses) library.

Compile using `make` and an executable named `sudoku` will appear.
So far, I've only compiled on Linux.
I might add more assets and perhaps share on [AUR](https://wiki.archlinux.org/title/AUR_submission_guidelines), but I've never done it before so I think it will be a great opportunity for me to learn!

The sudoku asset files are in the `sdk/` directory, divided by difficulties `EASY`, `MEDIUM`, `HARD`, and `EXPERT`.
Each asset file must be atleast 81 bytes long as the game will randomly select a file in the directory and read the first 81 bytes of characters.
Inside the asset file, the empty cells are denoted by the `0` character.

After the difficulty has been chosen and the asset file is successfully loaded, the program will calculate an answer for the sudoku puzzle by brute-force using backtracking to try every number in each empty cell.
If an answer does not exist, it will exit and print that the answer does not exist.

When the sudoku game is loaded and solved, the player can move around using the `arrow keys` and place a number using the `keypad`.
Erase cells using `d` or `delete`.
Check the answer using `enter` and the incorrect cells will be blink or highlight.
When everything is correct or the player just wants to quit, exit using the `q` key.
Press `q` again to exit from the difficulty selection window and close the game.

