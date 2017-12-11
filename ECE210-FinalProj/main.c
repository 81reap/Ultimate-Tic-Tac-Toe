//*****************************************************************************
// main.c
// Author: Prayag Bhakar & Amanda Emerson
//*****************************************************************************
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

//draw board
void drawBoard(int board[BOARD_SIZE][BOARD_SIZE]){
	for(int column = 0; column < BOARD_SIZE; column++){
		for(int row = 0; row < BOARD_SIZE; row++){
			if((row+1)%3 == 0){
				if(board[column][row] == SPACE){
					ece210_lcd_print_string("     ", X_MAX_DRAW-X_SPACING*(row+0.5), Y_MAX_DRAW-Y_SPACING*(2*column+1), LCD_COLOR_WHITE, LCD_COLOR_BLACK);
				} else if(board[column][row] == X) {
					ece210_lcd_print_string("X  ", X_MAX_DRAW-X_SPACING*(row+0.5), Y_MAX_DRAW-Y_SPACING*(2*column+1), LCD_COLOR_BLUE2, LCD_COLOR_BLACK);
				} else if(board[column][row] == O) {
					ece210_lcd_print_string("O  ", X_MAX_DRAW-X_SPACING*(row+0.5), Y_MAX_DRAW-Y_SPACING*(2*column+1), LCD_COLOR_MAGENTA, LCD_COLOR_BLACK);
				} else if(board[column][row] == TIE) {
					ece210_lcd_print_string(" ~  ", X_MAX_DRAW-X_SPACING*(row+0.5), Y_MAX_DRAW-Y_SPACING*(2*column+1), LCD_COLOR_GREEN2, LCD_COLOR_BLACK);
				}
			}else{
				if(board[column][row] == SPACE){
					ece210_lcd_print_string("    |", X_MAX_DRAW-X_SPACING*(row+0.5), Y_MAX_DRAW-Y_SPACING*(2*column+1), LCD_COLOR_WHITE, LCD_COLOR_BLACK);
				} else if(board[column][row] == X) {
					ece210_lcd_print_string("X |", X_MAX_DRAW-X_SPACING*(row+0.5), Y_MAX_DRAW-Y_SPACING*(2*column+1), LCD_COLOR_BLUE2, LCD_COLOR_BLACK);
				} else if(board[column][row] == O) {
					ece210_lcd_print_string("O |", X_MAX_DRAW-X_SPACING*(row+0.5), Y_MAX_DRAW-Y_SPACING*(2*column+1), LCD_COLOR_MAGENTA, LCD_COLOR_BLACK);
				} else if(board[column][row] == TIE) {
					ece210_lcd_print_string(" ~ |", X_MAX_DRAW-X_SPACING*(row+0.5), Y_MAX_DRAW-Y_SPACING*(2*column+1), LCD_COLOR_GREEN2, LCD_COLOR_BLACK);
				}
			}
		}
		for(int row = 0; row < BOARD_SIZE; row++){
			if((column+1)%3 == 0){
				ece210_lcd_print_string("    ", X_MAX_DRAW-X_SPACING*(row+0.5), Y_MAX_DRAW-Y_SPACING*(2*column+2), LCD_COLOR_WHITE, LCD_COLOR_BLACK);
			}else if((row+1)%3 == 0){
				ece210_lcd_print_string("-- ", X_MAX_DRAW-X_SPACING*(row+0.5), Y_MAX_DRAW-Y_SPACING*(2*column+2), LCD_COLOR_WHITE, LCD_COLOR_BLACK);
			}else{
				ece210_lcd_print_string("--+", X_MAX_DRAW-X_SPACING*(row+0.5), Y_MAX_DRAW-Y_SPACING*(2*column+2), LCD_COLOR_WHITE, LCD_COLOR_BLACK);
			}
		}
	}
}

