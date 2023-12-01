// CLEAR ONE SQUARE
void clearSquare(int column, int row) {
  lcd.setCursor(column, row);
  lcd.print(" ");
}

// CLEAR ONE LINE
void clearLine(int row) {
  lcd.setCursor(0, row);
  lcd.print("                "); //16 Spaces
}

// PRINT LOADING TEXT
void printText(String text, int column, int row) {
  int textLength = text.length() + 1;
  char textArray[textLength];
  text.toCharArray(textArray, textLength);

  lcd.setCursor(column, row);
  lcd.print(textArray);
}

// PRINT HOURGLASS ANIMATION
void printHourGlass(int column, int row) {
  switch (hourGlassState) {
    case 0:
      printIcon(6, column, row);
      
      hourGlassState = 1;
    break;

    case 1:
      printIcon(7, column, row);

      hourGlassState = 2;
    break;

    case 2:
      clearSquare(column, row);

      hourGlassState = 0;
    break;
  }
} 

// PRINT TEXT AND HOURGLASS TOGETHER
void printLoadingProcess(String text, int column, int row) {
  if (millis() - previousHourGlassMillis >= defaultLoadingDuration) {
    printHourGlass(column, row);
    previousHourGlassMillis = millis();
  } 
  
  printText(text, column + 2, row); 
}

// BLINK TEXT
void blinkText(String text, int column, int row) {
  switch (blinkState) {
    case 0:
      printText(text, column, row);
      
      blinkState = 1;
    break;

    case 1:
      for (int x = 0; x < text.length(); x++) {
        clearSquare(column + x, row);
      }

      blinkState = 0;
    break;
  }
}

// PROGRESS BAR
void printBar(int value) {
  switch (value) {
    case 0:
      clearSquare(5, 1);
      clearSquare(6, 1);
      clearSquare(7, 1);
      clearSquare(8, 1);
      clearSquare(9, 1);
      clearSquare(10, 1);
      clearSquare(11, 1);
      clearSquare(12, 1);
      clearSquare(13, 1);
      clearSquare(14, 1);
      clearSquare(15, 1);
    break;

    case 1:
      printIcon(8, 5, 1);
    break;

    case 2:
      printIcon(9, 5, 1);

      printIcon(8, 6, 1);
    break;

    case 3:
      printIcon(9, 5, 1);
      printIcon(9, 6, 1);

      printIcon(8, 7, 1);
    break;

    case 4:
      printIcon(9, 5, 1);
      printIcon(9, 6, 1);
      printIcon(9, 7, 1);

      printIcon(8, 8, 1);
    break;

    case 5:
      printIcon(9, 5, 1);
      printIcon(9, 6, 1);
      printIcon(9, 7, 1);
      printIcon(9, 8, 1);  

      printIcon(8, 9, 1);
    break;

    case 6:
      printIcon(9, 5, 1);
      printIcon(9, 6, 1);
      printIcon(9, 7, 1);
      printIcon(9, 8, 1);  
      printIcon(9, 9, 1);

      printIcon(8, 10, 1);
    break;

    case 7:
      printIcon(9, 5, 1);
      printIcon(9, 6, 1);
      printIcon(9, 7, 1);
      printIcon(9, 8, 1);  
      printIcon(9, 9, 1);
      printIcon(9, 10, 1);

      printIcon(8, 11, 1);
    break;

    case 8:
      printIcon(9, 5, 1);
      printIcon(9, 6, 1);
      printIcon(9, 7, 1);
      printIcon(9, 8, 1);  
      printIcon(9, 9, 1);
      printIcon(9, 10, 1);
      printIcon(9, 11, 1);
      
      printIcon(8, 12, 1);
    break;

    case 9:
      printIcon(9, 5, 1);
      printIcon(9, 6, 1);
      printIcon(9, 7, 1);
      printIcon(9, 8, 1);  
      printIcon(9, 9, 1);
      printIcon(9, 10, 1);
      printIcon(9, 11, 1);
      printIcon(9, 12, 1);
      
      printIcon(8, 13, 1);
    break;

    case 10:
      printIcon(9, 5, 1);
      printIcon(9, 6, 1);
      printIcon(9, 7, 1);
      printIcon(9, 8, 1);  
      printIcon(9, 9, 1);
      printIcon(9, 10, 1);
      printIcon(9, 11, 1);
      printIcon(9, 12, 1);
      printIcon(9, 13, 1);

      printIcon(8, 14, 1);
    break;
  }
}

// CONVERTS SET SPEED TO SERVO.WRITE VALUE
int setSpinSpeed(int setSpeed) {
  int spinSpeed = map(setSpeed, 0, 10, 1360, 1330);
  Serial.println("Spin Speed: " + String(spinSpeed));
  return spinSpeed;
}   