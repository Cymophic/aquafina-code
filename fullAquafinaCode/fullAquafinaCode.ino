// PIN NUMBERS
#define pinFeeder 22          /* Feeding Servo Motor: PIN 22 */
#define pinThermistor 23      /* Waterproof Temperature Sensor: PIN 23 */
#define pinSDA 26             /* RTC SDA: PIN 26 */
#define pinSCL 27             /* RTC SCL: PIN 27 */
#define pinEcho 30            /* Ultrasonic Echo: PIN 30 */
#define pinTrig 31            /* Ultrasonic Trigger: PIN 31 */
#define pinRelayDrainPump 42  /* Draining Pump: PIN 42 */
#define pinRelayFillPump 43   /* Filling Pump: PIN 43 */
#define pinRelayHeaterOne 46  /* Heater I: PIN 46 */
#define pinRelayHeaterTwo 47  /* Heater II: PIN 47 */

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

// CONTROLS
bool upButton, downButton, backButton, selectButton, idle;

// MENU NAVIGATION
uint8_t mainNav = 1;
uint8_t subNav = 1; 
uint8_t setTimeNav = 1;
uint8_t feedFreqNumber = 1;
uint8_t changeFreqNumber = 1;

// SCREENS
enum screenType {
 // Menu Screens
  title, menu, feed, water, temp, brightness, time, 
 // Feeding Settings
  feedNow, setFeedFrequency, editFeedFrequency, setFeedSchedule, scheduledFeeding, servoSpeedSetting,
 // Water-Change Settings
  checkWaterLevel, changeNow, setChangeFrequency, editChangeFrequency, setChangeSchedule, setChangingTime, setWaterLevel, manualDrain, manualFill, scheduledChanging,
 // Time Settings 
  displayTime, adjustTime, setTime, setDay,
 // Temperature Settings
  checkTemp, toggleControl, toggleHeater, setTemp};
// Default Screen
enum screenType currentScreen = title;

// BUFFER SIZES
const int timeBuffer = 30;
const int dayBuffer = 30;
const int dayAndTimeBuffer = 60;
const int normalBuffer = 30;
const int phraseBuffer = 60;

// RTC-RELATED VARIABLES
char dayAndTimeNow[dayAndTimeBuffer];
char* timeNow;
char* timeDisplay;
char* dayNow;
char* dayDisplay;

// FEEDING VARIABLES
enum servoStates {turn, feedStop};
enum servoStates currentServoState = turn;
unsigned long previousSpinMillis, previousFeedMillis;
uint8_t numberOfFeedFrequencies, feedingFrequency;
uint16_t spinSpeed;
bool triggerFeedSchedule;
  // SCHEDULE MEMORY
  char savedFeedingSchedule01[timeBuffer];
  char savedFeedingSchedule02[timeBuffer];
  char savedFeedingSchedule03[timeBuffer];
  char savedFeedingSchedule04[timeBuffer];
  char savedFeedingSchedule05[timeBuffer]; 

// WATER-CHANGING VARIABLES
enum regulationMode {change, regulate};
enum regulationMode currentRegulationMode = regulate;
enum changingState {drain, fill, changeStop};
enum changingState currentChangingState = drain; 
unsigned long previousChangeMillis, previousWaterMillis;
uint8_t numberOfChangeFrequencies, changingFrequency, waterDistance, waterLevel;
int waterPercentage;
bool triggerWaterSchedule;
char* tempHolder;
  // SCHEDULE MEMORY
  char savedChangingSchedule01[dayAndTimeBuffer];
  char savedChangingSchedule02[dayAndTimeBuffer];
  char savedChangingSchedule03[dayAndTimeBuffer];
  char savedChangingSchedule04[dayAndTimeBuffer];
  char savedChangingSchedule05[dayAndTimeBuffer];

// TEMPERATURE VARIABLES
enum controlState {automatic, manual};
enum controlState currentControlState = automatic;
enum heaterStates {on, off};
enum heaterStates currentHeaterState = on;
unsigned long previousTempMillis;
float fahrenheit, celsius;
bool oldControl = 1;
bool oldState = 1;

// SCREEN BRIGHTNESS
int setBrightness;

// OTHER PARAMETERS
unsigned long previousHourGlassMillis, previousBlinkMillis, previousUpdateMillis, previousMillis, previousMillis02;
uint8_t hourGlassState, blinkState;
bool oneTimeUpdate = true;
bool shouldBlink;

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

/*/
Copyright 2023, Aquafina Group, All right reserved.
/*/

