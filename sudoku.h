#ifndef SUDOKU_H
#define SUDOKU_H

/* function declaration that loads a Sudoku board from a file*/
void load_board(const char* filename, char board[9][9]);

/* function declaration that display a Sudoku board */
void display_board(const char board[9][9]);

/* ---------- function declarations above are pre-supplied  ------------- */

/* function declaration that return TRUE if all board positiions are occupied by digits, and false otherwise */
bool is_complete(char board[9][9]);

/* function declaration that returns TRUE if the digit is valid to be put at the given position, and FALSE otherwise */
bool make_move(const char position[2], char digit, char board[9][9]);

/* function declaration that outputs the 2D array board to a file with name filename */
bool save_board(const char *filename, char board[9][9]);

/* function declaration of solve_board() that returns TRUE if the Sudoku can be solved, and FALSE otherwise */
bool solve_board(char board[9][9]);

/* function declaration of is_given_board_valid() that checks if the whole given board is valid */
bool is_given_board_valid(char board[9][9]);

/* function declaration of is_existing_digit_valid() that checks if the existed digits are valid */
bool is_existing_digit_valid(const char position[2], char digit, char board[9][9]);

/* function declaration of backtracking() with two default parameters initialised */
bool backtracking(char board[9][9], int row = 0, int col = 0);

#endif
