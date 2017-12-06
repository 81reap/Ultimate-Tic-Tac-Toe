//*****************************************************************************
// main.c
// Author: Prayag Bhakar
//*****************************************************************************

// Turn Debuggin off
#undef DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x) do {} while (0)
#endif

int main(void){
	
}

void drawBoard(){
  //void ece210_lcd_print_string(char* string, uint16_t x_pos, uint16_t y_pos, uint16_t fg_color, uint16_t bg_color);
  ece210_lcd_print_string()
}
