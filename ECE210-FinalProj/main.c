//*****************************************************************************
// main.c
// Authors: Prayag Bhakar & Amanda Emerson
//*****************************************************************************

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// globals
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "ece210_api.h"
#include "lab_buttons.h"

//draw constants
#define Y_SPACING 17
#define X_SPACING 25
#define Y_MAX_DRAW 320
#define X_MAX_DRAW 240
#define BOARD_SIZE 9
//board states
#define SPACE 0
#define TIE 3
#define X 1
#define O 2


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// subfunctions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Draws the board
void drawBoard(int board[BOARD_SIZE][BOARD_SIZE]){
	for(int column = 0; column < BOARD_SIZE; column++){
		for(int row = 0; row < BOARD_SIZE; row++){
			//every third row has to be special beacuse it marks the end of a 
			//  board. This is why I have two diffrent blocks of prints
			if((row+1)%3 == 0){
				if(board[column][row] == SPACE){
					ece210_lcd_print_string("     ", 
						X_MAX_DRAW-X_SPACING*(row+0.5), 
						Y_MAX_DRAW-Y_SPACING*(2*column+1), 
						LCD_COLOR_WHITE, 
						LCD_COLOR_BLACK);
				} else if(board[column][row] == X) {
					ece210_lcd_print_string("X  ", 
						X_MAX_DRAW-X_SPACING*(row+0.5), 
						Y_MAX_DRAW-Y_SPACING*(2*column+1), 
						LCD_COLOR_BLUE2, 
						LCD_COLOR_BLACK);
				} else if(board[column][row] == O) {
					ece210_lcd_print_string("O  ", 
						X_MAX_DRAW-X_SPACING*(row+0.5), 
						Y_MAX_DRAW-Y_SPACING*(2*column+1), 
						LCD_COLOR_MAGENTA, 
						LCD_COLOR_BLACK);
				} else if(board[column][row] == TIE) {
					ece210_lcd_print_string(" ~  ", 
						X_MAX_DRAW-X_SPACING*(row+0.5), 
						Y_MAX_DRAW-Y_SPACING*(2*column+1), 
						LCD_COLOR_GREEN2, 
						LCD_COLOR_BLACK);
				}
			// regular draw. This includes dividers
			}else{
				if(board[column][row] == SPACE){
					ece210_lcd_print_string("    |", 
						X_MAX_DRAW-X_SPACING*(row+0.5), 
						Y_MAX_DRAW-Y_SPACING*(2*column+1), 
						LCD_COLOR_WHITE, 
						LCD_COLOR_BLACK);
				} else if(board[column][row] == X) {
					ece210_lcd_print_string("X |", 
						X_MAX_DRAW-X_SPACING*(row+0.5), 
						Y_MAX_DRAW-Y_SPACING*(2*column+1), 
						LCD_COLOR_BLUE2, 
						LCD_COLOR_BLACK);
				} else if(board[column][row] == O) {
					ece210_lcd_print_string("O |", 
						X_MAX_DRAW-X_SPACING*(row+0.5), 
						Y_MAX_DRAW-Y_SPACING*(2*column+1), 
						LCD_COLOR_MAGENTA, 
						LCD_COLOR_BLACK);
				} else if(board[column][row] == TIE) {
					ece210_lcd_print_string(" ~ |", 
						X_MAX_DRAW-X_SPACING*(row+0.5), 
						Y_MAX_DRAW-Y_SPACING*(2*column+1), 
						LCD_COLOR_GREEN2, 
						LCD_COLOR_BLACK);
				}
			}
		}
		for(int row = 0; row < BOARD_SIZE; row++){
			// simmilaly to the rows, every 3rd column also had to be special
			if((column+1)%3 == 0){
				ece210_lcd_print_string("    ", 
					X_MAX_DRAW-X_SPACING*(row+0.5), 
					Y_MAX_DRAW-Y_SPACING*(2*column+2), 
					LCD_COLOR_WHITE, 
					LCD_COLOR_BLACK);
			}else if((row+1)%3 == 0){
				ece210_lcd_print_string("-- ", 
					X_MAX_DRAW-X_SPACING*(row+0.5), 
					Y_MAX_DRAW-Y_SPACING*(2*column+2), 
					LCD_COLOR_WHITE, 
					LCD_COLOR_BLACK);
			}else{
				ece210_lcd_print_string("--+", 
					X_MAX_DRAW-X_SPACING*(row+0.5), 
					Y_MAX_DRAW-Y_SPACING*(2*column+2), 
					LCD_COLOR_WHITE, 
					LCD_COLOR_BLACK);
			}
		}
	}
}

