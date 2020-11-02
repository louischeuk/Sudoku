#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <ctime>
#include "sudoku.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* pre-supplied function to load a Sudoku board from a file */
void load_board(const char* filename, char board[9][9]) {

	cout << "Loading Sudoku board from file '" << filename << "'... ";

	ifstream in(filename);
	if (!in) {
		cout << "Failed!" << '\n';
	}
	assert(in);

	char buffer[512];

	int row = 0;
	in.getline(buffer,512);
	while (in && row < 9) {
		for (int n=0; n<9; n++) {
			assert(buffer[n] == '.' || isdigit(buffer[n]));
			board[row][n] = buffer[n];
		}
		row++;
		in.getline(buffer,512);
	}

	cout << ((row == 9) ? "Success!" : "Failed!") << '\n';
	assert(row == 9);
}

/* internal helper function */
void print_frame(int row) {
	if (!(row % 3)) {
		cout << "  +===========+===========+===========+" << '\n';
	} else {
		cout << "  +---+---+---+---+---+---+---+---+---+" << '\n';
	}
}

/* internal helper function */
void print_row(const char* data, int row) {
	cout << (char) ('A' + row) << " ";
	for (int i=0; i<9; i++) {
		cout << ( (i % 3) ? ':' : '|' ) << " ";
		cout << ( (data[i]=='.') ? ' ' : data[i]) << " ";
	}
	cout << "|" << '\n';
}

/* pre-supplied function to display a Sudoku board */
void display_board(const char board[9][9]) {
	cout << "    ";
	for (int r=0; r<9; r++) {
		cout << (char) ('1'+r) << "   ";
	}
	cout << '\n';
	for (int r=0; r<9; r++) {
		print_frame(r);
		print_row(board[r],r);
	}
	print_frame(9);
}

/* add your functions here */

/* function that returns TRUE if the sudoku is all board positions are occupied by digits, and FALSE otherwise */
bool is_complete(char board[9][9]) {
	for (int r=0; r<9; r++) {
		for (int c=0; c<9; c++) {
			if ( board[r][c] < 49 || board[r][c] > 57 )
				return false;
		}
	}
	return true;
}

/* function that return TRUE if the digit is valid to be put at the given position, and FALSE if not valid */
bool make_move(const char position[2], char digit, char board[9][9]) {

	// check if the position is out of range of the Sudoku board
	if ( position[0]<65 || position[0]>73 || position[1]<49 || position[1] > 57 )
        return false;
    
    // check if digit input is valid
    if ( digit < 49 || digit > 57 )
        return false;

    // initialise the position in integer
    int pos_0 = position[0]-65;
    int pos_1 = position[1]-49;
    
    // existed digit should not be replaced
    if (board[pos_0][pos_1] != '.')
        return false;
    
    for (int i=0 ; i<9 ; i++) {
        // check if a copy of the digit exists at the same column (exclude given position)
		if ( board[i][pos_1] == digit && i != pos_0 )
			return false;
        // check if a copy of the digit exists at the same row (exclude given position)
        if ( board[pos_0][i] == digit && i != pos_1 )
            return false;
    }

    // initialise the positions of the 3x3 board to be checked at given position
	int box_row = pos_0 - (pos_0 % 3);
	int box_col = pos_1 - (pos_1 % 3);
    
    // check if a copy of the digit exists within the same 3x3 board (exclude given psoition)
	for (int r = box_row ; r < box_row+3 ; r++)
		for (int c = box_col ; c < box_col+3 ; c++)
			if ( board[r][c] == digit && ( r != pos_0 || c != pos_1 ) )
				return false;
    
	// if none of the above condition matches, the move can be made
	board[pos_0][pos_1] = digit;
	return true;
}

/* function that outputs the 2D-array board to a file with name filename */
bool save_board(const char *filename, char board[9][9]) {
    
    ofstream out_stream;
    out_stream.open(filename);
    
    // check if the board exists, filename can be opened or it fails for other reasons
    try {
        if (board == NULL)
            throw "there is no board!";
        if ( out_stream.fail() )
            throw "outstream fails!";
    } catch(const char* msg) {
        cerr << msg << endl;
        return false;
    }

    for (int r=0; r<9; r++) {
        for (int c=0; c<9; c++) {
                    out_stream << board[r][c];
        }
        out_stream << endl;
    }
    out_stream.close();
    return true;
}


