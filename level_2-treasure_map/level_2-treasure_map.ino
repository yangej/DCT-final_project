#include <HashMap.h>
#include <Servo.h>

Servo servo_1;
Servo servo_2;

int T1_BUTTON_PIN = 2;
int T2_BUTTON_PIN = 3;
int T3_BUTTON_PIN = 4;
int D1_BUTTON_PIN = 5;
int D2_BUTTON_PIN = 6;
int D3_BUTTON_PIN = 7;
int BUZZER_PIN = 8;
int MOTOR1_PIN = 10;
int MOTOR2_PIN = 11;
int G_LED_PIN = 12;
int R_LED_PIN = 13;

bool t1ButtonState = LOW;
bool t2ButtonState = LOW;
bool t3ButtonState = LOW;
bool d1ButtonState = LOW;
bool d2ButtonState = LOW;
bool d3ButtonState = LOW;

int selection = -1;
char* signText = (char*)"";

const byte HASH_SIZE = 6;
HashType<int,int> hashRawArray[HASH_SIZE];
HashMap<int,int> answers = HashMap<int,int>( hashRawArray , HASH_SIZE );

void setup() {
  Serial.begin(9600);

  pinMode(BUZZER_PIN,OUTPUT);
  pinMode(T1_BUTTON_PIN, INPUT);
  pinMode(T2_BUTTON_PIN, INPUT);
  pinMode(T3_BUTTON_PIN, INPUT);
  pinMode(D1_BUTTON_PIN, INPUT);
  pinMode(D2_BUTTON_PIN, INPUT);
  pinMode(D3_BUTTON_PIN, INPUT);
  pinMode(R_LED_PIN, OUTPUT);
  pinMode(G_LED_PIN, OUTPUT);
  servo_1.attach(MOTOR1_PIN);
  servo_2.attach(MOTOR2_PIN);

  answers[0](T1_BUTTON_PIN, D1_BUTTON_PIN);
  answers[1](T2_BUTTON_PIN, D2_BUTTON_PIN);
  answers[2](T3_BUTTON_PIN, D3_BUTTON_PIN);
  answers[3](D1_BUTTON_PIN, T1_BUTTON_PIN);
  answers[4](D2_BUTTON_PIN, T2_BUTTON_PIN);
  answers[5](D3_BUTTON_PIN, T3_BUTTON_PIN);
}

void loop() {
  t1ButtonState = digitalRead(T1_BUTTON_PIN);
  t2ButtonState = digitalRead(T2_BUTTON_PIN);
  t3ButtonState = digitalRead(T3_BUTTON_PIN);
  d1ButtonState = digitalRead(D1_BUTTON_PIN);
  d2ButtonState = digitalRead(D2_BUTTON_PIN);
  d3ButtonState = digitalRead(D3_BUTTON_PIN);

  if (t1ButtonState == HIGH) { selectAndCheck(T1_BUTTON_PIN); }
  else if (t2ButtonState == HIGH) { selectAndCheck(T2_BUTTON_PIN); }
  else if (t3ButtonState == HIGH) { selectAndCheck(T3_BUTTON_PIN); }
  else if (d1ButtonState == HIGH) { selectAndCheck(D1_BUTTON_PIN); }
  else if (d2ButtonState == HIGH) { selectAndCheck(D2_BUTTON_PIN); }
  else if (d3ButtonState == HIGH) { selectAndCheck(D3_BUTTON_PIN); }

  delay(1000);
}

void selectAndCheck(int selectedPin) {
  Serial.print("selection: ");
  Serial.println(selection);
  Serial.print("selectedPin: ");
  Serial.println(selectedPin);
  if (selection != -1) {
    if (answers.getValueOf(selection) == selectedPin) {
      signText = (char*)"success!";
      answerRight();
      getCoin();
    } else {
      signText = (char*)"fail!";
      answerWrong();
    }
    Serial.println(signText);
    reset();
  } else {
    ledDetected();
    selection = selectedPin;
  }

  Serial.println(selectedPin);
}

void reset() {
  signText = (char*)"";
  selection = -1;
}

void getCoin() {
  servo_1.write(90);
  servo_2.write(90);
  delay(2000);
  servo_1.write(0);
  servo_2.write(0);
  delay(2000);
  servo_1.write(90);
  servo_2.write(90);
}

void buzzFail() {
  int melody[] = {100, 100};
  int noteDurations[] = { 4, 6 };

  delay(500);
   for (int thisNote = 0; thisNote < 2; thisNote++) {
      int noteDuration = 1000/noteDurations[thisNote];
      tone(8, melody[thisNote],noteDuration);
      delay(noteDuration +30);
   }
}

void buzzSuccess() {
  int melody[] = {2500, 1000, 2500, 1000};  
  int noteDurations[] = { 12, 10, 12, 10 };

   for (int thisNote = 0; thisNote < 4; thisNote++) {
      int noteDuration = 1000/noteDurations[thisNote];
      tone(8, melody[thisNote], noteDuration);
      delay(noteDuration +30);
   }
}

void ledBlink(int pin) {
  for (int i = 0; i < 6; i++) {
    digitalWrite(pin, HIGH);
    delay(100);
    digitalWrite(pin, LOW);
    delay(100); 
  } 
}

void answerRight() {
  digitalWrite(R_LED_PIN, LOW);
  buzzSuccess();
  ledBlink(G_LED_PIN);
}

void answerWrong() {
  digitalWrite(G_LED_PIN, LOW);
  buzzFail();
  ledBlink(R_LED_PIN);
}

void ledDetected() {
  digitalWrite(G_LED_PIN, HIGH);
  digitalWrite(R_LED_PIN, HIGH);
}