// move cursor left/right. Requries a few pointers to be passed throught to
//	properly upate the cursor postion.
void update_x(int* x_cursor, 
	int* y_cursor, 
	int board[BOARD_SIZE][BOARD_SIZE], 
	bool* boardStateChanged, 
	int x_min, 
	int x_max, 
	int y_min, 
	int y_max){

	// get current x positon
	int x = ece210_ps2_read_x();
  
	// move left if possible
  if(x <= PS2_THRESHOLD_X_LO){
		// check for open spot in row
		for(int row = *x_cursor-1; row > x_min-1; row--){
			if(board[*y_cursor][row] == SPACE){
				*boardStateChanged = true;
				*x_cursor = row;
				return;
			}
		}
		
		//check for next open spot
		for(int column = *y_cursor-1; column > y_min-1; column--){
			for(int row = x_max; row > x_min-1; row--){
				if(board[column][row] == SPACE){
					*boardStateChanged = true;
					*x_cursor = row;
					*y_cursor = column;
					return;
				}
			}
		}
	// move right if possible
  }else if (x >= PS2_THRESHOLD_X_HI){
		// check for open spot in row
		for(int row = *x_cursor+1; row < x_max+1; row++){
			if(board[*y_cursor][row] == SPACE){
				*boardStateChanged = true;
				*x_cursor = row;
				return;
			}
		}
		
		//check for next open spot
		for(int column = *y_cursor+1; column < y_max+1; column++){
			for(int row = x_min; row < x_max+1; row++){
				if(board[column][row] == SPACE){
					*boardStateChanged = true;
					*x_cursor = row;
					*y_cursor = column;
					return;
				}
			}
		}
	}
}

// move cursor up/down. Requires a few pointers to be passed through to 
//	properly update the cursor positon.
void update_y(int* x_cursor, 
	int* y_cursor, 
	int board[BOARD_SIZE][BOARD_SIZE], 
	bool* boardStateChanged, 
	int *x_min, 
	int *x_max, 
	int *y_min, 
	int *y_max){

	//get current y positon
	int y = ece210_ps2_read_y();
  
	// down
	if(y >= PS2_THRESHOLD_Y_HI){
		//move down if possible
		for(int column = *y_cursor+1; column < y_max+1; column++){
			if(board[column][*x_cursor] == SPACE){
				*boardStateChanged = true;
				*y_cursor = column;
				return;
			}
		}
		
		//check for next open spot
		for(int row = *x_cursor+1; row < *x_max+1; row++){
			for(int column = *y_min; column < *y_max+1; column++){
				if(board[column][row] == SPACE){
					*boardStateChanged = true;
					*x_cursor = row;
					*y_cursor = column;
					return;
				}
			}
		}
		
	// up
	}else if (y <= PS2_THRESHOLD_Y_LO){
		//move up if possible
		for(int column = *y_cursor-1; column > y_min-1; column--){
			if(board[column][*x_cursor] == SPACE){
				*boardStateChanged = true;
				*y_cursor = column;
				return;
			}
		}
		
		//check for next open spot
		for(int row = *x_cursor-1; row > x_min-1; row--){
			for(int column = y_max; column > y_min-1; column--){
				if(board[column][row] == SPACE){
					*boardStateChanged = true;
					*x_cursor = row;
					*y_cursor = column;
					return;
				}
			}
		}
	}
}

