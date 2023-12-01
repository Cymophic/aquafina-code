// WATER-CHANGING MENU
void waterScreen() {
  bool updateScreen = true;

  while (true) {
    getKeyInput();
    printIcon(0, 0, 0);
    lcd.setCursor(2, 0);
    lcd.print(F("Water:"));

    if (updateScreen == true) {
      clearLine(1);
      updateScreen = false;

      switch (subNav) {
        case 1:
          lcd.setCursor(2, 1);
          lcd.print(F("Check Level"));

          printIcon(1, 0, 1);
        break;

        case 2: 
          lcd.setCursor(2, 1);
          lcd.print(F("Change Now"));

          printIcon(2, 0, 1);
        break;

        case 3:
          lcd.setCursor(2, 1);
          lcd.print(F("Settings"));

          printIcon(2, 0, 1);
        break;

        case 4:
          lcd.setCursor(2, 1);
          lcd.print(F("Schedule"));

          printIcon(2, 0, 1);
        break;

        case 5:
          lcd.setCursor(2, 1);
          lcd.print(F("Manual Drain"));

          printIcon(2, 0, 1);
        break;

        case 6:
          lcd.setCursor(2, 1);
          lcd.print(F("Manual Fill"));

          printIcon(3, 0, 1);
        break;
      }
    }

    if (selectButton) {
      switch (subNav) {
        case 1:
          lcd.clear();
          subNav = 1;
          currentScreen = checkWaterLevel; 
          return;
        break;

        case 2:
          lcd.clear();
          subNav = 2;
          currentScreen = changeNow; 
          return;
        break;

        case 3:
          lcd.clear();
          subNav = 3;
          currentScreen = setWaterLevel;
          return;
        break;

        case 4:
          lcd.clear();
          subNav = 4;
          currentScreen = setChangeFrequency;
          return;
        break;

        case 5:
          lcd.clear();
          subNav = 5;
          currentScreen = manualDrain;
          return;
        break;

        case 6:
          lcd.clear();
          subNav = 6;
          currentScreen = manualFill;
          return;
        break;
      }
    }

    if (downButton && subNav != 6) {
      subNav++;
      updateScreen = true;
    }

    if (upButton && subNav != 1) {
      subNav--;
      updateScreen = true;
    }

    if (backButton) {
      lcd.clear();
      mainNav = 2;
      currentScreen = menu; 
      return;
    }
  }
}

// CHECK WATER LEVEL
void checkWaterLevelScreen() {
  unsigned long currentWaterMillis;
  unsigned long previousWaterMillis;
  char displayLevel[phraseBuffer];
  int oldWaterLevel = waterPercentage;

  printIcon(0, 0, 0);
  lcd.setCursor(2, 0);
  lcd.print(F("Current Level"));
  
  for (int x = 0; x < 2; x++) {
    printLoadingProcess("Reading", 0, 1);
    delay(300);
  }

  clearLine(1);
  printIcon(13, 0, 1);

  while (true) {
    getKeyInput();
    currentWaterMillis = millis();
    sprintf(displayLevel, "%0d%% | %0d cm  ", waterPercentage, waterLevel);

    if (currentWaterMillis - previousWaterMillis >= 1000) {
      checkWaterDistance();
      previousWaterMillis = currentWaterMillis;
    }

    /* Updates only when water level has changed */
    if (oldWaterLevel != waterPercentage) {
      clearLine(1);
      oldWaterLevel = waterPercentage;
      printIcon(13, 0, 1);
      
      sprintf(displayLevel, "%0d%%  ", waterPercentage);
      Serial.print(F("Current Water Level: "));
      Serial.println(displayLevel);

      lcd.setCursor(2, 1); lcd.print(displayLevel);
    }

    else {
      lcd.setCursor(2, 1); lcd.print(displayLevel);
    }

    if (backButton) {
      lcd.clear();
      currentScreen = water; 
      return;
    }
  }
}