//move cursor left/right
void update_x(int* x_cursor, int* y_cursor, int board[BOARD_SIZE][BOARD_SIZE], bool* boardStateChanged, int x_min, int x_max, int y_min, int y_max){
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

//move cursor up/down
void update_y(int* x_cursor, int* y_cursor, int board[BOARD_SIZE][BOARD_SIZE], bool* boardStateChanged, int x_min, int x_max, int y_min, int y_max){
	int y = ece210_ps2_read_y();
  
	// move down if possible
  if(y >= PS2_THRESHOLD_Y_HI){
    for(int column = *y_cursor+1; column < y_max+1; column++){
			if(board[column][*x_cursor] == SPACE){
				*boardStateChanged = true;
				*y_cursor = column;
				return;
			}
		}
		
		//check for next open spot
		for(int row = *x_cursor+1; row < x_max+1; row++){
			for(int column = y_min; column < y_max+1; column++){
				if(board[column][row] == SPACE){
					*boardStateChanged = true;
					*x_cursor = row;
					*y_cursor = column;
					return;
				}
			}
		}
		
	// move up if possible
  }else if (y <= PS2_THRESHOLD_Y_LO){
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

//empty the screen
void clearScreen(){
	for(int i = 0; i < 16; i++){
		ece210_lcd_add_msg("", TERMINAL_ALIGN_CENTER, LCD_COLOR_WHITE);
	}
}

bool isStillPlayed(int board[BOARD_SIZE][BOARD_SIZE], int x_min, int x_max, int y_min, int y_max){
	for(int column = y_min; column < y_max+1; column++){
		for(int row = x_min; row < x_max+1; row++){
			if(board[column][row] == SPACE)
				return true;
		}
	}
	return false;
}

int isBoardWin(int board[BOARD_SIZE][BOARD_SIZE], int x_min, int x_max, int y_min, int y_max){
	//Chck vertical wins
	for(int row = x_min; row < x_max+1; row++){
		if(board[y_min][row] != SPACE && board[y_min][row] == board[y_min+1][row] && board[y_min+1][row] == board[y_min+2][row]){
			return board[y_min][row];
		}
	}
	
	//check horizontal wins
	for(int column = y_min; column < y_max+1; column++){
		if(board[column][x_min] != SPACE && board[column][x_min] == board[column][x_min+1] && board[column][x_min+1] == board[column][x_min+2]){
			return board[column][x_min];
		}
	}
	
	//check diagonals
	if(board[y_min][x_min] != SPACE && board[y_min][x_min] == board[y_min+1][x_min+1] && board[y_min+1][x_min+1] == board[y_min+2][x_min+2]){
		return board[y_min][x_min];
	}
	if(board[y_min][x_max] != SPACE && board[y_min][x_max] == board[y_min+1][x_max-1] && board[y_min+1][x_max-1] == board[y_min+2][x_max-2]){
		return board[y_min][x_max];
	}
	
	return 0;
}

int checkOverallBoard(int board[BOARD_SIZE][BOARD_SIZE]){
	//Chck vertical wins
	for(int row = 0; row < 3; row++){
		if(!isStillPlayed(board, row*3, row*3+2, 0, 8) &&
			board[0][row*3] == board[3][row*3] && board[3][row*3] == board[6][row*3]){
			return board[0][row*3];
		}
	}
	
	//check horizontal wins
	for(int column = 0; column < 3; column++){
		if(!isStillPlayed(board, 0, 8, column*3, column*3+2) &&
			board[column*3][0] == board[column*3][3] && board[column*3][3] == board[column*3][6]){
			return board[column*3][0];
		}
	}
	
	//check diagonals
	if(!isStillPlayed(board, 0, 2, 0, 2) && !isStillPlayed(board, 3, 5, 3, 5) && !isStillPlayed(board, 6, 8, 6, 8) &&
		board[0][0] == board[3][3] && board[3][3] == board[6][6]){
		return board[0][0];
	}
	if(!isStillPlayed(board, 6, 8, 0, 2) && !isStillPlayed(board, 3, 5, 3, 5) && !isStillPlayed(board, 0, 2, 6, 8) &&
		board[0][6] == board[3][3] && board[3][3] == board[6][0]){
		return board[0][6];
	}
	
	return -1;
}

int main(void){
	ece210_initialize_board();						// initialize Tiva C
	
	
	int board[BOARD_SIZE][BOARD_SIZE];
	int playerTurn; // 0 = player 1 (X), 1 = player 2 (O)
	int winner; //-1 = none, 0 = player 1 (X), 1 = player 2 (O), 2 = tie
	bool boardStateChanged;
	char gameState = 'I';	// I = instructions
											// P = play
											// O = over
											// W = wait for play signal
	int x_min;	//limits for what board can be selected
	int x_max;	//  initially any postion is allowed
	int y_min;
	int y_max;
	int x_cursor;
	int y_cursor;
	
	while(1){
		if(gameState == 'I'){	// Instructions
			ece210_lcd_add_msg("ULTIMATE TIC-TAC-TOE", TERMINAL_ALIGN_CENTER, LCD_COLOR_WHITE);
			ece210_lcd_add_msg("Xs go first.", TERMINAL_ALIGN_CENTER, LCD_COLOR_WHITE);
			ece210_lcd_add_msg("Press down to play", TERMINAL_ALIGN_CENTER, LCD_COLOR_WHITE);
			gameState = 'W';
		}else if(gameState == 'W'){	// Wait for play signal
			if(AlertButtons){
				AlertButtons = false;
				if(btn_up_pressed()){
					clearScreen();
					
					// reset game variables
					winner = -1;
					playerTurn = 0;
					boardStateChanged = true;
					x_min = 0;	//limits for what board can be selected
					x_max = 8;	//  initially any postion is allowed
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
					
					
					gameState = 'P';
				}
				if(btn_down_pressed()){
					clearScreen();
					gameState = 'I';
				}
			}
		} else if(gameState == 'P'){	// Play
			// draw board and cursor
			if(boardStateChanged){
				drawBoard(board);
				if(playerTurn){
					ece210_lcd_print_string("*", X_MAX_DRAW-X_SPACING*(x_cursor+0.5), Y_MAX_DRAW-Y_SPACING*(2*y_cursor+1), LCD_COLOR_MAGENTA, LCD_COLOR_BLACK);
				}else{
					ece210_lcd_print_string("*", X_MAX_DRAW-X_SPACING*(x_cursor+0.5), Y_MAX_DRAW-Y_SPACING*(2*y_cursor+1), LCD_COLOR_BLUE2, LCD_COLOR_BLACK);
				}
				boardStateChanged = false;
			}
		
			//move cursor
			// update x
			update_x(&x_cursor, &y_cursor, board, &boardStateChanged, x_min, x_max, y_min, y_max);
			// update y
			update_y(&x_cursor, &y_cursor, board, &boardStateChanged, x_min, x_max, y_min, y_max);
			
			// select postion
			if(AlertButtons){
				AlertButtons = false;
				if(btn_up_pressed()){
					boardStateChanged = true;
					
					//mark postion and swtich players
					if(playerTurn){
						board[y_cursor][x_cursor] = O;
						playerTurn = 0;
					} else{
						board[y_cursor][x_cursor] = X;
						playerTurn = 1;
					}
					
					//resets limits of at global board
					
					if(x_min == 0 && x_max == 8){
						if(x_cursor < 3){
							x_min = 0;
							x_max = 2;
						} else if(x_cursor < 6) {
							x_min = 3;
							x_max = 5;
						} else {
							x_min = 6;
							x_max = 8;
						}
						
						if(y_cursor < 3){
							y_min = 0;
							y_max = 2;
						} else if(y_cursor <5){
							y_min = 3;
							y_max = 5;
						} else {
							y_min = 6;
							y_max = 8;
						}
					}
					
					//checks to set new board state
					int check = isBoardWin(board, x_min, x_max, y_min, y_max);
					if(check != 0){
						//sets board as a win state
						for(int column = y_min; column < y_max+1; column++){
							for(int row = x_min; row < x_max+1; row++){
								board[column][row] = check;
							}
						}
					//check if the board is a tie
					} else if(!isStillPlayed(board, x_min, x_max, y_min, y_max)) {
						//sets board as a tie state
						for(int column = y_min; column < y_max+1; column++){
							for(int row = x_min; row < x_max+1; row++){
								board[column][row] = TIE;
							}
						}
						//checks the overall board
						
					}
					
					//checks the overall board
					check = checkOverallBoard(board);
					if(check != -1){
						winner = check-1;
						gameState = 'O';
					//check overall Board tie
					} else if(!isStillPlayed(board, 0, 8, 0, 8)){
						winner = 2;
						gameState = 'O';
					} else {
						//set new limits
						x_min=3*(x_cursor%3);
						x_max=x_min+2;
						y_min=3*(y_cursor%3);
						y_max=y_min+2;
						
						//check of board is already played
						if(!isStillPlayed(board, x_min, x_max, y_min, y_max)){
							//set limits to global board
							x_min=0;
							x_max=8;
							y_min=0;
							y_max=8;
						}
						
						//set cursor to open positon
						for(int column = y_min; column < y_max+1; column++){
							for(int row = x_min; row < x_max+1; row++){
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
		} else if(gameState == 'O'){ // Game Over
			if(winner == 0){
				ece210_lcd_add_msg("X wins.", TERMINAL_ALIGN_CENTER, LCD_COLOR_WHITE);
			} else if (winner == 1){
				ece210_lcd_add_msg("O wins.", TERMINAL_ALIGN_CENTER, LCD_COLOR_WHITE);
			} else if (winner == 2){
				ece210_lcd_add_msg("It's a tie.", TERMINAL_ALIGN_CENTER, LCD_COLOR_WHITE);
			}
			ece210_lcd_add_msg("Press down to play again", TERMINAL_ALIGN_CENTER, LCD_COLOR_WHITE);
			ece210_lcd_add_msg("Press up for instructions", TERMINAL_ALIGN_CENTER, LCD_COLOR_WHITE);
			gameState = 'W';
		}
	}
}