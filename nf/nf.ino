

#define codename "Net-Flasher"
#define codedate " 2022-0524 "

#include <EEPROM.h>
#include <Bounce.h>
typedef unsigned int u_int32;

const int vibSensePin = 23;
//Bounce vibSense = Bounce(vibSensePin, 10);
const int senseBtnPin = 22;
Bounce senseBtn = Bounce(senseBtnPin, 20);
const int flashEn = 21;
const int detectLed = 16;
const int senseLed = 17;
const int actLed = 13;
const int senseEE = 4;       // eeprom location
const int senseMax = 4;      // max vibration count value
const int flashDur = 200;    // flash duration in ms
int senseSet = 0;            // read from eeprom, vibration count setting
int senseCnt = 0;            // number of LED flashes to output (x2)
u_int32 senseAt = 0;         // last time the LED was updated
u_int32 senseBtnAt = 0;      // last time the button was pressed
volatile int vib = 0;        // set to 1 by interrupt handler
int vibCnt = 0;              // vibration count
int vibState = 0;            // digitalRead result

u_int32 flashAt = -60*1000;  // last time the flash was lit
u_int32 detectAt = -60*1000; // last time the detect LED was lit
u_int32 vibAt = 0;           // last time the detect LED was lit
u_int32 actAt = 0;           // last time the activity LED was updated

void vibIntr() { vib = 1; }

void setup(void) {
  //Serial.begin(115200); // unnecessary on teensy
  Serial.println("\nHello!");

  pinMode(flashEn, OUTPUT);
  pinMode(detectLed, OUTPUT);
  pinMode(senseLed, OUTPUT);
  pinMode(actLed, OUTPUT);
  pinMode(vibSensePin, INPUT_PULLUP);
  pinMode(senseBtnPin, INPUT_PULLUP);

  digitalWrite(flashEn, LOW);
  digitalWrite(detectLed, HIGH);
  digitalWrite(senseLed, HIGH);
  digitalWrite(actLed, HIGH);

  Serial.println(" ");
  Serial.print(codename);
  Serial.print(codedate);

  delay(300);
  digitalWrite(detectLed, LOW);
  digitalWrite(senseLed, LOW);
  digitalWrite(actLed, HIGH);

  attachInterrupt(vibSensePin, vibIntr, FALLING);

  senseSet = EEPROM.read(senseEE);
  if (senseSet < 1 || senseSet > senseMax) senseSet = 1;
  if (Serial) {
    Serial.print("sensitivity = ");
    Serial.println(senseSet);
  }
}

// ================================================================================================

void loop() {
  // slow the loop down
  delay(10);
  int now = millis();

  // read sensitivity button and adjust
  if (senseBtn.update()) {
    if (senseBtn.fallingEdge()) {
      if (now - senseBtnAt < 2000) {
        senseSet++;
        if (senseSet > senseMax) senseSet = 1;
        if (Serial) {
          Serial.print("sensitivity = ");
          Serial.println(senseSet);
        }
        EEPROM.write(senseEE, senseSet);
      } else {
        flashAt = now-60*1000;
      }
      senseCnt = senseSet*2;
      senseBtnAt = now;
    }
  }

  // flash sense button LED to indicate sensitivity
  if (senseCnt > 0 && now - senseAt > 200) {
    digitalWrite(senseLed, !(senseCnt&1));
    senseCnt--;
    senseAt = now;
  }

  // read vibration sensor
  //int v = digitalRead(vibSensePin);
  //if (v == 0 && v != vibState) {
  if (vib) {
    vibCnt++;
    vibAt = now;
    vib = 0;
    if (Serial) {
      Serial.print("vibration = ");
      Serial.println(vibCnt);
    }
    if (vibCnt > 1<<(senseSet-1)) {
      digitalWrite(detectLed, HIGH);
      detectAt = now;
      if (now - flashAt > 10*1000) {
        digitalWrite(flashEn, HIGH);
        flashAt = now;
      }
    }
  }
  //vibState = v;
  if (now - vibAt > 100) vibCnt = 0; // reset vibration count
  if (now - detectAt > 1000) digitalWrite(detectLed, LOW);
  if (now - flashAt > flashDur) digitalWrite(flashEn, LOW);
  if (now - flashAt > 60*1000) flashAt = now-60*1000; // avoid blind time when rolling over

  u_int32 actDel = now - flashAt > 10*1000 ? 2000 : 200;
  if (now - actAt > actDel) {
    digitalWrite(actLed, !digitalRead(actLed));
    actAt = now;
  }
}
