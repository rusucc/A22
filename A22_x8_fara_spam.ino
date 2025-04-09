#include "DYPA22.h"
#define MIN_TIME 50
#define DEBUG false

#define RANGE 1
#define LISTEN 0
#define SONAR_DELAY 10

DYPA22 Sensors[8] = {};
uint8_t addressArray[8] = { 0xDE, 0xDC, 0xDA, 0xD8, 0xD6, 0xD4, 0xD2, 0xE0 };
uint8_t firingSequence[8] = { 0, 4, 2, 6, 1, 5, 3, 7 };  // -1 fata de realitate, array 0-index
uint8_t distances[8] = {};

void setup() {
  Serial.begin(38400);
  Wire.begin();
  delay(100);

  for (int i = 0; i < 8; i++) {
    Sensors[i].setAddress(addressArray[i]);
    delay(1);
    Sensors[i].setRange(0x03);
    delay(1);
    Sensors[i].sendPowerNoiseReductionLevel(2);
    delay(1);
    Sensors[i].setConeAngle(3);  //1-2-3-4
    delay(1);
  }
  for (int i = 0; i < 8; i++) {
    Sensors[i].setMillisMeasure(i * 10);
  }
}
void loop() {
  static bool rangeListenToggle = RANGE;  //trece prima data prin array de senzori ca sa le trimita ranging request, apoi mai trece o data sa culeaga rezultatele
  static int currentSonarId = 1;
  static uint32_t millisSonarChange = 0;

  static uint32_t millisSerialPrint = 0;

  if (rangeListenToggle == RANGE) {
    if (millis() - millisSonarChange > SONAR_DELAY) {
      int firingId = firingSequence[currentSonarId - 1];  //array cu index de la 0
      Sensors[firingId].sendMeasureRequest();
      if (currentSonarId == 8) {
        rangeListenToggle = LISTEN;
        currentSonarId = 1;
      } else {
        currentSonarId++;
      }
      millisSonarChange = millis();
    }
  } else {
    if (millis() - millisSonarChange > SONAR_DELAY) {
      int firingId = firingSequence[currentSonarId - 1];  //array cu index de la 0
      Sensors[firingId].sendReceiveRequest();
      if (currentSonarId == 8) {
        rangeListenToggle = RANGE;
        currentSonarId = 1;
      } else {
        currentSonarId++;
      }
      millisSonarChange = millis();
    }
  }
  // for (int i = 0; i < 8; i++) {
  //   int id = firingSequence[i];
  //   Sensors[id].sendMeasureRequest();
  //   delay(10);
  // }
  // delay(1);
  // for (int i = 0; i < 8; i++) {
  //   int id = firingSequence[i];
  //   Sensors[id].sendReceiveRequest();
  //   delay(10);
  // }
  // delay(10);
  if (millis() - millisSerialPrint > 100) {
    for (int i = 0; i < 8; i++) {
      Serial.print(Sensors[i].getDistance());
      Serial.print(',');
      Serial.print(Sensors[i].getConeAngle());
      if (i != 7) Serial.print(',');
    }
    Serial.println();
    millisSerialPrint = millis();
  }
}