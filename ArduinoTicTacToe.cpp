//Code for playing TicTacToe on an Arduino UNO
//DFRobot RBGLCD1602 and DFRobot RGBButtons were used with I2C
//OLED with SSD1306 controller was used with SPI

//Libraries
#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <DFRobot_RGBLCD1602.h>
#include <DFRobot_RGBButton.h>
#include <Wire.h>

//Defining columns and rows for tictactoe checkerboard
#define column1 36
#define column2 57
#define column3 78

#define row1 18
#define row2 39
#define row3 60

//OLED constructor
U8G2_SSD1306_128X64_NONAME_1_4W_SW_SPI OLED(U8G2_R0, /* clock=*/ 12, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 8, /* reset=*/ 9);

//LCD constructor
DFRobot_RGBLCD1602 lcd(0x2D , 16, 2);  //16 characters and 2 lines of show

//Button constructors
DFRobot_RGBButton buttonSelect(&Wire, 0x23);
DFRobot_RGBButton buttonRight(&Wire, 0x24);
DFRobot_RGBButton buttonLeft(&Wire, 0x25);

//Initilizing counting variable, board array and turn counter
char board[10] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
int i = 1;
bool turn = 0; //Turn = 0 means X turn. Turn = 1 means O turn

//Initilize LCD function
void initLCD(){
    lcd.init();
    lcd.clear();
    lcd.setBacklight(1);
    lcd.setRGB(127, 20, 127);
    lcd.setCursor(4, 0);
    lcd.print("Welcome"); 
    lcd.setCursor(0,1);
    lcd.print("to Tic Tac Toe!");
    delay(2000);
}

//Function that updates the board every time theres an update to the OLED
void updateBoard(){
  OLED.firstPage();
  do {
    //Makes a checkerboard on the OLED
    OLED.drawFrame(32, 0, 64, 64);
    OLED.drawHLine(32, 21, 64);
    OLED.drawHLine(32, 42, 64);
    OLED.drawVLine(53, 0, 64);
    OLED.drawVLine(74, 0, 64);

    //Updates every square on the OLED with the corresponding address in the board array
    OLED.setCursor(column1, row1);  OLED.print(board[1]);
    OLED.setCursor(column2, row1);  OLED.print(board[2]);
    OLED.setCursor(column3, row1);  OLED.print(board[3]);
    OLED.setCursor(column1, row2);  OLED.print(board[4]);
    OLED.setCursor(column2, row2);  OLED.print(board[5]);
    OLED.setCursor(column3, row2);  OLED.print(board[6]);
    OLED.setCursor(column1, row3);  OLED.print(board[7]);
    OLED.setCursor(column2, row3);  OLED.print(board[8]);
    OLED.setCursor(column3, row3);  OLED.print(board[9]);
  } while ( OLED.nextPage() );
}

//Function that handles all the logic tied to placing an O or X
//as well as seeing the current cursor position
void posSelect(){
    for (int x = 1; x < 10; x++){ //The question mark is to indicate the selected position before you confirm your choice
    if (board[x] == '?'){         //this for loop just erases the question marks as you move along the board
      board[x] = ' ';
    }
  }
  delay(10);
  if (board[i] == ' '){ //Places a question mark 
    board[i] = '?';
  }

  if (buttonRight.getButtonStatus()){ //Scrolls to the right when the red button is pressed
    i = i + 1;
    while((board[i] == 'X') || (board[i] == 'O')){//Skips past positions that have an X or O
      i = i + 1;
    }
    while(1){
      if (!(buttonRight.getButtonStatus())){//Makes sure that you only move one position every time you press the button
        break;
      }
    }
  }
  else if (buttonLeft.getButtonStatus()){ //Same as above just for going left 
    i = i - 1;
    while((board[i] == 'X') || (board[i] == 'O')){
      i = i - 1;
    }
    while(1){
      if (!(buttonLeft.getButtonStatus())){
        break;
      }
    }
  }

  if (i < 1){ //Scrolls to opposite end of board if you go past the edges of the board
    i = 9;
    while((board[i] == 'X') || (board[i] == 'O')){
      i = i - 1;
    }
  }
  else if (i > 9){ //Same as above for the opposite direction
    i = 1;
    while((board[i] == 'X') || (board[i] == 'O')){
      i = i + 1;
    }
  }
  

  if (buttonSelect.getButtonStatus()){ //Confirms X or O placement depending on turn
    if ((turn == 0) && (board[i] != 'X') && (board[i] != 'O')){
      board[i] = 'X';
      while(1){
      if (!(buttonSelect.getButtonStatus())){
        break;
      }
      turn = 1;
    }
    }
    else if ((turn == 1) && (board[i] != 'X') && (board[i] != 'O')){
      board[i] = 'O';
      while(1){
      if (!(buttonSelect.getButtonStatus())){
        break;
      }
      turn = 0;
    }
    }
  
  }
}

//Function in charge of checking for win conditions of X and O
bool checkForWin(char symbol){
    return(
        (board[1] == symbol && board[2] == symbol && board[3] == symbol) || // Top row
        (board[4] == symbol && board[5] == symbol && board[6] == symbol) || // Middle row
        (board[7] == symbol && board[8] == symbol && board[9] == symbol) || // Bottom row
        (board[1] == symbol && board[4] == symbol && board[7] == symbol) || // Left collumn
        (board[2] == symbol && board[5] == symbol && board[8] == symbol) || // Middle collumn
        (board[3] == symbol && board[6] == symbol && board[9] == symbol) || // Right collumn
        (board[1] == symbol && board[5] == symbol && board[9] == symbol) || // Negative diagonal
        (board[3] == symbol && board[5] == symbol && board[7] == symbol)    // Positive diagonal
    );
}

//Function that checks if the board is full to determine if theres a draw or not
bool isBoardFull(){
    for(int i = 1; i < 10; i++){
        if ((board[i] == ' ') || (board[i] == '?')){
            return false;
        }
    }
    return true; //Returns true if all of the squares are occupied by an X or O
}

void setup() {
  OLED.begin(); //Initialize OLED
  OLED.clear(); //Clear OLED
  OLED.setFont(u8g2_font_fub14_tf); //Sets font thats used for O and X

  //Sets colour for the buttons
  buttonSelect.setRGBColor(0, 255, 0);  
  buttonRight.setRGBColor(255, 0, 0);
  buttonLeft.setRGBColor(0, 0, 255);

  //Runs LCD initilize function
  initLCD();
}

void loop(){
  updateBoard();
  posSelect();

  //Shows whos turn it is
  if (turn == 0){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("X to play."); 
  }
  else if (turn == 1){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("O to play.");
  }
  
  //Checks if X or O has won
  if (checkForWin('X') == 1){
    updateBoard();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("X wins!");
    while(1){ //Endless while loop to stop the game
      delay(10);
    }
  }
  else if (checkForWin('O') == 1){
    updateBoard();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("O wins!");
    while(1){ //Endless while loop to stop the game
      delay(10);
    }
  } //If the board is full and X or O hasnt won the game is a draw.
  else if (isBoardFull() == 1){
    updateBoard();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Game is a draw!");
    while(1){ //Endless while loop to stop the game
      delay(10);
    }
  }
}

