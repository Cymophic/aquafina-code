// INITIALIZE COMPONENTS
void initializeComponents() {
  lcd.begin(16, 2);
  rtc.begin();
  servoFeeder.attach(pinFeeder);
  spinSpeed = setSpinSpeed(setSpeed);
  initializePumps();
  initializeHeaters();
  initializeWaterLevelSensor();
}

// INITIALIZE WATER PUMPS
void initializePumps() {
  pinMode(pinRelayDrainPump, OUTPUT);
  pinMode(pinRelayFillPump, OUTPUT);
  digitalWrite(pinRelayDrainPump, HIGH);
  digitalWrite(pinRelayFillPump, HIGH);
}

// INITIALIZE WATER HEATERS
void initializeHeaters() {
  pinMode(pinRelayHeaterOne, OUTPUT);
  pinMode(pinRelayHeaterTwo, OUTPUT);
  digitalWrite(pinRelayHeaterOne, LOW);
  digitalWrite(pinRelayHeaterTwo, LOW);
}

// INITIALIZE WATER LEVEL SENSORS
void initializeWaterLevelSensor() {
  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);
}

// LCD BRIGHTNESS SETTING
void setBright(int value) {
  analogWrite(10, value);
}

// RUN FEEDING SERVO
void runFeedingServo() {
  if (currentHeaterState == on) {
    turnOffHeaters();
  }

  servoFeeder.write(1350);

  if (millis() - previousMillis >= 1000) {
    previousMillis = millis();
  }
}

// STOP FEEDING SERVO
void stopFeedingServo() {
  if (currentHeaterState == on && oldState == 0) {
    turnOnHeaters();
  }

  servoFeeder.write(90);

  if (millis() - previousMillis >= 1000) {
    previousMillis = millis();
  }
}

// RUN DRAIN PUMP
void drainPumpOn() {
  if (currentHeaterState == on) {
    turnOffHeaters();
  }

  digitalWrite(pinRelayDrainPump, LOW);

  if (millis() - previousMillis >= 1000) {
    previousMillis = millis();
  }
}

// STOP DRAIN PUMP
void drainPumpOff() {
  if (currentHeaterState == on && oldState == 0) {
    turnOnHeaters();
  }
  
  digitalWrite(pinRelayDrainPump, HIGH);
}

// RUN FILL PUMP
void fillPumpOn() {
  if (currentHeaterState == on) {
    turnOffHeaters();
  }

  digitalWrite(pinRelayFillPump, LOW);
  
  if (millis() - previousMillis >= 1000) {
    previousMillis = millis();
  }
}

// STOP FILL PUMP
void fillPumpOff() {
  digitalWrite(pinRelayFillPump, HIGH);
}

// TURN ON HEATERS
void turnOnHeaters() {
  oldState = 1;
  digitalWrite(pinRelayHeaterOne, LOW);
  digitalWrite(pinRelayHeaterTwo, LOW);
}

// TURN OFF HEATERS
void turnOffHeaters() {
  oldState = 0;
  digitalWrite(pinRelayHeaterOne, HIGH);
  digitalWrite(pinRelayHeaterTwo, HIGH);
}

// TOGGLES ULTRASONIC SENSOR
void pulseUltrasonicSensor() {
  float alpha = 0.4;

  digitalWrite(pinTrig, LOW); 
  delayMicroseconds(2);
  digitalWrite(pinTrig, HIGH); 
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW); 

  float duration = pulseIn(pinEcho, HIGH, 26000);
  int rawDistance = duration / 58;
  waterDistance = alpha * rawDistance + (1 - alpha) * waterDistance;
}

// SYNC SET TIME TO RTC
void saveSetTime(int hours, int minutes, int seconds) {
  char printSavedTime[60];
  rtc.setTime(hours, minutes, seconds);

  sprintf(printSavedTime, "Saved %02d:%02d:%02d As New Time", hours, minutes, seconds);
  Serial.println(printSavedTime);
}

