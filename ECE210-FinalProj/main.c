//*****************************************************************************
// main.c
// Author: Prayag Bhakar
//*****************************************************************************
#include "ece210_api.h"
#include "lab_buttons.h"

#define Y_SPACING 17
#define X_SPACING 25
#define Y_MAX_DRAW 320
#define X_MAX_DRAW 240
#define BOARD_SIZE 9
//board states
#define SPACE 0
#define DRAW 3
#define X 1
#define O 2

bool boardStateChanged = true;
char gameState = 'I';	// I = instructions
						// P = play
						// O = over

//draw board
void drawBoard(int board[BOARD_SIZE][BOARD_SIZE]){
	float offset = 0;
	for(int row = 0; row < BOARD_SIZE; row++){
		for(int column = 0; column < BOARD_SIZE; column++){
			if((column+1)%3 == 0){
				if(board[row][column] == SPACE){
					ece210_lcd_print_string("     ", X_SPACING*(BOARD_SIZE-column), Y_SPACING*offset, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
				} else if(board[row][column] == X) {
					ece210_lcd_print_string(" X  ", X_SPACING*(BOARD_SIZE-column), Y_SPACING*offset, LCD_COLOR_BLUE2, LCD_COLOR_BLACK);
				} else if(board[row][column] == O) {
					ece210_lcd_print_string(" O  ", X_SPACING*(BOARD_SIZE-column), Y_SPACING*offset, LCD_COLOR_MAGENTA, LCD_COLOR_BLACK);
				} else if(board[row][column] == DRAW) {
					ece210_lcd_print_string(" -  ", X_SPACING*(BOARD_SIZE-column), Y_SPACING*offset, LCD_COLOR_GREEN2, LCD_COLOR_BLACK);
				}
			}else{
				if(board[row][column] == SPACE){
					ece210_lcd_print_string("    |", X_SPACING*(BOARD_SIZE-column), Y_SPACING*offset, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
				} else if(board[row][column] == X) {
					ece210_lcd_print_string(" X |", X_SPACING*(BOARD_SIZE-column), Y_SPACING*offset, LCD_COLOR_BLUE2, LCD_COLOR_BLACK);
				} else if(board[row][column] == O) {
					ece210_lcd_print_string(" O |", X_SPACING*(BOARD_SIZE-column), Y_SPACING*offset, LCD_COLOR_MAGENTA, LCD_COLOR_BLACK);
				} else if(board[row][column] == DRAW) {
					ece210_lcd_print_string(" - |", X_SPACING*(BOARD_SIZE-column), Y_SPACING*offset, LCD_COLOR_GREEN2, LCD_COLOR_BLACK);
				}
			}
		}
		offset++;
		for(int column = 0; column < BOARD_SIZE; column++){
			if((row+1)%3 == 0){
				ece210_lcd_print_string("    ", X_SPACING*(BOARD_SIZE-column), Y_SPACING*offset, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
			}else if((column+1)%3 == 0){
				ece210_lcd_print_string("-- ", X_SPACING*(BOARD_SIZE-column), Y_SPACING*offset, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
			}else{
				ece210_lcd_print_string("--+", X_SPACING*(BOARD_SIZE-column), Y_SPACING*offset, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
			}
		}
		offset++;
	}
}

//move cursor left/right
int update_x(int x_cursor, int y_cursor){
	int x = ece210_ps2_read_x();
  
	// left
	if( (x >= PS2_THRESHOLD_X_HI) && ( x_cursor > 0 )){
		boardStateChanged = true;
			for(int row in )
			return --x_cursor;
	// right
	}else if ( (x <= PS2_THRESHOLD_X_LO) && ( x_cursor < BOARD_SIZE-1) ){
		boardStateChanged = true;
		return  ++x_cursor;
	// no change
	}else{
		return x_cursor;
  	}
}	// end update_x

//move cursor up/down
int update_y(int x_cursor, int y_cursor){
	int y = ece210_ps2_read_y();
  
	// down 
  if( (y >= PS2_THRESHOLD_Y_HI) && ( y_cursor > 0 ) ){
    boardStateChanged = true;
		return --y_cursor ;
	// up
  }else if ( (y <= PS2_THRESHOLD_Y_LO) && ( y_cursor < BOARD_SIZE-1)){
    boardStateChanged = true;
		return ++y_cursor ;
	// no change
  }else{
    return y_cursor ;
  }
}	// end update_y

//empty the screen
void clearScreen(){
	for(int i = 0; i < 20; i++){
		ece210_lcd_add_msg("", TERMINAL_ALIGN_CENTER, LCD_COLOR_WHITE);
	}
}

int main(void){
	ece210_initialize_board();						// initialize Tiva C
	
	bool showInstructions = true;
	
	// initialize board
	int board[BOARD_SIZE][BOARD_SIZE];
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
				board[i][j] = SPACE;
		}
	}
	
	int cursor_x = 0;
	int cursor_y = 0;
	int playerTurn = 0; // 0 = player 1 (X), 1 = player 2 (O)
	
	while(1){
		if(gameState == 'I'){	// Instructions
			if(showInstructions){
				ece210_lcd_add_msg("ULTIMATE TIC-TAC-TOE", TERMINAL_ALIGN_CENTER, LCD_COLOR_WHITE);
				ece210_lcd_add_msg("Xs go first.", TERMINAL_ALIGN_CENTER, LCD_COLOR_WHITE);
				showInstructions = false;
			}
			if(AlertButtons){
				AlertButtons = false;
				if(btn_up_pressed()){
					clearScreen();
					gameState = 'P';
				}
			}
		} else if(gameState == 'P'){	// Play
			// draw board and cursor
			if(boardStateChanged){
				board[0][0] = O;
				board[0][1] = X;
				drawBoard(board);
				if(playerTurn){
					ece210_lcd_print_string(" * ", X_SPACING*(BOARD_SIZE-cursor_x), Y_SPACING*cursor_y*2, LCD_COLOR_MAGENTA, LCD_COLOR_BLACK);
				}else{
					ece210_lcd_print_string(" * ", X_SPACING*(BOARD_SIZE-cursor_x), Y_SPACING*cursor_y*2, LCD_COLOR_BLUE2, LCD_COLOR_BLACK);
				}
				boardStateChanged = false;
			}
		
			//move cursor
			// update x
			cursor_x = update_x(cursor_x, cursor_y);
			// update y
			cursor_y = update_y(cursor_x, cursor_y);
			
			// select postion
			if(AlertButtons){
				AlertButtons = false;
				boardStateChanged = true;
				if(btn_up_pressed()){
					if(playerTurn){
						board[cursor_y][cursor_x] = O;
						playerTurn = 0;
					} else{
						board[cursor_y][cursor_x] = X;
						playerTurn = 1;
					}
				}
			}
		} //else if(gameState == 'O'){ // Game Over
			
		//}
	}
}
