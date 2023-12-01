// PIN NUMBERS
#include "pinDeclarations.h"

// LIBRARIES USED
#include <Servo.h>
Servo servoFeeder;
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
#include <DS3231.h>
DS3231 rtc(pinSDA, pinSCL);
#include <OneWire.h>
OneWire oneWire(pinThermistor);
#include <DallasTemperature.h>
DallasTemperature sensors(&oneWire);

// GLOBAL VARIABLES
#include "variableDeclarations.h"

// DEBUGGING VARIABLES
uint8_t setSpeed = 5;                        /* Feeder Rotation Speed */
uint16_t spinTime = 3500;                    /* Feeder Spin Duration */
uint8_t aquariumHeight = 50;                 /* Aquarium Height Reference for Water-Changing Process */
uint8_t maxDrainLevel = 75;                  /* Reference Water Level for Draining Process */
uint8_t maxFillLevel = 96;                   /* Reference Water Level for Filling Process */
uint8_t optimalTemp = 28;                    /* Default Optimal Water Temperatures */
const char* morningTime = "04:00";           /* Default Morning Time for Reference */
const char* eveningTime = "19:00";           /* Default Evening TIme for Reference */
uint8_t defaultBrightness = 120;             /* Default Brightness Settings */
const uint16_t defaultLoadingDuration = 700; /* Default Loading-Animation Duration */
const uint16_t defaultDuration = 700;        /* Default Duration of Processes */

void setup() {
  Serial.begin(9600);
  Serial.println(F("Booting..."));
  initializeComponents();
  setBright(defaultBrightness);
}

void loop() {
  switch (currentScreen) {
    case title:
      titleScreen();
      break;

    case scheduledFeeding:
      feedOnce();
      break;

    case scheduledChanging:
      changeOnce();
      break;

    case menu:
      mainMenu();
      break;

    case feed:
      feedScreen();
      break;

    case feedNow:
      feedNowScreen();
      break;

    case setFeedFrequency:
      setFeedFrequencyScreen();
      break;

    case editFeedFrequency:
      editFeedFrequencyScreen();
      break;

    case setFeedSchedule:
      setFeedScheduleScreen();
      break;

    case servoSpeedSetting:
      setServoSpeedScreen();
      break;

    case water:
      waterScreen();
      break;

    case checkWaterLevel:
      checkWaterLevelScreen();
      break;

    case changeNow:
      changeNowScreen();
      break;

    case setWaterLevel:
      setWaterLevelScreen();
      break;

    case setChangeFrequency:
      setChangeFrequencyScreen();
      break;

    case editChangeFrequency:
      editChangeFrequencyScreen();
      break;

    case setChangeSchedule:
      setChangeScheduleScreen();
      break;

    case setChangingTime:
      setChangingTimeScreen();
      break;

    case manualDrain:
      manualDrainScreen();
      break;

    case manualFill:
      manualFillScreen();
      break;

    case temp:
      tempScreen();
      break;

    case checkTemp:
      tempCheckScreen();
      break;

    case toggleControl:
      toggleControlScreen();
      break;

    case toggleHeater:
      toggleHeaterScreen();
      break;

    case setTemp:
      setTempScreen();
      break;

    case brightness:
      setBrightnessScreen();
      break;

    case time:
      timeScreen();
      break;

    case displayTime:
      displayTimeScreen();
      break;

    case adjustTime:
      adjustTimeSettingsScreen();
      break;

    case setTime:
      setTimeScreen();
      break;

    case setDay:
      setDayScreen();
      break;
  }
}