// SYNC SET DAY TO RTC
void saveSetDay(int dayNumber, char day[]) {
  char printSavedDay[phraseBuffer];
  rtc.setDOW(dayNumber);

  sprintf(printSavedDay, "Saved %s As Day", day);
  Serial.println(printSavedDay);
}

// FOR TEMPERATURE CALCULATION
void calculateTemp() {
  OneWire oneWire(pinThermistor);
  DallasTemperature sensors(&oneWire);

  sensors.requestTemperatures();
  celsius = sensors.getTempCByIndex(0) + 1.5;
  fahrenheit = celsius * 1.8 + 32;
  Serial.println(sensors.getTempCByIndex(0));
}

// WATER LEVEL CALCULATION
void calculateWaterLevel() {
  if (waterDistance <= aquariumHeight) {
    waterLevel = aquariumHeight - waterDistance;
    waterPercentage = ((float) waterLevel / aquariumHeight) * 100;
  }

  else {
    waterPercentage = 0;
  }
}

// SAVING FEEDING SCHEDULE
void saveTimeAsChar(int feedFreqNumber, int hours,  int minutes) {
  char saveTime[timeBuffer];
  char displaySaved[phraseBuffer];

  sprintf(saveTime, "%02d:%02d", hours, minutes);
  switch (feedFreqNumber) {
    case 1:
      memcpy(savedFeedingSchedule01, saveTime, 30 * sizeof(char));
      sprintf(displaySaved, "Saved %s as 1st Frequency", saveTime);
      Serial.println(displaySaved);
    break;

    case 2:
      memcpy(savedFeedingSchedule02, saveTime, 30 * sizeof(char));
      sprintf(displaySaved, "Saved %s as 2nd Frequency", saveTime);
      Serial.println(displaySaved);
    break;

    case 3:
      memcpy(savedFeedingSchedule03, saveTime, 30 * sizeof(char));
      sprintf(displaySaved, "Saved %s as 3rd Frequency", saveTime);
      Serial.println(displaySaved);
    break;

    case 4:
      memcpy(savedFeedingSchedule04, saveTime, 30 * sizeof(char));
      sprintf(displaySaved, "Saved %s as 4th Frequency", saveTime);
      Serial.println(displaySaved);
    break;

    case 5:
      memcpy(savedFeedingSchedule05, saveTime, 30 * sizeof(char));
      sprintf(displaySaved, "Saved %s as 5th Frequency", saveTime);
      Serial.println(displaySaved);
    break;
  }
}

// SAVE DAY TO MEMORY
void saveDayToMemory(int changeFreqNumber, char day[]) {
  char displaySaved[phraseBuffer];

  switch (changeFreqNumber) {
    case 1:
      memcpy(savedChangingSchedule01, day, 60 * sizeof(char));
      sprintf(displaySaved, "Saved %s as 1st Scheduled Day", day);
      Serial.println(displaySaved);
    break;

    case 2:
      memcpy(savedChangingSchedule02, day, 60 * sizeof(char));
      sprintf(displaySaved, "Saved %s as 2nd Scheduled Day", day);
      Serial.println(displaySaved);
    break;

    case 3: 
      memcpy(savedChangingSchedule03, day, 60 * sizeof(char));
      sprintf(displaySaved, "Saved %s as 3rd Scheduled Day", day);
      Serial.println(displaySaved);
    break;

    case 4:
      memcpy(savedChangingSchedule04, day, 60 * sizeof(char));
      sprintf(displaySaved, "Saved %s as 4th Scheduled Day", day);
      Serial.println(displaySaved);
    break;

    case 5:
      memcpy(savedChangingSchedule05, day, 60 * sizeof(char));
      sprintf(displaySaved, "Saved %s as 5th Scheduled Day", day);
      Serial.println(displaySaved);
    break;
  }
}

