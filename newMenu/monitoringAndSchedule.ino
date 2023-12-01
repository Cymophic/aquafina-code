// CHECK FEEDING SCHEDULE
void checkFeedSchedule() {
  int firstFeedScheduleCheck = strcmp(timeNow, savedFeedingSchedule01);
  int secondFeedScheduleCheck = strcmp(timeNow, savedFeedingSchedule02);
  int thirdFeedScheduleCheck = strcmp(timeNow, savedFeedingSchedule03);
  int fourthFeedScheduleCheck = strcmp(timeNow, savedFeedingSchedule04);
  int fifthFeedScheduleCheck = strcmp(timeNow, savedFeedingSchedule05);

  timeNow = rtc.getTimeStr(FORMAT_SHORT);

  if (firstFeedScheduleCheck == 0 && triggerFeedSchedule == true) {
    Serial.println(F("Feeding..."));
    lcd.clear();
    feedOnce();
  }

  if (secondFeedScheduleCheck == 0 && triggerFeedSchedule == true) {
    Serial.println(F("Feeding..."));
    lcd.clear();
    feedOnce();
  }

  if (thirdFeedScheduleCheck == 0 && triggerFeedSchedule == true) {
    Serial.println(F("Feeding..."));
    lcd.clear();
    feedOnce();
  }

  if (fourthFeedScheduleCheck == 0 && triggerFeedSchedule == true) {
    Serial.println(F("Feeding..."));
    lcd.clear();
    feedOnce();
  }

  if (fifthFeedScheduleCheck == 0 && triggerFeedSchedule == true) {
    Serial.println(F("Feeding..."));
    lcd.clear();
    feedOnce();
  }

 // Reset Trigger
  if (millis() - previousFeedMillis >= 60000) {
    triggerFeedSchedule = true;
  }
}

// WATER-CHANGING SCHEDULE PROCESS
void checkChangeSchedule() {
  checkDayAndTime();
  int firstWaterScheduleCheck = strcmp(dayAndTimeNow, savedChangingSchedule01);
  int secondWaterScheduleCheck = strcmp(dayAndTimeNow, savedChangingSchedule02);
  int thirdWaterScheduleCheck = strcmp(dayAndTimeNow, savedChangingSchedule03);
  int fourthWaterScheduleCheck = strcmp(dayAndTimeNow, savedChangingSchedule04);
  int fifthWaterScheduleCheck = strcmp(dayAndTimeNow, savedChangingSchedule05);

  // Reset Trigger
  if (millis() - previousChangeMillis >= 60000) {
    triggerWaterSchedule = true;
  }

  // Schedule Checks
  if (firstWaterScheduleCheck == 0 && triggerWaterSchedule == true) {
    Serial.println(F("Changing Water..."));
    currentRegulationMode = change;
  }

  else if (secondWaterScheduleCheck == 0 && triggerWaterSchedule == true) {
    Serial.println(F("Changing Water..."));
    currentRegulationMode = change;
  }

  else if (thirdWaterScheduleCheck == 0 && triggerWaterSchedule == true) {
    Serial.println(F("Changing Water..."));
    currentRegulationMode = change;
  }

  else if (fourthWaterScheduleCheck == 0 && triggerWaterSchedule == true) {
    Serial.println(F("Changing Water..."));
    currentRegulationMode = change;
  }

  else if (fifthWaterScheduleCheck == 0 && triggerWaterSchedule == true) {
    Serial.println(F("Changing Water..."));
    currentRegulationMode = change;
  }
}

// CHECK RTC AND UPLOAD TO MEMORY
void checkDayAndTime() {
  dayNow = rtc.getDOWStr();
  timeNow = rtc.getTimeStr(FORMAT_SHORT);

  memcpy(dayAndTimeNow, dayNow, 60 * sizeof(char));
  strcat(dayAndTimeNow, timeNow);
}

// CHECK WATER LEVEL
void checkWaterDistance() {
  pulseUltrasonicSensor();
  calculateWaterLevel();
}

// CHECK AQUARIUM TEMPERATURE
void checkTemps() {
  if (millis() - previousTempMillis >= 1000) {
    calculateTemp();
    previousTempMillis = millis();
  }

  if (currentControlState == automatic) {
    if (isCold()) {
      currentHeaterState = on;
      turnOnHeaters();
    }

    else {
      currentHeaterState = off;
      turnOffHeaters();
    }
  }
}

// CHECK IF AQUARIUM IS HOT OR COLD
bool isCold() {
  if (celsius < optimalTemp) {
    return true;
  }

  if (celsius >= optimalTemp) {
    return false;
  }
}

// CHECK WHETHER TIME IS MORNING OR NIGHT
bool isMorning() {
  if (strcmp(timeNow, morningTime) > 0 && strcmp(timeNow, eveningTime) < 0) 
    return true;
  else 
    return false;
}

// CHECK KEY PRESSES
void getKeyInput() {
  int key = analogRead(A0);

  //No Input
  if (key > 1000) {
    upButton = false;
    downButton = false;
    backButton = false;
    selectButton = false;
    idle = true;
  }

  //Right Button
  if (key < 50) {
    selectButton = true;
    idle = false;
    delay(200);
  }

  //Up Button
  else if (key < 200) {
    upButton = true;
    idle = false;
    delay(200);
  }

  //Down Button
  else if (key < 400) {
    downButton = true;
    idle = false;
    delay(200);
  }

  //Left Button
  else if (key < 600) {
    backButton = true;
    idle = false;
    delay(200);
  }

  //Select Button
  else if (key < 800) {
    selectButton = true;
    idle = false;
    delay(200);
  }
}

