#include "DYPA22.h"
uint8_t address = 0xDA;
uint8_t coneAngle = 0x01;
uint8_t range = 0x02;

DYPA22 S1(address, range);

void setup() {
  Wire.begin();
  Serial.begin(38400);
  delay(100);
}
void loop() {
  for (int angle = 0x01; angle <= 0x04; angle++) {
    S1.setConeAngle(angle);
    delay(100);
    Serial.print("Changed angle to: "),Serial.println(angle);
    for (int i = 0; i < 20; i++) {
      S1.sendMeasureRequest();
      delay(100);
      S1.sendReceiveRequest();
      Serial.println(S1.getDistance());
      delay(100);
    }
  }
  Serial.println();
  delay(1000);
}