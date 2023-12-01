// HOME SCREEN
void titleScreen() {
  char displayTime[timeBuffer];
  char displayWaterLevel[normalBuffer];
  char displayTemp[normalBuffer];
  dayDisplay = rtc.getDOWStr(FORMAT_SHORT);
  timeDisplay = rtc.getTimeStr(FORMAT_SHORT);

  sprintf(displayWaterLevel, "%d%%  ", waterPercentage);
  sprintf(displayTemp, "%0d%cC", celsius, degreeSymbol);

  while (true) {
    getKeyInput();
    checkWaterDistance();
    checkTemps();
    checkFeedSchedule();
    
    checkChangeSchedule();
    if (currentRegulationMode == regulate) {
      regulateMode();
    } else if (currentRegulationMode == change) {
      changeOnce();
    }

    // Water Level Display
    printIcon(13, 1, 0);
    printText(displayWaterLevel, 3, 0);

   // Time Tracking Display
    printIcon(12, 8, 0);
    printText(String(timeDisplay), 10, 0);

   // Heater Status Display
    if (oldState == 1) {
      printIcon(19, 1, 1);
      printText("On", 3, 1);
    } else {
      printIcon(20, 1, 1);
      printText("Off", 3, 1);
    }

   // Temperature Display
    printIcon(4, 8, 1);
    printText(displayTemp, 10, 1);

    // Update Data
    if (millis() - previousUpdateMillis >= 1000) {
      sprintf(displayWaterLevel, "%d%%  ", waterPercentage);
      sprintf(displayTemp, "%0d%cC", celsius, degreeSymbol);
      previousUpdateMillis = millis();
    }

    if (selectButton || upButton || downButton || backButton) {
      lcd.clear();
      mainNav = 1;
      currentScreen = menu;
      return;
    }
  }
}

// MAIN MENU
void mainMenu() {
  bool updateScreen = true;

  while (true) {
    getKeyInput();
    lcd.setCursor(0, 0);
    lcd.print(F("Menu:"));

    if (updateScreen == true) {
      clearLine(1);
      updateScreen = false;

      switch (mainNav) {
        case 1:
          lcd.setCursor(2, 1);
          lcd.print(F("Feed"));

          printIcon(1, 0, 1);
          break;

        case 2:
          lcd.setCursor(2, 1);
          lcd.print(F("Water"));

          printIcon(2, 0, 1);
          break;

        case 3:
          lcd.setCursor(2, 1);
          lcd.print(F("Temperature"));

          printIcon(2, 0, 1);
          break;

        case 4:
          lcd.setCursor(2, 1);
          lcd.print(F("Brightness"));

          printIcon(2, 0, 1);
          break;

        case 5:
          lcd.setCursor(2, 1);
          lcd.print(F("Time"));

          printIcon(2, 0, 1);
          break;

        case 6:
          lcd.setCursor(2, 1);
          lcd.print(F("Sleep"));

          printIcon(3, 0, 1);
          break;
      }
    }

    if (selectButton) {
      switch (mainNav) {
        case 1:
          lcd.clear();
          subNav = 1;
          currentScreen = feed;
          return;
          break;

        case 2:
          lcd.clear();
          subNav = 1;
          currentScreen = water;
          return;
          break;

        case 3:
          lcd.clear();
          subNav = 1;
          currentScreen = temp;
          return;
          break;

        case 4:
          lcd.clear();
          subNav = 1;
          currentScreen = brightness;
          return;
          break;

        case 5:
          lcd.clear();
          subNav = 1;
          currentScreen = time;
          return;
          break;

        case 6:
          while (true) {
            lcd.clear();
            setBright(0);
            getKeyInput();

            if (selectButton || upButton || downButton || backButton) {
              currentScreen = title;
              delay(100);
              setBright(defaultBrightness);
              return;
            }
          }
          break;
      }
    }

    if (downButton && mainNav != 6) {
      mainNav++;
      updateScreen = true;
    }

    if (upButton && mainNav != 1) {
      mainNav--;
      updateScreen = true;
    }

    if (backButton) {
      lcd.clear();
      currentScreen = title;
      return;
    }
  }
}

// BRIGHTNESS SETTING
void setBrightnessScreen() {
  bool updateScreen = true;
  char buffer[normalBuffer];
  int brightnessValue;

  setBrightness = defaultBrightness;
  mainNav = 4;

  printIcon(0, 0, 0);
  lcd.setCursor(2, 0);
  lcd.print(F("Set Brightness"));

  while (true) {
    getKeyInput();
    brightnessValue = setBrightness / 20;

    if (updateScreen == true) {
      clearLine(1);
      updateScreen = false;

      lcd.setCursor(0, 1);
      sprintf(buffer, "%02d", brightnessValue);
      lcd.print(buffer);

      printBar(brightnessValue);

      switch (brightnessValue) {
        case 0:
          printIcon(3, 3, 1);
          break;

        case 10:
          printIcon(1, 3, 1);
          break;

        default:
          printIcon(2, 3, 1);
          break;
      }
    }

    if (selectButton) {
      lcd.clear();
      defaultBrightness = setBrightness;
      printBar(brightnessValue);
      setBright(defaultBrightness);

      currentScreen = menu;
      return;
    }

    if (backButton) {
      lcd.clear();
      setBright(defaultBrightness);
      printBar(brightnessValue);
      currentScreen = menu;
      return;
    }

    if (downButton && brightnessValue != 0) {
      updateScreen = true;
      setBrightness = setBrightness - 20;

      setBright(setBrightness);
      printBar(brightnessValue);
    }

    if (upButton && brightnessValue != 10) {
      updateScreen = true;
      setBrightness = setBrightness + 20;

      setBright(setBrightness);
      printBar(brightnessValue);
    }
  }
}
