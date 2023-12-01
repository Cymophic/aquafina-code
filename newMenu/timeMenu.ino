// TIME SETTING MENU
void timeScreen() {
  bool updateScreen = true;
  mainNav = 5;

  while (true) {
    getKeyInput();
    printIcon(0, 0, 0);
    lcd.setCursor(2, 0);
    lcd.print(F("Time:"));

    if (updateScreen == true) {
      clearLine(1);
      updateScreen = false;

      switch (subNav) {
        case 1: 
          lcd.setCursor(2, 1);
          lcd.print(F("Display"));

          printIcon(1, 0, 1);
        break;

        case 2:
          lcd.setCursor(2, 1);
          lcd.print(F("Adjust"));

          printIcon(3, 0, 1);
        break;
      }
    }

    if (selectButton) {
      switch (subNav) {
        case 1:
          lcd.clear();
          subNav = 1;
          currentScreen = displayTime;
          return;
        break;
          
        case 2:
          lcd.clear();
          subNav = 2;
          currentScreen = adjustTime;
          return;
        break;
      }
    }

    if (downButton && subNav != 2) {
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

// DISPLAY TIME SET IN RTC
void displayTimeScreen() {
  bool updateScreen = true;
  String oldTime;

  printIcon(0, 0, 0);
  lcd.setCursor(2, 0);
  lcd.print(F("Current Time"));

  while (true) {
    oldTime = rtc.getTimeStr(FORMAT_SHORT);
    getKeyInput();

    if (isMorning() == true) printIcon(17, 1, 1);
    else printIcon(18, 1, 1);
    
    printIcon(12, 8, 1);
    lcd.setCursor(3, 1); lcd.print(rtc.getDOWStr(FORMAT_SHORT));
    lcd.setCursor(10, 1); lcd.print(rtc.getTimeStr(FORMAT_SHORT));

    if (backButton) {
      lcd.clear();
      currentScreen = time; 
      return;
    }

    //For Debugging
    if (oldTime != rtc.getTimeStr(FORMAT_SHORT)) {
      Serial.print(rtc.getDOWStr(FORMAT_SHORT));
      Serial.print(F(" | "));
      Serial.println(rtc.getTimeStr(FORMAT_SHORT));
    }
  }
}

// SET TIME MENU
void adjustTimeSettingsScreen() {
  bool updateScreen = true;
  printIcon(0, 0, 0);
  printText("Adjust Settings", 2, 0);

  while (true) {
    getKeyInput();

    if (updateScreen == true) {
      clearLine(1);
      updateScreen = false;

      switch (setTimeNav) {
        case 1:
          getKeyInput();
          printText("Set Time", 2, 1);

          printIcon(1, 0, 1);
        break;

        case 2: 
          getKeyInput();
          printText("Set Day", 2, 1);

          printIcon(3, 0, 1);
        break;
      }
    }

    if (selectButton) {
      switch (setTimeNav) {
        case 1:
          lcd.clear();
          setTimeNav = 1;
          currentScreen = setTime; 
          return;
        break;

        case 2:
          lcd.clear();
          setTimeNav = 2;
          currentScreen = setDay; 
          return;
        break;
      }
    }

    if (downButton && setTimeNav != 2) {
      setTimeNav++;
      updateScreen = true;
    }

    if (upButton && setTimeNav != 1) {
      setTimeNav--;
      updateScreen = true;
    }

    if (backButton) {
      lcd.clear();
      setTimeNav = 1;
      currentScreen = time; 
      return;
    }
  }
}

// SET TIME SETTINGS
void setTimeScreen() {
  bool updateScreen = true;
  char displaySetHour[timeBuffer]; 
  char displaySetMin[timeBuffer]; 
  char displaySetSec[timeBuffer];

  enum state {hour, minute, seconds, done};
  enum state currentState = hour;
  shouldBlink = true;

  uint8_t maxHour = 23; uint8_t maxMin = 59; uint8_t maxSec = 59;
  uint8_t setHour; uint8_t setMin; uint8_t setSec;

  setHour = 0; setMin = 0; setSec = 0;

  printText("Set Time", 0, 0);

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
            blinkText(String(displaySetHour), 2, 1);
          break;

          case minute:
            blinkText(String(displaySetMin), 5, 1);
          break;

          case seconds:
            blinkText(String(displaySetSec), 8, 1);
          break;
        }

        previousBlinkMillis = millis();
      }
    }

    if (updateScreen == true) {
      updateScreen = false;
      
      printIcon(12, 0, 1);

      sprintf(displaySetHour, "%02d", setHour);
      lcd.setCursor(2, 1); lcd.print(displaySetHour);
      printText(":", 4, 1);
      sprintf(displaySetMin, "%02d", setMin);
      lcd.setCursor(5, 1); lcd.print(displaySetMin);
      printText(":", 7, 1);
      sprintf(displaySetSec, "%02d", setSec);
      lcd.setCursor(8, 1); lcd.print(displaySetSec);
    }
    
    if (selectButton) {
      switch (currentState) {
        case hour:
          currentState = minute;
          updateScreen = true;
        break;

        case minute:
          currentState = seconds;
          updateScreen = true;
        break;

        case seconds:
          currentState = done;
          updateScreen = true;
        break;

        case done:
          //Save and Reset Local Variables
          saveSetTime(setHour, setMin, setSec);

          lcd.clear();
          lcd.setCursor(2, 0);

          for (int x = 0; x <= 3; x++) {
            printLoadingProcess("Saving", 0, 0);
            delay(defaultLoadingDuration);
          }

          lcd.clear();
          printText("Saved", 2, 0);
          printIcon(10, 0, 0);
          delay(defaultDuration); 

          lcd.clear();
          currentScreen = adjustTime;
          return; 
        break;
      }
    }

    if (backButton) {
      switch (currentState) {
        case hour:
          lcd.clear();
          currentScreen = adjustTime; 
          return;
        break;

        case minute:
          currentState = hour;
          updateScreen = true;
        break;

        case seconds:
          currentState = minute;
          updateScreen = true;
        break;
        
        case done:
          for (int x = 11; x <= 15; x++) 
            clearSquare(x, 1);
          currentState = seconds;
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

        case seconds:
          if (setSec == maxSec)
            setSec = 0;
          else
            setSec++;
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

        case seconds:
          if (setSec == 0)
            setSec = maxSec;
          else
            setSec--;
        break;
      }
    }
  }
}

// SET DAY SETTINGS
void setDayScreen() {
  bool updateScreen = true;
  char displayDay[dayBuffer];
  char* scheduleDays[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
  const uint8_t maxDayNumber = 7;
  const uint8_t minDayNumber = 0;
  uint8_t dayNumber = 0;

  printText("Set Day", 0, 0);

  while (true) {
    getKeyInput();
    
    sprintf(displayDay, "%s", scheduleDays[dayNumber]);
    if (updateScreen == true) {
      clearLine(1);
      updateScreen = false;

      lcd.setCursor(0, 0);
      printIcon(17, 0, 1);
      lcd.setCursor(2, 1); lcd.print(displayDay);
    }

    if (selectButton) {
      //Save and Reset Local Variables
      saveSetDay(dayNumber, displayDay);

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

      lcd.clear();
      currentScreen = adjustTime; 
      return;
    }

    if (backButton) {
      lcd.clear();
      currentScreen = adjustTime; 
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
