//ICON VARIABLES
byte degreeSymbol = char(223);
byte returnIcon[8] = {0x0E, 0x01, 0x05, 0x09, 0x1E, 0x08, 0x04, 0x00};
byte downArrow[8] = {0x00, 0x00, 0x00, 0x00, 0x1F, 0x0E, 0x04, 0x00};
byte upArrow[8] = {0x04, 0x0E, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00};
byte middleArrow[8] = {0x04, 0x0E, 0x1F, 0x00, 0x1F, 0x0E, 0x04, 0x00};
byte forwardArrow[8] = {0x00, 0x04, 0x02, 0x1F, 0x02, 0x04, 0x00, 0x00};
byte backArrow[8] = {0x00, 0x04, 0x08, 0x1F, 0x08, 0x04, 0x00, 0x00};

byte hourGlassUp[8] = {0x1F, 0x0E, 0x0E, 0x04, 0x0A, 0x0A, 0x1F, 0x00};
byte hourGlassDown[8] = {0x1F, 0x0A, 0x0A, 0x04, 0x0E, 0x0E, 0x1F, 0x00};

byte fullBar[8] = {0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x00};
byte halfBar[8] = {0x15, 0x0A, 0x15, 0x0A, 0x15, 0x0A, 0x1F, 0x00};

byte checkIcon[8] = {0x00, 0x00, 0x01, 0x02, 0x14, 0x08, 0x00, 0x00};
byte crossIcon[8] = {0x00, 0x00, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x00};
byte exclaimMark[8] = {0x00, 0x04, 0x04, 0x04, 0x04, 0x00, 0x04, 0x00};

byte thermometerIcon[8] = {0x04, 0x0A, 0x0A, 0x0A, 0x11, 0x1F, 0x0E, 0x00};
byte clockIcon[8] = {0x0E, 0x0E, 0x15, 0x17, 0x11, 0x0E, 0x0E, 0x00};
byte resetArrow[8] = {0x00, 0x00, 0x0D, 0x13, 0x17, 0x10, 0x0E, 0x00};
byte savedIcon[8] = {0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1B, 0x11, 0x00};
byte waterLevelIcon[8] = {0x00, 0x04, 0x0E, 0x1F, 0x1F, 0x0E, 0x00, 0x00};
byte dayIcon[8] = {0x00, 0x0E, 0x1F, 0x1F, 0x1F, 0x0E, 0x00, 0x00};
byte nightIcon[8] = {0x00, 0x0E, 0x03, 0x03, 0x17, 0x0E, 0x00, 0x00};
byte lightBulbOn[8] = {0x0E, 0x1F, 0x1F, 0x1F, 0x0E, 0x0A, 0x04, 0x00};
byte lightBulbOff[8] = {0x0E, 0x11, 0x11, 0x11, 0x0E, 0x0A, 0x04, 0x00};
         

// PRINT SPECIFIC ICON WHEN CALLED (Icon Number, LCDColumn, LCDRow)
void printIcon(int icon, int column, int row) { 
  switch (icon) {
    case 0: //Return Icon
      lcd.createChar(0, returnIcon);

      lcd.setCursor(column, row);
      lcd.write(byte(0));
    break;

    case 1: //Down Arrow
      lcd.createChar(1, downArrow);

      lcd.setCursor(column, row);
      lcd.write(byte(1));
    break;

    case 2: //Middle Arrow
      lcd.createChar(1, middleArrow);

      lcd.setCursor(column, row);
      lcd.write(byte(1));
    break;

    case 3: //Up Arrow
      lcd.createChar(1, upArrow);

      lcd.setCursor(column, row);
      lcd.write(byte(1));
    break;

    case 4: //Thermometer Icon
      lcd.createChar(3, thermometerIcon);

      lcd.setCursor(column, row);
      lcd.write(byte(3));
    break;

    case 5: //Reset Icon
      lcd.createChar(2, resetArrow);

      lcd.setCursor(column, row);
      lcd.write(byte(2));
    break;

    case 6: //Hour Glass Up
      lcd.createChar(3, hourGlassUp);

      lcd.setCursor(column, row);
      lcd.write(byte(3));
    break;

    case 7: //Hour Glass Down
      lcd.createChar(3, hourGlassDown);

      lcd.setCursor(column, row);
      lcd.write(byte(3));
    break;

    case 8: //Filled LCD Cell
      lcd.createChar(5, fullBar);

      lcd.setCursor(column, row);
      lcd.write(byte(5));
    break;

    case 9: //Semi-filled LCD Cell
      lcd.createChar(6, halfBar);

      lcd.setCursor(column, row);
      lcd.write(byte(6));
    break;

    case 10: //Saved Icon
      lcd.createChar(2, savedIcon);

      lcd.setCursor(column, row);
      lcd.write(byte(2));
    break;

    case 11: //Forward Arrow
      lcd.createChar(1, forwardArrow);

      lcd.setCursor(column, row);
      lcd.write(byte(1));
    break;

    case 12: // Clock Icon
      lcd.createChar(2, clockIcon);

      lcd.setCursor(column, row);
      lcd.write(byte(2));
    break;

    case 13: // Water Level Icon
      lcd.createChar(6, waterLevelIcon);

      lcd.setCursor(column, row);
      lcd.write(byte(6));
    break;

    case 14: // Back Arrow
      lcd.createChar(2, backArrow);

      lcd.setCursor(column, row);
      lcd.write(byte(2));
    break;

    case 15: // Check Icon
      lcd.createChar(5, checkIcon);

      lcd.setCursor(column, row);
      lcd.write(byte(5));
    break;

    case 16: // Cross Icon
      lcd.createChar(6, crossIcon);

      lcd.setCursor(column, row);
      lcd.write(byte(6));
    break;

    case 17: // Day Icon
      lcd.createChar(4, dayIcon);

      lcd.setCursor(column, row);
      lcd.write(byte(4));
    break;

    case 18: // Night Icon
      lcd.createChar(4, nightIcon);

      lcd.setCursor(column, row);
      lcd.write(byte(4));
    break;

    case 19: // Light Bulb On
      lcd.createChar(5, lightBulbOn);

      lcd.setCursor(column, row);
      lcd.write(byte(5));
    break;

    case 20: // Light Bulb Off
      lcd.createChar(5, lightBulbOff);

      lcd.setCursor(column, row);
      lcd.write(byte(5));
    break;

    case 21: // Exclamation Mark Icon
      lcd.createChar(5, exclaimMark);

      lcd.setCursor(column, row);
      lcd.write(byte(5));
    break;
  }
}
