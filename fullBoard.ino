#include <SoftwareSerial.h>
SoftwareSerial buf(12,13);

// constant arrays
String rows[64];
String columns[64];
int pins[66];

// need to be updated on every move
String pieces[64];
char colors[64];
bool state[64];
bool prevState[64];
String wholeGame = "";

// for castling detection
bool whiteShort = false;
bool whiteLong = false;
bool blackShort = false;
bool blackLong = false;

bool gameStarted = true;
bool moveStarted = false;
bool withOwn = false;
bool theirTaken = false;
bool ourTaken = false;
bool pieceSelection = false;
int lastRowFieldOccupied = -1;

char colorMoving = 'w';
String pieceMoving = "";
char colorBeaten = 'x';
String pieceBeaten = "";
String publishMove = "";

String start = "";
String finish = "";

int moveCounter = 0;

void resetMove() {
  publishMove = "";
  pieceMoving = "";
  if (colorMoving == 'w') {
    colorMoving = 'b';
  } else if (colorMoving == 'b') {
    colorMoving = 'w';
  }
  start = "";
  finish = "";
  moveStarted = false;
  withOwn = false;
  theirTaken = false;
  ourTaken = false;
  colorBeaten = 'x';
  pieceBeaten = "";
  delay(1500);
}

void assignPiece(int i) {
 colors[i] = colorMoving;
 pieces[i] = pieceMoving;
}

void removePiece(int i) {
 colors[i] = ' ';
 pieces[i] = "";
}

String getCell(int i) {
  String result = rows[i];
  result += columns[i];
  return result;
}

void getMovedPieceInfo(int i) {
  pieceMoving = pieces[i];
}

void resetGame() {
  // 1-4 ROWS RESET
  for (int i = 31; i >= 0; --i) {
    switch (i+1) {
      case 1:
      case 29:
        pieces[i] = "R";
        break;
      case 5:
      case 25:
        pieces[i] = "N";
        break;
      case 9:
      case 21:
        pieces[i] = "B";
        break;
      case 13:
        pieces[i] = "Q";
        break;
      case 17:
        pieces[i] = "K";
        break;
      default:
        pieces[i] = "";
        break;
    }
    switch (i%4+1) {
      case 1:
      case 2:
        colors[i] = 'w';
        break;
      default:
        colors[i] = ' ';
    }
    state[i] = false;
    prevState[i] = false;
  }

  // 5-8 ROWS RESET
  for (int i = 0; i < 32; ++i) {
    state[32+i] = false;
    prevState[32+i] = false;
    switch (i+1) {
      case 4:
      case 32:
        pieces[32+i] = "R";
        break;
      case 8:
      case 28:
        pieces[32+i] = "N";
        break;
      case 12:
      case 24:
        pieces[32+i] = "B";
        break;
      case 16:
        pieces[32+i] = "Q";
        break;
      case 20:
        pieces[32+i] = "K";
        break;
      default:
        pieces[32+i] = "";
    }
    switch ((i+1)%4) {
      case 3:
      case 0:
        colors[32+i] = 'b';
        break;
      default:
        colors[32+i] = ' ';
    }
  }

  whiteShort = false;
  whiteLong = false;
  blackShort = false;
  blackLong = false;
  moveStarted = false;
  withOwn = false;
  theirTaken = false;
  ourTaken = false;
  colorMoving = 'w';
  pieceMoving = "";
  publishMove = "";
  start = "";
  finish = "";
  moveCounter = 0;
  
  for (int i = 0; i < 64; ++i) {
      if (digitalRead(pins[i]) == HIGH) {
        Serial.println(String(pins[i]) + "- HIGH");
      } 
      else {
        Serial.println(String(pins[i]) + "- LOW");
      }
    }
    
}