/* function that returns TRUE if the Sudoku can be solved, and FALSE otherwise */
bool solve_board(char board[9][9]) {
    
    //first, check if the given board is valid
    if ( !is_given_board_valid(board) )
        return false;
    
    // calcuate the amonut of clock ticks and processing time of solving each puzzles
    clock_t t;
    t = clock(); // start recording the clock ticks
    
    // result of whether the board can be solved is stored in a variable for later use
    bool is_board_solvable = backtracking(board);
    
    t = clock() - t; // difference of the number of clock ticks between start and end
    cout << "Number of clock ticks of this board is " << t;
    cout << " (" <<  static_cast<float>(t) / CLOCKS_PER_SEC; // processing time
    cout << " seconds)."<< '\n' << '\n';
    
    // if the board can be solved, return TRUE else FALSE
    return is_board_solvable ? true : false;
}

/* helper function of solve_board() that checks if the whole given board is valid */
bool is_given_board_valid(char board[9][9]) {
    for (int row = 0; row < 9; row++) {
        for (int column = 0; column < 9; column ++) {
            if (board[row][column] != '.' ) {
                char pos[2] = { static_cast<char>(row+65), static_cast<char>(column+49) };
                // check if the given digit in the board is valid
                if ( !is_existing_digit_valid(pos, board[row][column], board) ) {
                    return false;
                }
            }
        }
    }
    return true;
}

/* helper function of helper function of solve_board() that checks if the existed digits are valid */
bool is_existing_digit_valid(const char position[2], char digit, char board[9][9]) {

    // check if the position is out of range of the Sudoku board: ie. <A or >I or <1 or >9
    if ( position[0]<65 || position[0]>73 || position[1]<49 || position[1] > 57 )
        return false;
    
    // check if digit input is <1 or >9
    if ( digit < 49 || digit > 57 )
        return false;

    // cast the variable position from char to int
    int pos_0 = position[0]-65;
    int pos_1 = position[1]-49;
    
    for (int i=0 ; i<9 ; i++) {
        // check if a copy of the digit exists at the same column (exclude given position)
        if ( board[i][pos_1] == digit && i != pos_0 )
            return false;
        // check if a copy of the digit exists at the same row (exclude given position)
        if ( board[pos_0][i] == digit && i != pos_1 )
            return false;
    }

    // initialise the position of the 3x3 board to be checked at given position
    int box_row = pos_0 - (pos_0 % 3);
    int box_col = pos_1 - (pos_1 % 3);
    
    // check if a copy of the digit exists within the same 3x3 board (exclude given psoition)
    for (int r = box_row ; r < box_row+3 ; r++)
        for (int c = box_col ; c < box_col+3 ; c++)
            if ( board[r][c] == digit && ( r != pos_0 || c != pos_1 ) )
                return false;
    
    // if none of the above condition matches, the existing digit is valid
    return true;
}

/* helper function of solve_board() that solves the Sudoku */
bool backtracking(char board[9][9], int row, int col) {
    /* two default parameters: row and col which corrosponding the row and col iterators of the whole board */
    
    // if the column iterator reaches outside the range of the board (col == 9),
    // move row iterator to the next row (row++) and reset column iterator (col=0)
    if (col == 9) {
        row++;
        col=0;
    }

    // when reaches beyond the last element, the board is solved and completed
    if (row == 9)
        return true;

    // keep the element unchanged if the element is not empty, and move to the next element
    if ( board[row][col] != '.' )
        return backtracking(board, row, col+1);

    // consider digits from 1 to 9
    for (int digit = 1 ; digit <=9 ; digit++) {
        // update the position accordingly
        char position[2] = { static_cast<char>(row+65), static_cast<char>(col+49) };
        // check if the digit is valid to be put in the Sudoku
        if ( make_move(position, digit+48, board) ) {
            // go to the next element and continue recursively
            if( backtracking(board, row, col+1) ) {
                return true;
            }
        }
        // reset all elements if the board cannot be solved
        board[row][col] = '.';
    }
    return false;
}