// Clears the screen
void clearScreen(){
	//draws 16 empty lines to completly clear the screen
	for(int i = 0; i < 16; i++){
		ece210_lcd_add_msg("", TERMINAL_ALIGN_CENTER, LCD_COLOR_WHITE);
	}
}

// checks if a board is still played
// returns:
//	false - board is no longer being played
//	true - board is still playable
bool isStillPlayed(int board[BOARD_SIZE][BOARD_SIZE], 
	int x_min, 
	int x_max, 
	int y_min, 
	int y_max){

	//check if there is an open space in the defined parameters
	for(int column = y_min; column < y_max+1; column++){
		for(int row = x_min; row < x_max+1; row++){
			if(board[column][row] == SPACE)
				return true;
		}
	}
	return false;
}

// checks if a smaller board has a winner or not
// returns:
//	0 - no winner. Still being played
//	int - the value for the winner. Corresponds to the defined board states
int isBoardWin(int board[BOARD_SIZE][BOARD_SIZE], 
	int x_min, 
	int x_max, 
	int y_min, 
	int y_max){

	//check vertical wins
	for(int row = x_min; row < x_max+1; row++){
		if(board[y_min][row] != SPACE 
			&& board[y_min][row] == board[y_min+1][row] 
			&& board[y_min+1][row] == board[y_min+2][row]){
			return board[y_min][row];
		}
	}
	
	//check horizontal wins
	for(int column = y_min; column < y_max+1; column++){
		if(board[column][x_min] != SPACE 
			&& board[column][x_min] == board[column][x_min+1] 
			&& board[column][x_min+1] == board[column][x_min+2]){
			return board[column][x_min];
		}
	}
	
	//check diagonals
	if(board[y_min][x_min] != SPACE 
		&& board[y_min][x_min] == board[y_min+1][x_min+1] 
		&& board[y_min+1][x_min+1] == board[y_min+2][x_min+2]){
		return board[y_min][x_min];
	}
	if(board[y_min][x_max] != SPACE 
		&& board[y_min][x_max] == board[y_min+1][x_max-1] 
		&& board[y_min+1][x_max-1] == board[y_min+2][x_max-2]){
		return board[y_min][x_max];
	}
	
	//no win found
	return 0;
}

// checks to see if the general board is still being played.
// returns:
//	-1 - no win found
//	int - the value for the winner. Corresponds to the defined board states
int checkOverallBoard(int board[BOARD_SIZE][BOARD_SIZE]){

	//check vertical wins
	for(int row = 0; row < 3; row++){
		if(!isStillPlayed(board, row*3, row*3+2, 0, 8) 
			&& board[0][row*3] == board[3][row*3] 
			&& board[3][row*3] == board[6][row*3]){
			return board[0][row*3];
		}
	}
	
	//check horizontal wins
	for(int column = 0; column < 3; column++){
		if(!isStillPlayed(board, 0, 8, column*3, column*3+2) 
			&& board[column*3][0] == board[column*3][3] 
			&& board[column*3][3] == board[column*3][6]){
			return board[column*3][0];
		}
	}
	
	//check diagonals
	if(!isStillPlayed(board, 0, 2, 0, 2) 
		&& !isStillPlayed(board, 3, 5, 3, 5) 
		&& !isStillPlayed(board, 6, 8, 6, 8) 
		&& board[0][0] == board[3][3] 
		&& board[3][3] == board[6][6]){
		return board[0][0];
	}
	if(!isStillPlayed(board, 6, 8, 0, 2) 
		&& !isStillPlayed(board, 3, 5, 3, 5) 
		&& !isStillPlayed(board, 0, 2, 6, 8) 
		&& board[0][6] == board[3][3] 
		&& board[3][3] == board[6][0]){
		return board[0][6];
	}
	
	// no win found
	return -1;
}