bool checkStart() {
  delay(1000);
  bool result = true;

  // CHECK ROWS 1-4
  for (int i = 0; i < 32; ++i) {
    
    if (i%4 == 0 || i%4 == 1) {
      if (!digitalRead(pins[i]) == false) {
        result = false;
      }
    } else {
      if (!digitalRead(pins[i]) == true) {
        result = false;
      }
    }
    
  }

  // CHECK ROWS 5-8
  for (int i = 0; i < 32; ++i) {
    if (i%4 == 0 || i%4 == 1) {
      if (!digitalRead(pins[32+i]) == true) {
        result = false;
      }
    } else {
      if (!digitalRead(pins[32+i]) == false) {
        result = false;
      }
    }
  }

  for (int i = 0; i < 64; ++i) {
      if (digitalRead(pins[i]) == HIGH) {
        Serial.println(String(pins[i]) + "- HIGH");
      } 
      else {
        Serial.println(String(pins[i]) + "- LOW");
      }
    }
  
  return result;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  buf.begin(9600);
  char a = 'a';

  // 1-4 rows initialized
  for (int i = 31; i >= 0; --i) {
    rows[i] = String(char('a'+(i/4)));
    columns[i] = String(i%4+1);
    switch (i+1) {
      case 1:
      case 29:
        pieces[i] = "R";
        break;
      case 5:
      case 25:
        pieces[i] = "N";
        break;
      case 9:
      case 21:
        pieces[i] = "B";
        break;
      case 13:
        pieces[i] = "Q";
        break;
      case 17:
        pieces[i] = "K";
        break;
      default:
        pieces[i] = "";
        break;
    }
    switch (i%4+1) {
      case 1:
      case 2:
        colors[i] = 'w';
        break;
      default:
        colors[i] = ' ';
    }
    state[i] = false;
    prevState[i] = false;
    switch (i) {
      case 0:
        pins[i] = A4;
        break;
      case 1:
        pins[i] = A5;
        break;
      case 2:
        pins[i] = A6;
        break;
      case 3:
        pins[i] = A7;
        break;
      case 4:
        pins[i] = A8;
        break;
      case 5:
        pins[i] = A9;
        break;
      case 6:
        pins[i] = A10;
        break;
      case 7:
        pins[i] = A11;
        break;
      case 8:
        pins[i] = A12;
        break;
      case 9:
        pins[i] = A13;
        break;
      case 10:
        pins[i] = A14;
        break;
      case 11:
        pins[i] = A15;
        break;
      default:
        pins[i] = 53 - i + 12;
        break;
    }
    pinMode(pins[i], INPUT_PULLUP);
  }

  pins[64] = A0;
  pinMode(A0, INPUT_PULLUP);
  pins[65] = A1;
  pinMode(A1, INPUT_PULLUP);
  

  // 5-8 rows initialized
  for (int i = 0; i < 32; ++i) {
    rows[32+i] = String(char('a'+(i/4)));
    columns[32+i] = String(i%4 + 5);
    state[32+i] = false;
    prevState[32+i] = false;
    pins[32+i] = i+2;
    pins[42] = A2;
    pins[43] = A3;
    switch (i+1) {
      case 4:
      case 32:
        pieces[32+i] = "R";
        break;
      case 8:
      case 28:
        pieces[32+i] = "N";
        break;
      case 12:
      case 24:
        pieces[32+i] = "B";
        break;
      case 16:
        pieces[32+i] = "Q";
        break;
      case 20:
        pieces[32+i] = "K";
        break;
      default:
        pieces[32+i] = "";
    }
    switch ((i+1)%4) {
      case 3:
      case 0:
        colors[32+i] = 'b';
        break;
      default:
        colors[32+i] = ' ';
    }
    pinMode(pins[32+i], INPUT_PULLUP);
  }
  
  for (int i = 0; i < 64; ++i) {
      if (digitalRead(pins[i]) == HIGH) {
        Serial.println(String(pins[i]) + "- HIGH");
      } 
      else {
        Serial.println(String(pins[i]) + "- LOW");
      }
    }

  if (!checkStart()) {
    Serial.println("Please setup your pieces correctly for the start of the game");
    gameStarted = false;
  } else {
    Serial.println("Seems good, start the game");
  }

}