// SET DRAIN AND FILL WATER LEVELS FOR WATER-CHANGING
void setWaterLevelScreen() {
  bool updateScreen = true;
  char displayWaterLevel[normalBuffer];
  enum state {height, drain, fill, done};
  enum state currentLevelState = height;
  const uint8_t maxSetLevel = 100;
  const uint8_t minSetLevel = 0;
  const uint8_t changeValue = 1;

  uint8_t setAquariumHeight = aquariumHeight;
  uint8_t setDrainLevel = maxDrainLevel;
  uint8_t setFillLevel = maxFillLevel;

  while (true) {
    getKeyInput();

    if (updateScreen == true) {
      updateScreen = false;

      switch (currentLevelState) {
        case height:
          clearLine(0);
          lcd.setCursor(0, 0);
          lcd.print(F("Set Tank Height"));  
          sprintf(displayWaterLevel, "%0d cm  ", setAquariumHeight);
        break;

        case drain:
          clearLine(0);
          lcd.setCursor(0, 0);
          lcd.print(F("Set Drain Level"));  
          sprintf(displayWaterLevel, "%0d%%  ", setDrainLevel);
        break;

        case fill:
          clearLine(0);
          lcd.setCursor(0, 0);
          lcd.print(F("Set Fill Level"));
          sprintf(displayWaterLevel, "%0d%%  ", setFillLevel);
        break;

        case done:
          
        break;
      }

      clearLine(1);
      printIcon(13, 0, 1);
      lcd.setCursor(2, 1);
      lcd.print(displayWaterLevel);
    }

    if (selectButton) {
      switch (currentLevelState) {
        case height:
          currentLevelState = drain;
          updateScreen = true;
        break;

        case drain:
          currentLevelState = fill;
          updateScreen = true;
        break;

        case fill:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(F("Save now?"));

          lcd.setCursor(11, 1);
          lcd.print(F("Yes"));
          printIcon(11, 15, 1);

          lcd.setCursor(2, 1);
          lcd.print(F("No"));
          printIcon(14, 0, 1);

          currentLevelState = done;
        break;

        case done:
          //Save and Reset Local Variables
          aquariumHeight = setAquariumHeight;
          maxDrainLevel = setDrainLevel;
          maxFillLevel = setFillLevel;

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

          delay(700);   

          lcd.clear();                             
          currentScreen = water;
          return; 
        break;
      }
    }

    if (backButton) {
      switch (currentLevelState) {
        case height:
          lcd.clear();
          currentScreen = water; 
          return;
        break;

        case drain:
          currentLevelState = height;
          updateScreen = true;
          return;
        break;

        case fill:       
          currentLevelState = drain;
          updateScreen = true;
        break;

        case done:
          currentLevelState = fill;
          updateScreen = true;
        break;
      }
    }

    switch (currentLevelState) {
      case height:
        if (upButton && setAquariumHeight == maxSetLevel) {
          setAquariumHeight = minSetLevel;
          updateScreen = true;
        }

        else if (upButton) {
          setAquariumHeight = setAquariumHeight + changeValue;
          updateScreen = true;
        }

        if (downButton && setAquariumHeight == minSetLevel) {
          setAquariumHeight = maxSetLevel;
          updateScreen = true;
        }

        else if (downButton) {
          setAquariumHeight = setAquariumHeight - changeValue;
          updateScreen = true;
        }
      break;

      case drain:
        if (upButton && setDrainLevel == maxSetLevel) {
          setDrainLevel = minSetLevel;
          updateScreen = true;
        }

        else if (upButton) {
          setDrainLevel = setDrainLevel + changeValue;
          updateScreen = true;
        }

        if (downButton && setDrainLevel == minSetLevel) {
          setDrainLevel = maxSetLevel;
          updateScreen = true;
        }

        else if (downButton) {
          setDrainLevel = setDrainLevel - changeValue;
          updateScreen = true;
        }
      break;

      case fill:
        if (upButton && setFillLevel == maxSetLevel) {
          setFillLevel = minSetLevel;
          updateScreen = true;
        }

        else if (upButton) {
          setFillLevel = setFillLevel + changeValue;
          updateScreen = true;
        }

        if (downButton && setFillLevel == minSetLevel) {
          setFillLevel = maxSetLevel;
          updateScreen = true;
        }

        else if (downButton) {
          setFillLevel = setFillLevel - changeValue;
          updateScreen = true;
        }
      break;
    }
  }
}

