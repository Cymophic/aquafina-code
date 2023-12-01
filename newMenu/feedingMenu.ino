// FEED MENU
void feedScreen() {
  bool updateScreen = true;
  mainNav = 1;

  while (true) {
    getKeyInput();
    printIcon(0, 0, 0);
    lcd.setCursor(2, 0);
    lcd.print(F("Feed:"));

    if (updateScreen == true) {
      clearLine(1);
      updateScreen = false;

      switch (subNav) {
        case 1: 
          lcd.setCursor(2, 1);
          lcd.print(F("Feed Now"));

          printIcon(1, 0, 1);
        break;

        case 2:
          lcd.setCursor(2, 1);
          lcd.print(F("Schedule"));

          printIcon(2, 0, 1);
        break;

        case 3:
          lcd.setCursor(2, 1);
          lcd.print(F("Set Spin Speed"));

          printIcon(3, 0, 1);
        break;
      }
    }

    if (selectButton) {
      switch (subNav) {
        case 1:
          lcd.clear();
          subNav = 1;
          currentScreen = feedNow;
          return;
        break;
          
        case 2:
          lcd.clear();
          subNav = 2;
          currentScreen = setFeedFrequency;
          return;
        break;

        case 3:
          lcd.clear();
          subNav = 3;
          currentScreen = servoSpeedSetting;
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
      subNav = 1;
      currentScreen = menu; 
      return;
    }
  }
}

// MANUAL FEED PROCESS
void feedNowScreen() {
  currentServoState = turn; 
  previousSpinMillis = millis();
  
  while (true) {
    if (currentServoState == turn) {
      runFeedingServo();
      printLoadingProcess("Feeding", 0, 0);

      if (millis() - previousSpinMillis >= spinTime) {
        stopFeedingServo();
        currentServoState = feedStop;
      }
    }

    else {
      clearLine(0);
      printIcon(15, 0, 0);
      printText("Feeding Done", 2, 0);
      delay(1700);

      lcd.clear();
      currentScreen = feed; 
      return;
    }
  }
}

// SET NUMBER OF FEEDING TIMES PER DAY
void setFeedFrequencyScreen() {
  bool updateScreen = true;

  const int minFrequency = 1;
  const int maxFrequency = 5;
  
  feedingFrequency = 0;

  while (true) {
    getKeyInput();
    lcd.setCursor(0, 0);
    lcd.print(F("Set Frequency"));

    if (updateScreen == true) {
      clearLine(1);
      updateScreen = false;

      printIcon(5, 0, 1);
      lcd.setCursor(2, 1);
      lcd.print(feedingFrequency);
      
      if (feedingFrequency == minFrequency) {
        lcd.setCursor(4, 1);
        lcd.print(F("time"));
      }

      else if (feedingFrequency == maxFrequency) {
        lcd.setCursor(4, 1);
        lcd.print(F("times"));
      }

      else {
        lcd.setCursor(4, 1);
        lcd.print(F("times"));
      }

      if (feedingFrequency == 0) {
        clearLine(1);
        printIcon(2, 0, 1);
        lcd.setCursor(2, 1);
        lcd.print(F("- ----"));
      }
    }

    if (selectButton && feedingFrequency != 0) {
      lcd.clear();
      feedFreqNumber = 1;
      currentScreen = editFeedFrequency; 
      return;
    }

    if (backButton) {
      lcd.clear();
      currentScreen = feed; 
      return;
    }

    if (upButton && feedingFrequency == maxFrequency) {
      feedingFrequency = minFrequency;
      updateScreen = true;
    }

    else if (upButton) {
      feedingFrequency++;
      updateScreen = true;
    }

    if (downButton && feedingFrequency == minFrequency) {
      feedingFrequency = maxFrequency;
      updateScreen = true;
    }

    if (downButton && feedingFrequency == 0) {
      feedingFrequency = maxFrequency;
      updateScreen = true;
    }

    else if (downButton) {
      feedingFrequency--;
      updateScreen = true;
    }
  }
}

// EDIT SET FREQUENCIES
void editFeedFrequencyScreen() {
  bool updateScreen = true;
  char displayFrequencies[normalBuffer];
  numberOfFeedFrequencies = feedingFrequency;

  int maxFreq = numberOfFeedFrequencies;
  int minFreq = 1;

  while (true) {
    getKeyInput();
    lcd.setCursor(0, 0);
    lcd.print(F("Edit Frequency"));

    if (updateScreen == true) {
      clearLine(1);
      updateScreen = false;

      if (feedFreqNumber == minFreq && feedingFrequency == minFreq) {
        printIcon(11, 0, 1);
      }

      else if (feedFreqNumber == minFreq) {
        printIcon(3, 0, 1);
      }

      else if (feedFreqNumber == maxFreq) {
        printIcon(1, 0, 1);
      }

      else {
        printIcon(2, 0, 1);
      }

      lcd.setCursor(2, 1);
      sprintf(displayFrequencies, "Freq. #%d", feedFreqNumber);
      lcd.print(displayFrequencies);
    }

    if (selectButton) {
      lcd.clear();
      currentScreen = setFeedSchedule;
      return; 
    }

    if (backButton) {
      lcd.clear();
      currentScreen = setFeedFrequency; 
      return;
    }

    if (upButton && feedFreqNumber != maxFreq) {
      updateScreen = true;
      feedFreqNumber++;
    }
    
    if (downButton && feedFreqNumber != minFreq) {
      updateScreen = true;
      feedFreqNumber--;
    }
  }
}

// SET TIME PER FREQUENCY
void setFeedScheduleScreen() {
  bool updateScreen = true;
  char displayHour[timeBuffer], displayMin[timeBuffer];
  char scheduleNumber[phraseBuffer];
  enum state {hour, minute, done};
  enum state currentState = hour;
  shouldBlink = true;

  uint8_t maxMin = 59; uint8_t maxHour = 23;
  uint8_t setMin; uint8_t setHour;

  switch (feedFreqNumber) {
    default:
      setMin = 0; setHour = 0;
    break;
  }

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
            blinkText(String(displayHour), 2, 1);
          break;

          case minute:
            blinkText(String(displayMin), 5, 1);
          break;
        }

        previousBlinkMillis = millis();
      }
    }

    sprintf(scheduleNumber, "Set #%d Schedule", feedFreqNumber);
    lcd.setCursor(0, 0);
    lcd.print(scheduleNumber);

    if (updateScreen == true) {
      updateScreen = false;
      
      printIcon(12, 0, 1);
      lcd.setCursor(4, 1);
      lcd.print(F(":"));

      sprintf(displayHour, "%02d", setHour);
      lcd.setCursor(2, 1); lcd.print(displayHour);
      sprintf(displayMin, "%02d", setMin);
      lcd.setCursor(5, 1); lcd.print(displayMin);
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
          saveTimeAsChar(feedFreqNumber, setHour, setMin);

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

          if (numberOfFeedFrequencies == 1) {
            lcd.clear();
            currentScreen = feed;
            return; 
          }

          else {
            lcd.clear();
            currentScreen = editFeedFrequency;
            return; 
          }
        break;
      }
    }

    if (backButton) {
      switch (currentState) {
        case hour:
          lcd.clear();
          currentScreen = editFeedFrequency; 
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

// SET SPEED PER ROTATION
void setServoSpeedScreen() {
  bool updateScreen = true;
  char buffer[normalBuffer];
  int oldSpeed = setSpeed;

  printIcon(0, 0, 0);
  lcd.setCursor(2, 0);
  lcd.print(F("Set Speed"));

  while (true) {
    getKeyInput();

    if (updateScreen == true) {
      clearLine(1);
      updateScreen = false;

      lcd.setCursor(0, 1);
      sprintf(buffer, "%02d", setSpeed);
      lcd.print(buffer);

      printBar(setSpeed);

      switch (setSpeed) {
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
      spinSpeed = setSpinSpeed(setSpeed);

      for (int x = 0; x <= 3; x++) {
        printLoadingProcess("Saving", 0, 0);
        delay(defaultLoadingDuration);
      }

      lcd.clear();

      lcd.setCursor(2, 0);
      lcd.print(F("Saved"));
      printIcon(10, 0, 0);

      delay(700);

      currentScreen = feed;
      return;
    }

    if (backButton) {
      lcd.clear();
      setSpeed = oldSpeed;
      currentScreen = feed;
      return;
    }

    if (downButton && setSpeed != 0) {
      updateScreen = true;
      setSpeed = setSpeed - 1;
      printBar(setSpeed);
    }

    if (upButton && setSpeed != 10) {
      updateScreen = true;
      setSpeed = setSpeed + 1;
      printBar(setSpeed);
    }
  }
}