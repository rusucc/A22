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
    uint32_t _millisMeasure;
    uint32_t _millisReceive;
    uint8_t _powerReductionLevel; //1, baterie, 5 sursa commutatie, scade sensibilitatea
  public:
    DYPA22(uint8_t address8 = 0xE8, uint8_t range = 0x02, uint8_t coneAngle = 4);
    void writeNewAddress(uint8_t newAddress = 0xE8);
    void setConeAngle(uint8_t coneAngle);
    void setRange(uint8_t range);
    void setAddress(uint8_t address = 0xE8);
    void setMillisMeasure(uint32_t timeOfRequest);
    void setMillisReceive(uint32_t timeOfRequest);
    void sendMeasureRequest();
    void sendReceiveRequest();
    void sendPowerNoiseReductionLevel(uint8_t level);
    uint8_t getAddress();
    uint8_t getRange();
    uint8_t getConeAngle();
    uint16_t getDistance();
    uint32_t getMillisMeasure();
    uint32_t getMillisReceive();

};
#endif