// MANUAL WATER-CHANGE PROCESS
void changeNowScreen() {
  char displayWaterLevel[normalBuffer];
  currentChangingState = drain; 
  previousMillis02 = millis();

  while (true) {
    getKeyInput();

    if (currentChangingState == drain) {
      checkWaterDistance();
      drainPumpOn();
      printLoadingProcess("Draining", 0, 0);

      if (waterPercentage <= maxDrainLevel) {
        printIcon(15, 0, 0);
        printText("Draining Done", 2, 0);

        delay(2000);

        lcd.clear();
        previousMillis02 = millis();
        currentChangingState = fill;
        oneTimeUpdate = true;
      }

      if (millis() - previousMillis02 >= 1000) {
        previousMillis02 = millis();
        sprintf(displayWaterLevel, "%0d%%  ", waterPercentage);
        Serial.println(waterPercentage);
        printIcon(13, 0, 1);
        lcd.setCursor(2, 1);
        lcd.print(displayWaterLevel); 
      }
    }

    if (currentChangingState == fill) {
      fillPumpOn();
      printLoadingProcess("Filling", 0, 0);

      if (oneTimeUpdate == true) {
        drainPumpOff();
        oneTimeUpdate = false;
      }

      if (waterPercentage >= maxFillLevel) {
        fillPumpOff();
        printIcon(15, 0, 0);
        printText("Filling Done", 2, 0);
        
        delay(1000);

        lcd.clear();
        currentChangingState = changeStop;
      }

      if (millis() - previousMillis02 >= 1000) {
        checkWaterDistance();
        previousMillis02 = millis();
        sprintf(displayWaterLevel, "%0d%%  ", waterPercentage);
        Serial.println(waterPercentage);
        printIcon(13, 0, 1);
        lcd.setCursor(2, 1);
        lcd.print(displayWaterLevel);
      }
    }

    if (currentChangingState == changeStop) {
      clearLine(1);
      printText("Water Changed", 2, 0);
      printIcon(5, 0, 0);
      
      delay(1700);
    
      lcd.clear();
      oneTimeUpdate = true;
      currentScreen = water;
      return;
    }

    if (backButton) {
      drainPumpOff();
      fillPumpOff();

      lcd.clear(); 
      printIcon(21, 0, 0);
      printText("Cancelled", 2, 0);
      currentChangingState = changeStop;

      delay(1000);

      lcd.clear();
      currentScreen = water; 
      return;
    }
  }
}

// SET NUMBER OF WATER-CHANGING TIMES 
void setChangeFrequencyScreen() {
  bool updateScreen = true;

  const int minFrequency = 1;
  const int maxFrequency = 5;
  
  changingFrequency = 0;

  while (true) {
    getKeyInput();

    if (updateScreen == true) {
      clearLine(1);
      updateScreen = false;

      lcd.setCursor(0, 0);
      lcd.print(F("Set Frequency"));
      printIcon(5, 0, 1);
      lcd.setCursor(2, 1);
      lcd.print(changingFrequency);
      
      if (changingFrequency == minFrequency) {
        lcd.setCursor(4, 1);
        lcd.print(F("time"));
      }

      else if (changingFrequency == maxFrequency) {
        lcd.setCursor(4, 1);
        lcd.print(F("times"));
      }

      else {
        lcd.setCursor(4, 1);
        lcd.print(F("times"));
      }

      if (changingFrequency == 0) {
        clearLine(1);
        printIcon(2, 0, 1);
        lcd.setCursor(2, 1);
        lcd.print(F("- ----"));
      }
    }

    if (selectButton && changingFrequency != 0) {
      lcd.clear();
      currentScreen = editChangeFrequency; 
      return;
    }

    if (backButton) {
      lcd.clear();
      currentScreen = water; 
      return;
    }

    if (upButton && changingFrequency == maxFrequency) {
      changingFrequency = minFrequency;
      updateScreen = true;
    }

    else if (upButton) {
      changingFrequency++;
      updateScreen = true;
    }

    if (downButton && changingFrequency == minFrequency) {
      changingFrequency = maxFrequency;
      updateScreen = true;
    }

    else if (downButton && changingFrequency == 0) {
      changingFrequency = maxFrequency;
      updateScreen = true;
    }

    else if (downButton) {
      changingFrequency--;
      updateScreen = true;
    }
  }
}

