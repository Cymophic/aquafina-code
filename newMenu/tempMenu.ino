// TEMPERATURE MENU
void tempScreen() {
  bool updateScreen = true;

  while (true) {
    getKeyInput();

    printIcon(0, 0, 0);
    lcd.setCursor(2, 0);
    lcd.print(F("Temperature:"));

    if (updateScreen == true) {
      clearLine(1);
      updateScreen = false;
    
      switch (subNav) {
        case 1: 
          lcd.setCursor(2, 1);
          lcd.print(F("Check Temps"));

          printIcon(1, 0, 1);
        break;

        case 2: 
          lcd.setCursor(2, 1);
          lcd.print(F("Optimal Temp"));

          printIcon(2, 0, 1);
        break;

        case 3: 
          lcd.setCursor(2, 1);
          lcd.print(F("Toggle Heater"));

          printIcon(3, 0, 1);
        break;
      }
    }

    if (selectButton) {
      switch (subNav) {
        case 1:
          lcd.clear();
          currentScreen = checkTemp; 
          return;
        break;

        case 2:
          lcd.clear();
          currentScreen = setTemp; 
          return;
        break;

        case 3:
          lcd.clear();
          currentScreen = toggleControl; 
          return;
        break;
      }
    }

    if (downButton && subNav != 3) {
      subNav++;
      updateScreen = true;
    }

    if (upButton && subNav != 1) {
      subNav--;
      updateScreen = true;
    }

    if (backButton) {
      lcd.clear();
      currentScreen = menu; 
      mainNav = 3;
      return;
    }
  }
}

// CHECK WATER TEMPERATURE 
void tempCheckScreen() {
  char celsiusBuffer[10];
  char fahrenheitBuffer[10];
  char temperatureData[60];
  char displayTemp[normalBuffer];
  char displayTempSerial[normalBuffer];

  bool switchUnit = 0;
  int oldCelsius = celsius;
  int oldFahrenheit = fahrenheit;

  printIcon(0, 0, 0);
  lcd.setCursor(2, 0);
  lcd.print(F("Current Temp"));

  for (int x = 0; x <= 4; x++) {
    printLoadingProcess("Reading", 0, 1);
    delay(300);
  }

  while (true) {
    getKeyInput();
    checkTemps();
    dtostrf(celsius, 5, 1, celsiusBuffer);
    dtostrf(fahrenheit, 5, 1, fahrenheitBuffer);

    if (isCold() == true) printIcon(19, 15, 0);
    else printIcon(20, 15, 0);

    if (celsius == -127) {
      printIcon(16, 0, 1);
      lcd.setCursor(2, 1); lcd.print(F("Not Connected"));
    }

    else {
      if (switchUnit == 0) {
        sprintf(temperatureData, "%s%cC", celsiusBuffer, degreeSymbol);
      }

      else if (switchUnit == 1) {
        sprintf(temperatureData, "%s%cF", fahrenheitBuffer, degreeSymbol);
      }

      printIcon(4, 0, 1);
      lcd.setCursor(1, 1);
      lcd.print(temperatureData);
    }
    
    if (backButton) {
      lcd.clear();
      subNav = 1;
      currentScreen = temp; 
      return;
    }

    if (downButton && switchUnit == 0) {
      switchUnit = 1;
      clearLine(1);
    }

    if (upButton && switchUnit == 1) {
      switchUnit = 0;
      clearLine(1);
    }
  }
}

// SET HEATER TOGGLE TO AUTOMATIC OR MANUAL MODE
void toggleControlScreen() {
  printIcon(0, 0, 0);
  printText("Toggle Control", 2, 0);

  if (oldControl == 1) {
    currentControlState = automatic;
    checkTemps();
  }
  
  else if (oldControl == 0) currentControlState = manual;

  while (true) {
    getKeyInput();

    switch (currentControlState) {
      case automatic:
        printIcon(19, 0, 1);
        printText("Automatic", 2, 1);
      break;

      case manual:
        printIcon(20, 0, 1);
        printText("Manual", 2, 1);

        printIcon(11, 14, 1);
      break;
    }

    if (upButton) {
      clearLine(1);
      currentControlState = automatic;
    }

    if (downButton) {
      clearLine(1);
      currentControlState = manual;
    }

    if (selectButton && currentControlState == manual) {
      lcd.clear();
      currentScreen = toggleHeater; 
      return;
    }

    if (selectButton && currentControlState == automatic) {
      lcd.clear();
      checkTemps();

      currentControlState = automatic;
      oldControl = 1;

      subNav = 3;
      currentScreen = temp; 
      return;
    }
    
    if (backButton) {
      lcd.clear();

      if (oldControl == 1) currentControlState = automatic;
      else currentControlState = manual;

      subNav = 3;
      currentScreen = temp; 
      return;
    }
  }
}

// TOGGLE HEATERS IF SET TO MANUAL
void toggleHeaterScreen() {
  bool setToggle;
  checkTemps();

  printIcon(0, 0, 0);
  printText("Toggle Heater", 2, 0);
  
  while (true) {
    getKeyInput();

    switch (setToggle) {
      case on:
        printIcon(19, 0, 1);
        printText("Heater: On", 2, 1);
      break;

      case off:
        printIcon(20, 0, 1);
        printText("Heater: Off", 2, 1);
      break;
    }

    if (upButton && setToggle == off) {
      clearLine(1);
      setToggle = on;
    }

    if (downButton && setToggle == on) {
      clearLine(1);
      setToggle = off;
    }

    if (selectButton) {
      oldControl = 0;

      switch (setToggle) {
        case on:
          turnOnHeaters();
          currentHeaterState = on;
        break;

        case off:
          turnOffHeaters();
          currentHeaterState = off;
        break;
      }

      lcd.clear();
      subNav = 3;
      currentScreen = temp; 
      return;
    }

    if (backButton) {
      if (currentHeaterState == on) {
        turnOnHeaters();
      }

      if (currentHeaterState == off) {
        turnOffHeaters();
      }

      lcd.clear();
      subNav = 3;
      currentScreen = toggleControl; 
      return;
    }
  }
}

// TO CHANGE OPTIMAL TEMPERATURE RANGE MANUALLY
void setTempScreen() {
  uint16_t setOptimalTemp = optimalTemp;
  bool updateScreen = true;
  char displayOptimalTemp[normalBuffer];
  
  printText(F("Set Optimal Temp"), 0, 0);
  printIcon(4, 0, 1);

  while (true) {
    getKeyInput();

    if (updateScreen == true) {
      updateScreen = false;

      sprintf(displayOptimalTemp, "%0d%cC", setOptimalTemp, degreeSymbol);
      printText(displayOptimalTemp, 2, 1);
    }
    
    if (selectButton) {
      //Save and Reset Local Variables
      optimalTemp = setOptimalTemp;

      lcd.clear();
      lcd.setCursor(2, 0);

      for (int x = 0; x <= 3; x++) {
        printLoadingProcess("Saving", 0, 0);
        delay(defaultLoadingDuration);
      }
      lcd.clear();

      lcd.setCursor(2, 0);
      lcd.print(F("Saved"));
      printIcon(10, 0, 0);

      delay(defaultDuration);

      lcd.clear();
      subNav = 3;
      currentScreen = temp; 
      return;
    }

    if (backButton) {
      lcd.clear();
      subNav = 2;
      currentScreen = temp; 
      return;
    }

    if (upButton) {
      setOptimalTemp++;
      updateScreen = true;
    }

    if (downButton) {
      setOptimalTemp--;
      updateScreen = true;
    }
  }
}