// updates the board when the player picks a specific. moved to own subfucntion
//	to clean up the main function. Requires a lot of pointers
//
char update_select(bool *boardStateChanged,
	int *board[BOARD_SIZE][BOARD_SIZE], 
	int *playerTurn,
	int *x_min,
	int *x_max,
	int *y_min,
	int *y_max,
	){
	// Alert button toggle to only count one button press
	if(AlertButtons){
		AlertButtons = false;

		// delegate up button as selectior button
		if(btn_up_pressed()){
			// toggle draw check
			*boardStateChanged = true;
			
			// mark postion and swtich players
			if(*playerTurn){
				*board[y_cursor][x_cursor] = O;
				*playerTurn = 0;
			} else{
				*board[y_cursor][x_cursor] = X;
				*playerTurn = 1;
			}
			
			// resets limits if limits are at global board baised on cursor
			if(*x_min == 0 && *x_max == 8){
				if(x_cursor < 3){
					*x_min = 0;
					*x_max = 2;
				} else if(x_cursor < 6) {
					*x_min = 3;
					x_max = 5;
				} else {
					x_min = 6;
					x_max = 8;
				}
				
				if(y_cursor < 3){
					*y_min = 0;
					*y_max = 2;
				} else if(y_cursor <5){
					*y_min = 3;
					*y_max = 5;
				} else {
					*y_min = 6;
					*y_max = 8;
				}
			}
			
			// checks to set new board state
			if(isBoardWin(board, *x_min, x_max, *y_min, *y_max) != 0){
				//sets board as a win state
				for(int column = *y_min; column < *y_max+1; column++){
					for(int row = *x_min; row < *x_max+1; row++){
						board[column][row] = isBoardWin(board, 
							*x_min, *x_max, *y_min, *y_max);
					}
				}

			//check if the board is a tie
			} else if(!isStillPlayed(board, *x_min, *x_max, *y_min, *y_max)) {
				//sets board as a tie state
				for(int column = *y_min; column < *y_max+1; column++){
					for(int row = *x_min; row < *x_max+1; row++){
						board[column][row] = TIE;
					}
				}
				//checks the overall board
				
			}
			
			// checks the overall board and sets game to Over
			check = checkOverallBoard(*board);
			if(checkOverallBoard(*board) != -1){
				// change variables to match winner format
				winner = checkOverallBoard(*board)-1;
				gameState = 'O';
			//check overall Board tie and sets game to Over
			} else if(!isStillPlayed(*board, 0, 8, 0, 8)){
				winner = 2;
				gameState = 'O';

			// continiues on with the game normally
			} else {
				//set new limits
				*x_min=3*(x_cursor%3);
				*x_max=*x_min+2;
				*y_min=3*(y_cursor%3);
				*y_max=*y_min+2;
				
				// check of board is already played
				if(!isStillPlayed(board, *x_min, *x_max, *y_min, *y_max)){
					//set limits to global board
					*x_min=0;
					*x_max=8;
					*y_min=0;
					*y_max=8;
				}
				
				//set cursor to open positon
				for(int column = *y_min; column < *y_max+1; column++){
					for(int row = *x_min; row < *x_max+1; row++){
						if(board[column][row] == SPACE){
							y_cursor = column;
							x_cursor = row;
							break;
						}
					}
				}
			}
		}
	}
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main function
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main(void){
	// initialize Tiva C
	ece210_initialize_board();
	
// game varibales
//========================================
	int board[BOARD_SIZE][BOARD_SIZE];

	int playerTurn; 
	// states:
	//	0 = player 1 (X)
	//	1 = player 2 (O)

	int winner; 
	// states:
	//	-1 = none
	//	0 = player 1 (X)
	//	1 = player 2 (O)
	//	2 = tie

	bool boardStateChanged;

	char gameState = 'I';	
	// states:
	// 	I = Instructions
	//	P = Play
	//	O = game Over
	//	W = Wait for play signal

	// limits for what voard can be selected. Initally any postion is allowed
	int x_min;
	int x_max;
	int y_min;
	int y_max;

	int x_cursor;
	int y_cursor;
	

	while(1){
// Instructions
//============================================================================
		if(gameState == 'I'){
			// print all instructions
			ece210_lcd_add_msg("ULTIMATE TIC-TAC-TOE", 
				TERMINAL_ALIGN_CENTER, 
				LCD_COLOR_WHITE);
			ece210_lcd_add_msg("You are playing 9 games of Tic-Tac-Toe, each game being played like normal and then when one game is won, lost, or tied, it counts as an X or an O (or nothing in the case of a tie) in the bigger game of Tic-Tac-Toe. Player 1 (X) starts on a random smaller board Wherever player one places their \"X\" is where player 2 will have to play an \"O\" (so putting an X in the center of a board, means player 2's turn must be on the center board).", 
				TERMINAL_ALIGN_CENTER, 
				LCD_COLOR_WHITE);
			ece210_lcd_add_msg("Press down to play", 
				TERMINAL_ALIGN_CENTER, 
				LCD_COLOR_WHITE);

			// switch gamestates so instructiosn don't print out again
			gameState = 'W';

// Wait for play signal
//============================================================================
		}else if(gameState == 'W'){
			// AlertButton check to make sure only one button press is read
			if(AlertButtons){
				AlertButtons = false;

				// delegate up button for starting the game
				if(btn_up_pressed()){
					clearScreen();
					
					// reset game variables
					winner = -1;

					playerTurn = 0;

					boardStateChanged = true;

					x_min = 0;
					x_max = 8;
					y_min = 0;
					y_max = 8;

					x_cursor = 0;
					y_cursor = 0;

					// initialize board
					for (int column = 0; column < BOARD_SIZE; column++) {
						for (int row = 0; row < BOARD_SIZE; row++) {
								board[row][column] = SPACE;
						}
					}
					
					// start the game
					gameState = 'P';
				}

				// delegate down button for showing instructions again.
				if(btn_down_pressed()){
					clearScreen();
					gameState = 'I';
				}
			}

// Play
//============================================================================
		} else if(gameState == 'P'){
			// draw board and cursor
			if(boardStateChanged){
				drawBoard(board);

				// draw cursor baised on player's turn
				if(playerTurn){
					ece210_lcd_print_string("*", 
						X_MAX_DRAW-X_SPACING*(x_cursor+0.5), 
						Y_MAX_DRAW-Y_SPACING*(2*y_cursor+1), 
						LCD_COLOR_MAGENTA, 
						LCD_COLOR_BLACK);
				}else{
					ece210_lcd_print_string("*", 
						X_MAX_DRAW-X_SPACING*(x_cursor+0.5), 
						Y_MAX_DRAW-Y_SPACING*(2*y_cursor+1), 
						LCD_COLOR_BLUE2, 
						LCD_COLOR_BLACK);
				}

				// toggle draw check
				boardStateChanged = false;
			}
		
			// move cursor
			update_x(&x_cursor, 
				&y_cursor, 
				board, 
				&boardStateChanged, 
				x_min, 
				x_max, 
				y_min, 
				y_max);
			update_y(&x_cursor, 
				&y_cursor, 
				board, 
				&boardStateChanged, 
				x_min, 
				x_max, 
				y_min, 
				y_max);
			
			// slecting postion
			update_select();
		}

// game Over
//============================================================================
		} else if(gameState == 'O'){
			// print conditonal win message
			if(winner == 0){
				ece210_lcd_add_msg("X wins.", 
					TERMINAL_ALIGN_CENTER, 
					LCD_COLOR_WHITE);
			} else if (winner == 1){
				ece210_lcd_add_msg("O wins.", 
					TERMINAL_ALIGN_CENTER, 
					LCD_COLOR_WHITE);
			} else if (winner == 2){
				ece210_lcd_add_msg("It's a tie.", 
					TERMINAL_ALIGN_CENTER, 
					LCD_COLOR_WHITE);
			}

			// print 
			ece210_lcd_add_msg("Press down to play again", 
				TERMINAL_ALIGN_CENTER, 
				LCD_COLOR_WHITE);
			ece210_lcd_add_msg("Press up for instructions", 
				TERMINAL_ALIGN_CENTER, 
				LCD_COLOR_WHITE);
			gameState = 'W';
		}
	}
}