// EDIT SET FREQUENCIES
void editChangeFrequencyScreen() {
  bool updateScreen = true;
  char displayFrequencies[normalBuffer];
  numberOfChangeFrequencies = changingFrequency;

  int maxFreq = numberOfChangeFrequencies;
  int minFreq = 1;

  while (true) {
    getKeyInput();
    lcd.setCursor(0, 0);
    lcd.print(F("Edit Frequency"));

    if (updateScreen == true) {
      clearLine(1);
      updateScreen = false;

      if (changeFreqNumber == minFreq && changingFrequency == minFreq) {
        printIcon(11, 0, 1);
      }

      else if (changeFreqNumber == minFreq) {
        printIcon(3, 0, 1);
      }

      else if (changeFreqNumber == maxFreq) {
        printIcon(1, 0, 1);
      }

      else {
        printIcon(2, 0, 1);
      }

      lcd.setCursor(2, 1);
      sprintf(displayFrequencies, "Day #%d", changeFreqNumber);
      lcd.print(displayFrequencies);
    }

    if (selectButton) {
      lcd.clear();
      currentScreen = setChangeSchedule;
      return; 
    }

    if (backButton) {
      lcd.clear();
      currentScreen = setChangeFrequency; 
      return;
    }

    if (upButton && changeFreqNumber != maxFreq) {
      updateScreen = true;
      changeFreqNumber++;
    }
    
    if (downButton && changeFreqNumber != minFreq) {
      updateScreen = true;
      changeFreqNumber--;
    }
  }
}

