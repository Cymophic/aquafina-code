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
