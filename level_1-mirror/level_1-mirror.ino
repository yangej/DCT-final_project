#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int colorR = 0;
const int colorG = 0;
const int colorB = 255;

// assign time
long previousMillis = 0;
int timer;
int recordedTime;

// sound
int SOUND_VOLUME_PIN = A7;
int SOUND_THRESHOLD = 500;
int soundValue;

// light
int LIGHT_PIN = 3;

// switch
int SWITCH_PIN = 4;
int switchState = 0;

// button
int BUTTON_PIN = 6;
bool buttonState = true;

// grouping
int groupCount = 4;
int memberCount = 4;
int numberPerGroup[4] = {0, 0, 0, 0};
const char *tools[4] = { "Lollipop", "Stick", "Straw", "Chopsticks" };
int assignedGroup = 0;
int isGrouped = false;

boolean isAllEmpty = false;

void setup() {
  Serial.begin(9600);

  // lcd settings
  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);
  resetLCD();
  
  pinMode(SOUND_VOLUME_PIN, INPUT);
  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT);

  getNumberPerGroup();
}

void loop() {
  buttonState = digitalRead(BUTTON_PIN);
  switchState = digitalRead(SWITCH_PIN);

  // print current situation of each group
  for (int i = 0; i < groupCount; i++) {
    Serial.println(numberPerGroup[i]);
  }

  // button setting
  if (buttonState == HIGH) {
    resetAllVar();
    resetLCD();
    getNumberPerGroup();
  } else {
    // switch and timer
    if (switchState == true) {
      Serial.println("switchState is true");

      //initialize values
      if (isGrouped == true) {
        reset();
        resetLCD();
        isGrouped = false;
      }
      
      // timer setting
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis > 1000) {
        previousMillis = currentMillis;   
        timer += 1;
      }
      timerTxt();
  
      // sound sensor setting
      soundValue = analogRead(SOUND_VOLUME_PIN);
      int transformedSoundValue = map(soundValue, 0, 1024, 0, 255);
      analogWrite(LIGHT_PIN, transformedSoundValue);
      soundValueTxt();
    } else {
      if (isGrouped == false) {
        minusCount();
        isGrouped = true;
        
        assignedGroupTxt();
        assignedToolTxt();
        
        isAllEmpty = allEmpty(numberPerGroup);
        if (isAllEmpty == true) {
          groupDoneTxt();
        }
      }
    }
  }

  delay(1000);
}

int getRandom(int max) {
  return floor(random(max));
}

int getArraySize(int* arr) {
  return sizeof(arr) / sizeof(arr[0]);
}

boolean allEmpty(int arr[]) {
  int arrayLength = getArraySize(arr);
  
  for (int i = 0; i < arrayLength; i++) {
    if (arr[i] != 0) {
      return false;
    }
  }

  return true;
}

void getNumberPerGroup() {
  int mod = memberCount % groupCount;
  int remain = floor(memberCount / groupCount);

  for (int i = 0; i < groupCount; i++) {
    numberPerGroup[i] = remain;
  }

  if (mod != 0) {
    for (int i = 0; i < mod; i++) {
      numberPerGroup[i] += 1;
    }
  }
}

void minusCount() {
  int randomNum = getRandom(groupCount);

  if (numberPerGroup[randomNum] > 0) {
    numberPerGroup[randomNum] -= 1;
    assignedGroup = randomNum;
  } else {
    if (allEmpty(numberPerGroup) == false) {
      minusCount();
    }
  }
}

void timerTxt() {
  lcd.setCursor(0, 1);
  lcd.print(timer);
  lcd.print("               ");
}

void soundValueTxt() {
  lcd.setCursor(11, 1);
  lcd.print(soundValue);
  lcd.print("               ");
}

void groupDoneTxt() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Grouping is done!");
  lcd.setCursor(0, 1);
  lcd.print("Click reset!");
  delay(2000);
}

void assignedGroupTxt() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Group: ");
  lcd.print(assignedGroup);
}

void assignedToolTxt() {
  lcd.setCursor(0, 1);
  lcd.print("Tool: ");
  lcd.print(tools[assignedGroup]);
  delay(2000);
}

void resetLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time");  
  lcd.setCursor(11, 0);
  lcd.print("Sound");
  delay(1000);
}

void reset() {
  timer = 0;
  recordedTime = 0;
  soundValue = 0;
}

void resetAllVar() {
  switchState = false;
  isAllEmpty = false;
  recordedTime = 0;
  soundValue = 0;
  
  assignedGroup = -1;
  for(int i = 0; i < groupCount; i++) {
    numberPerGroup[i] = 0;
  }
}
