#ifndef DYPA22_H
#define DYPA22_H
#include <Arduino.h>
#include <Wire.h>
class DYPA22{
  private:
    uint8_t _address;
    uint8_t _coneAngle;
    uint16_t _distance;
    uint8_t _range;
  public:
    DYPA22(uint8_t address8 = 0xE8, uint8_t range = 0x02);
    void writeNewAddress(uint8_t newAddress = 0xE8);
    void setConeAngle(uint8_t coneAngle);
    void setRange(uint8_t range);
    void setAddress(uint8_t address = 0xE8);
    void sendMeasureRequest();
    void sendReceiveRequest();
    uint8_t getAddress();
    uint8_t getRange();
    uint8_t getConeAngle();
    uint16_t getDistance();
};
#endif