// SAVE TIME TO MEMORY
void concatenateTimeToMemory(int changeFreqNumber, int hours,  int minutes) {
  char changingTime[timeBuffer];
  char displaySaved[phraseBuffer];

  sprintf(changingTime, "%02d:%02d", hours, minutes);

  switch (changeFreqNumber) {
    case 1:
      strcat(savedChangingSchedule01, changingTime);
      sprintf(displaySaved, "Saved %s as 1st Scheduled Time", changingTime);
      Serial.println(displaySaved);
    break;

    case 2:
      strcat(savedChangingSchedule02, changingTime);
      sprintf(displaySaved, "Saved %s as 2nd Scheduled Time", changingTime);
      Serial.println(displaySaved);
    break;

    case 3: 
      strcat(savedChangingSchedule03, changingTime);
      sprintf(displaySaved, "Saved %s as 3rd Scheduled Time", changingTime);
      Serial.println(displaySaved);
    break;

    case 4:
      strcat(savedChangingSchedule04, changingTime);
      sprintf(displaySaved, "Saved %s as 4th Scheduled Time", changingTime);
      Serial.println(displaySaved);
    break;

    case 5:
      strcat(savedChangingSchedule05, changingTime);
      sprintf(displaySaved, "Saved %s as 5th Scheduled Time", changingTime);
      Serial.println(displaySaved);
    break;
  }
}

// FEEDING SCHEDULE PROCESS
void feedOnce() {
  currentServoState = turn;
  previousSpinMillis = millis();

  while (true) {
    if (currentServoState == turn) {
      runFeedingServo();
      printLoadingProcess("Feeding", 3, 0);

      if (millis() - previousSpinMillis >= spinTime) {
        previousSpinMillis = millis();
        currentServoState = feedStop;

      // For Resetting
        triggerFeedSchedule = false;
        previousFeedMillis = millis();
      }
    }

    if (currentServoState == feedStop) {
      stopFeedingServo();
      printIcon(15, 1, 0);
      printText("Feeding Done", 3, 0);
      delay(1700);

      lcd.clear();
      currentScreen = title;
      return;
    }
  }
}

// WATER-CHANGING SCHEDULE PROCESS
void changeOnce() {
  char displayWaterLevel[normalBuffer];
  unsigned long previousUpdate = millis();
  currentChangingState = drain;

  lcd.clear();
  while (true) {
    if (millis() - previousUpdate >= 1700) {
      checkWaterDistance();
      sprintf(displayWaterLevel, "%d%%  ", waterPercentage);
      printIcon(13, 5, 1); printText(displayWaterLevel, 7, 1);
      previousUpdate = millis();
    }

    if (currentChangingState == drain) {
      drainPumpOn();
      printLoadingProcess("Draining", 3, 0);

      if (waterPercentage <= maxDrainLevel) {
        drainPumpOff();
        clearLine(0);
        printIcon(15, 0, 0);
        printText("Draining Done", 2, 0);

        delay(1700);

        clearLine(0);
        currentChangingState = fill;
      }
    }

    if (currentChangingState == fill) {
      fillPumpOn();
      printLoadingProcess("Filling", 3, 0);

      if (waterPercentage >= maxFillLevel ) {
        clearLine(0);
        fillPumpOff();
        printIcon(15, 1, 0);
        printText("Filling Done", 3, 0);

        delay(1700);
        
       // For Resetting
        triggerWaterSchedule = false;
        previousChangeMillis = millis();
        currentRegulationMode = regulate;
        currentChangingState = changeStop;

        lcd.clear();
        currentScreen = title;
        return;
      }
    }
  }
}

// WATER-REGULATION PROCESS
void regulateMode() {
  int discrepancyNumber = 2;

  if (waterPercentage >= (maxFillLevel + discrepancyNumber)) {
    drainPumpOn();
  }

  else if (waterPercentage <= (maxFillLevel - discrepancyNumber)) {
    fillPumpOn();
  }

  else {
    drainPumpOff();
    fillPumpOff();
  }
}