void loop() {

  if (!gameStarted) {
    if (!digitalRead(pins[64])) {
      if (checkStart()) {
        resetGame();
        Serial.println("Game reset");
        gameStarted = true;
      }
    }
  }
  
  // WHITE SHORT CASTLING HANDLED
  if (whiteShort) {
    state[20] = !digitalRead(pins[20]);
    if (state[20] != prevState[20] && state[20] == true) {
      whiteShort = false;
      state[24] = true;
      prevState[24] = true;
      prevState[20] = state[20];
      removePiece(28);
      removePiece(16);
      assignPiece(24);
      colors[20] = 'w';
      pieces[20] = 'R';
      publishMove = " 0-0";

      if (moveCounter%2 == 1) {
        if (moveCounter == 1) {
          String moveNum = String(moveCounter/2 + 1) + ".";
          Serial.print(moveNum);
          wholeGame += moveNum;
        } else {
          String moveNum = " " + String(moveCounter/2 + 1) + ".";
          Serial.print(moveNum);
          wholeGame += moveNum;
        }
      }
      
      Serial.print(publishMove);
      wholeGame += publishMove;
      resetMove();
    }
  }

  // WHITE LONG CASTLING HANDLED
  if (whiteLong) {
    state[12] = !digitalRead(pins[12]);
    if (state[12] != prevState[12] && state[12] == true) {
      whiteLong = false;
      state[8] = true;
      prevState[8] = true;
      prevState[12] = state[12];
      removePiece(0);
      removePiece(16);
      assignPiece(8);
      colors[12] = 'w';
      pieces[12] = 'R';
      publishMove = " 0-0-0";

      if (moveCounter%2 == 1) {
        if (moveCounter == 1) {
          String moveNum1 = String(moveCounter/2 + 1) + ".";
          Serial.print(moveNum1);
          wholeGame += moveNum1;
        } else {
          String moveNum1 = " " + String(moveCounter/2 + 1) + ".";
          Serial.print(moveNum1);
          wholeGame += moveNum1;
        }
      }
      
      Serial.print(publishMove);
      wholeGame += publishMove;
      resetMove();
    }
  }

  // BLACK SHORT CASTLING HANDLED
  if (blackShort) {
    state[55] = !digitalRead(pins[55]);
    if (state[55] != prevState[55] && state[55] == true) {
      blackShort = false;
      state[59] = true;
      prevState[59] = true;
      prevState[55] = state[55];
      removePiece(63);
      removePiece(51);
      assignPiece(59);
      colors[55] = 'b';
      pieces[55] = 'R';
      publishMove = " 0-0";

      if (moveCounter%2 == 1) {
        if (moveCounter == 1) {
          String moveNum3 = String(moveCounter/2 + 1) + ".";
          Serial.print(moveNum3);
          wholeGame += moveNum3;
        } else {
          String moveNum3 = " " + String(moveCounter/2 + 1) + ".";
          Serial.print(moveNum3);
          wholeGame += moveNum3;
        }
      }
      
      Serial.print(publishMove);
      wholeGame += publishMove;
      resetMove();
    }
  }
  
  // BLACK LONG CASTLING HANDLED
  if (blackLong) {
    state[47] = !digitalRead(pins[47]);
    if (state[47] != prevState[47] && state[47] == true) {
      blackLong = false;
      state[43] = true;
      prevState[43] = true;
      prevState[47] = state[47];
      removePiece(35);
      removePiece(51);
      assignPiece(43);
      colors[47] = 'b';
      pieces[47] = 'R';
      publishMove = " 0-0-0";
      
      if (moveCounter%2 == 1) {
        if (moveCounter == 1) {
          String moveNum4 = String(moveCounter/2 + 1) + ".";
          Serial.print(moveNum4);
          wholeGame += moveNum4;
        } else {
          String moveNum4 = " " + String(moveCounter/2 + 1) + ".";
          Serial.print(moveNum4);
          wholeGame += moveNum4;
        }
      }
      
      Serial.print(publishMove);
      wholeGame += publishMove;
      resetMove();
    }
  }
  
  if (!whiteShort && !whiteLong && !blackShort && !blackLong && gameStarted)  {
    for (int i = 0; i < 66; ++i) {

      if (i < 64 && !pieceSelection) {
        
        state[i] = !digitalRead(pins[i]);
        
        if (!moveStarted) {
    
          // TAKE SOMETHING FROM THE BOARD - then the move starts
          
          if (state[i] == false && state[i] != prevState[i]) {
            
            
              moveStarted = true;
              if (colors[i] == colorMoving) {
                withOwn = true;
                getMovedPieceInfo(i);
                start = getCell(i);
              } else {
                if (colorMoving == 'b') {
                  colorBeaten = 'w';
                  pieceBeaten = pieces[i];
                } else if (colorMoving == 'w') {
                  colorBeaten = 'b';
                  pieceBeaten = pieces[i];
                }
              }
              removePiece(i);
      
              ++moveCounter;
            
          }
          prevState[i] = state[i];
          continue;
        
        } 
        else if (moveStarted) {
          
          // WE ARE WAITING FOR COMPLETION
    
          if (withOwn) {
    
            // WE CAN MOVE TO THE UNOCCUPIED CELL - COMPLETE
            if (state[i] == true && state[i] != prevState[i] && colors[i] == ' ') {
              finish = getCell(i);

              
              
              String castlingCheck = pieceMoving + start + "-" + finish;
              if (castlingCheck == "Ke1-g1") {
                
                whiteShort = true;
                prevState[i] = state[i];
                state[28] = false;
                prevState[28] = state[28];
                break;
              }
              if (castlingCheck == "Ke1-c1") {
                
                whiteLong = true;
                prevState[i] = state[i];
                state[0] = false;
                prevState[0] = state[0];
                break;
              }
              if (castlingCheck == "Ke8-g8") {
                
                blackShort = true;
                prevState[i] = state[i];
                state[63] = false;
                prevState[63] = state[63];
                break;
              }
              if (castlingCheck == "Ke8-c8") {
                
                blackLong = true;
                prevState[i] = state[i];
                state[35] = false;
                prevState[35] = state[35];
                break;
              }
    
              if (start == finish) {
                finish = "";
                continue;
              }

              if (moveCounter%2 == 1) {
                if (moveCounter == 1) {
                  String moveNum6 = String(moveCounter/2 + 1) + ".";
                  Serial.print(moveNum6);
                  wholeGame += moveNum6;
                } else {
                  String moveNum6 = " " + String(moveCounter/2 + 1) + ".";
                  Serial.print(moveNum6);
                  wholeGame += moveNum6;
                }
              }
              
              
              if (columns[i] == "8" && pieceMoving == "" && colorMoving == 'w') {
                
                lastRowFieldOccupied = i;
                pieceSelection = true;
              }
              if (columns[i] == "1" && pieceMoving == "" && colorMoving == 'b') {
                
                lastRowFieldOccupied = i;                
                pieceSelection = true;
              }
              
              if (theirTaken) {
                publishMove = " " + pieceMoving + start + "x" + finish;
              } else if (!theirTaken) {
                publishMove = " " + pieceMoving + start + "-" + finish;
              }
              
              
              Serial.print(publishMove);
              wholeGame += publishMove;
              assignPiece(i);
              resetMove();
              prevState[i] = state[i];
              continue;
            }
    
            // OR WE CAN BEAT OTHER PIECE FIRST
            if (state[i] == false && state[i] != prevState[i] && colors[i] != colorMoving && colors[i] != ' ') {
              
              removePiece(i);
              theirTaken = true;
              prevState[i] = state[i];
              continue;
            }
            
          } else if (!withOwn) {
    
            // WE CAN TAKE OUR PIECE TO BEAT THE OPPONENTS'
            if (state[i] == false && state[i] != prevState[i] && colors[i] == colorMoving) {
              
              getMovedPieceInfo(i);
              start = getCell(i);
              removePiece(i);
              ourTaken = true;
              prevState[i] = state[i];
              continue;
            }
    
            // AFTER TAKING OUR PIECE, WE CAN PLACE IT ON THE UNOCCUPIED (NOW) FIELD - COMPLETE
            if (state[i] == true && state[i] != prevState[i] && colors[i] == ' ' && ourTaken) {
              finish = getCell(i);
    
              if (start == finish) {
                finish = "";
                continue;
              }

              if (columns[i] == "8" && pieceMoving == "" && colorMoving == 'w') {
               
                lastRowFieldOccupied = i;
                pieceSelection = true;
              }
              if (columns[i] == "1" && pieceMoving == "" && colorMoving == 'b') {
                
                lastRowFieldOccupied = i;                
                pieceSelection = true;
              }

              if (moveCounter%2 == 1) {
                if (moveCounter == 1) {
                  String moveNum7 = String(moveCounter/2 + 1) + ".";
                  Serial.print(moveNum7);
                  wholeGame += moveNum7;
                } else {
                  String moveNum7 = " " + String(moveCounter/2 + 1) + ".";
                  Serial.print(moveNum7);
                  wholeGame += moveNum7;
                }
              }
              publishMove = " " + pieceMoving + start + "x" + finish;
              Serial.print(publishMove);
              wholeGame += publishMove;
              assignPiece(i);
              resetMove();
              prevState[i] = state[i];
              continue;
            }
            
          }
          
        }
    
        prevState[i] = state[i];

      }

      if (i >= 64) {
        // CHECKMATE, CANCEL, RESET BUTTON
        if (i == 64) {
          if (!digitalRead(pins[i]) == true) {
              delay(1000); 
              if (!digitalRead(pins[i]) == false) {
                
                bool waitingForCancelMove = true;
                while(waitingForCancelMove) {
                  for (int i = 0; i < 64; ++i) {
                    state[i] = !digitalRead(pins[i]);
                    if (state[i] == true && state[i] != prevState[i] && colors[i] == ' ') {
                      
                      prevState[i] = state[i];
                      if (withOwn) {
                        assignPiece(i);
                      } else {
                        pieces[i] = pieceBeaten;
                        colors[i] = colorBeaten;
                      }
                      --moveCounter;
                      resetMove();
                      resetMove();
                      waitingForCancelMove = false;
                      break;
                    }
                  }
                }
                delay(200);
              } else {
                delay(2000);
                if (!digitalRead(pins[i]) == false) {
                  Serial.print("+");
                  wholeGame += "+";
                  delay(200);
                } else {
                  delay(2000);
                  if (!digitalRead(pins[i]) == false) {
                    Serial.print("#");
                    wholeGame += "#";
                    delay(200);
                  } else {
                    Serial.println("Restart the game");
                    gameStarted = false;
                    delay(5000);
                    delay(1000);
                  }
                }
              }
          }
        }
        // CHOICE OF THE PIECE TO REPLACE PAWN UPON REACHING LAST ROW
        if (i == 65) {
          if (!digitalRead(pins[i]) == true) {
            delay(2000);
            if (!digitalRead(pins[64]) == true) {
              if (wholeGame.length() > 2) {
                buf.print(wholeGame);
                delay(5000);
              }
            }
          }
          while (pieceSelection) {
            if (!digitalRead(pins[i]) == true) {
              delay(1000);
              if (!digitalRead(pins[i]) == false) {
                Serial.print("Q");
                wholeGame += "Q";
                pieces[lastRowFieldOccupied] = "Q";
                lastRowFieldOccupied = -1;
                pieceSelection = false;
              } else {
                delay(2000);
                if (!digitalRead(pins[i]) == false) {
                  Serial.print("R");
                  wholeGame += "R";
                  pieces[lastRowFieldOccupied] = "R";
                  lastRowFieldOccupied = -1;
                  pieceSelection = false;
                } else {
                  delay(2000);
                  if (!digitalRead(pins[i]) == false) {
                    Serial.print("B");
                    wholeGame += "B";
                    pieces[lastRowFieldOccupied] = "B";
                    lastRowFieldOccupied = -1;
                    pieceSelection = false;
                  } else {
                    Serial.print("N");
                    wholeGame += "N";
                    pieces[lastRowFieldOccupied] = "N";
                    lastRowFieldOccupied = -1;
                    pieceSelection = false;
                    }
                  
                }
              }
            }
          }
        }
      }
    }   
  }

}
  