// EDIT SET FREQUENCIES
void setChangeScheduleScreen() {
  bool updateScreen = true;
  char scheduleNumber[phraseBuffer];
  char displayDay[dayBuffer];
  char* scheduleDays[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
  const uint8_t maxDayNumber = 7;
  const uint8_t minDayNumber = 0;
  uint8_t dayNumber = 0;

  while (true) {
    getKeyInput();

    sprintf(scheduleNumber, "Set Change #%d", changeFreqNumber);
    sprintf(displayDay, "%s", scheduleDays[dayNumber]);
    lcd.setCursor(0, 0); lcd.print(scheduleNumber);

    if (updateScreen == true) {
      clearLine(1);
      updateScreen = false;

      lcd.setCursor(0, 0);
      printIcon(17, 0, 1);
      lcd.setCursor(2, 1); lcd.print(displayDay);
    }

    if (selectButton) {
      //Save and Reset Local Variables
      saveDayToMemory(changeFreqNumber, displayDay);
      
      lcd.clear();
      currentScreen = setChangingTime;
      return; 
    }

    if (backButton) {
      lcd.clear();
      currentScreen = editChangeFrequency; 
      return;
    }

    if (upButton && dayNumber != maxDayNumber - 1) {
      updateScreen = true;
      dayNumber++;
    }
    
    if (downButton && dayNumber != minDayNumber) {
      updateScreen = true;
      dayNumber--;
    }
  }
}

// SET TIME PER FREQUENCY
void setChangingTimeScreen() {
  bool updateScreen = true;
  char displayChangeHour[timeBuffer], displayChangeMin[timeBuffer];
  char saveChangeTime[60];
  enum state {hour, minute, done};
  enum state currentState = hour;
  shouldBlink = true;

  uint8_t maxMin = 59; uint8_t maxHour = 23;
  uint8_t setMin; uint8_t setHour;

  setMin = 0; setHour = 0;

  while (true) {
    getKeyInput();
    
    if (idle == false) {
      shouldBlink = false;
      previousBlinkMillis = 0;
      updateScreen = true;
    } 
    
    else if (idle == true && millis() - previousMillis >= 1000) {
      shouldBlink = true;
    }

    if (shouldBlink == true) {
      if (previousBlinkMillis == 0 || millis() - previousBlinkMillis >= 500) {
        switch (currentState) {
          case hour:
            blinkText(String(displayChangeHour), 2, 1);
          break;

          case minute:
            blinkText(String(displayChangeMin), 5, 1);
          break;
        }

        previousBlinkMillis = millis();
      }
    }

    printText("Set Change Time", 0, 0);
  
    if (updateScreen == true) {
      updateScreen = false;
      
      printIcon(12, 0, 1);
      lcd.setCursor(4, 1);
      lcd.print(F(":"));

      sprintf(displayChangeHour, "%02d", setHour);
      lcd.setCursor(2, 1); lcd.print(displayChangeHour);
      sprintf(displayChangeMin, "%02d", setMin);
      lcd.setCursor(5, 1); lcd.print(displayChangeMin);
    }

    if (selectButton) {
      switch (currentState) {
        case hour:
          currentState = minute;
          updateScreen = true;
        break;

        case minute:
          for (int x = 2; x <= 15; x++) 
            clearSquare(x, 1);

          lcd.setCursor(2, 1);
          lcd.print(F("Save?"));
          printIcon(11, 15, 1);
          currentState = done;
        break;

        case done:
          //Save and Reset Local Variables
          concatenateTimeToMemory(changeFreqNumber, setHour, setMin);

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

          if (changingFrequency == 1) {
            lcd.clear();
            currentScreen = water;
            return; 
          }

          else {
            lcd.clear();
            currentScreen = editChangeFrequency;
            return; 
          }
        break;
      }
    }

    if (backButton) {
      switch (currentState) {
        case hour:
          lcd.clear();
          currentScreen = setChangeSchedule; 
          return;
        break;

        case minute:
          currentState = hour;
          updateScreen = true;
        break;
        
        case done:
          for (int x = 11; x <= 15; x++) 
            clearSquare(x, 1);
          currentState = minute;
          updateScreen = true;
        break;
      }
    }

    if (upButton) {
      updateScreen = true;

      switch (currentState) {
        case hour:
          if (setHour == maxHour)
            setHour = 0;
          else
            setHour++;
        break;

        case minute:
          if (setMin == maxMin)
            setMin = 0;
          else
            setMin++;
        break;
      }
    }

    if (downButton) {
      updateScreen = true;

      switch (currentState) {
        case hour:
          if (setHour == 0)
            setHour = maxHour;
          else
            setHour--;
        break;

        case minute:
          if (setMin == 0)
            setMin = maxMin;
          else
            setMin--;
        break;
      }
    }
  }
}

// MANUAL WATER-DRAINING PROCESS
void manualDrainScreen() {
  char displayWaterLevel[normalBuffer];
  currentChangingState = drain;
  previousMillis02 = millis();
  
  while (true) {
    getKeyInput();

    if (currentChangingState == drain) {
      drainPumpOn();
      checkWaterDistance();
      printLoadingProcess("Draining", 0, 0);

      if (millis() - previousMillis02 >= 1000) {
        previousMillis02 = millis();
        sprintf(displayWaterLevel, "%0d%%  ", waterPercentage);
        Serial.println(displayWaterLevel);
        printIcon(13, 0, 1);
        lcd.setCursor(2, 1);
        lcd.print(displayWaterLevel); 
      }
    }

    if (backButton) {
      drainPumpOff();
      printIcon(21, 0, 0);
      printText("Drain Stopped", 2, 0);
      currentChangingState = changeStop;

      delay(1000);

      lcd.clear();
      currentScreen = water; 
      return;
    }
  }
}

// MANUAL WATER-FILLING PROCESS
void manualFillScreen() {
  char displayWaterLevel[normalBuffer];
  currentChangingState = fill;
  previousMillis02 = millis();
  
  while (true) {
    getKeyInput();

    if (currentChangingState == fill) {
      fillPumpOn();
      checkWaterDistance();
      printLoadingProcess("Filling", 0, 0);
    }

    if (millis() - previousMillis02 >= 1000) {
      previousMillis02 = millis();
      sprintf(displayWaterLevel, "%0d%%  ", waterPercentage);
      Serial.println(displayWaterLevel);
      printIcon(13, 0, 1);
      lcd.setCursor(2, 1);
      lcd.print(displayWaterLevel); 
    }

    if (backButton) {
      fillPumpOff();
      printIcon(21, 0, 0);
      printText("Fill Stopped", 2, 0);
      currentChangingState = changeStop;

      delay(1000);

      lcd.clear();
      currentScreen = water; 
      return;
    }
  }
}
