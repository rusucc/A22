#include "DYPA22.h"
void DYPA22::setAddress(uint8_t address){
  _address = (address)>>1;
}
DYPA22::DYPA22(uint8_t address8 = 0xE8, uint8_t range = 0x02, uint8_t coneAngle = 4){
  setRange(range);
  setAddress(address8);
  _coneAngle = coneAngle; //nu folosesc setterul pentru ca face apel la I2C, la constructor nu este initializat inca si da halt programul
}

void DYPA22::setConeAngle(uint8_t coneAngle){
  _coneAngle = coneAngle;
  Wire.beginTransmission(_address);
  Wire.write(0x07);
  Wire.write(_coneAngle);
  Wire.endTransmission();
}
void DYPA22::setRange(uint8_t range){
  _range = range;
}

void DYPA22::setMillisMeasure(uint32_t timeOfRequest){
  _millisMeasure = timeOfRequest;
}
void DYPA22::setMillisReceive(uint32_t timeOfRequest){
 _millisReceive = timeOfRequest;
}
void DYPA22::sendPowerNoiseReductionLevel(uint8_t level){
  _powerReductionLevel = level;
  Wire.beginTransmission(_address);
  Wire.write(0x06);
  Wire.write(level);
  Wire.endTransmission();
}
void DYPA22::sendMeasureRequest(){
  Wire.beginTransmission(_address);
  Wire.write(0x10);
  switch (_range) {
    case 0x01:
      Wire.write(0xBD);
      break;
    case 0x02:
      Wire.write(0xBC);
      break;
    case 0x03:
      Wire.write(0xB8);
      break;
    case 0x04:
      Wire.write(0xB4);
      break;
    default:
      break;
  }
  Wire.write(0xB0);
  Wire.endTransmission();
}
void DYPA22::sendReceiveRequest(){
  Wire.beginTransmission(_address);
  Wire.write(0x02);
  Wire.endTransmission();
  Wire.requestFrom(_address,2);
  byte received[2]={};
  if(Wire.available()){
    received[0] = Wire.read();
    received[1] = Wire.read();
  }
  _distance = (received[0] << 8) | (received[1]);
}
void DYPA22::writeNewAddress(uint8_t newAddress = 0xE8){
  if((newAddress>>7)&1){ //verifica sa fie adresa buna, pe 8 biti
    Wire.beginTransmission(_address);
    Wire.write(0x05);
    Wire.write(newAddress);
    Wire.endTransmission();
    setAddress(newAddress);
  }
}
uint16_t DYPA22::getDistance(){
  return _distance;
}
uint8_t DYPA22::getConeAngle(){
  return _coneAngle;
}
uint32_t DYPA22::getMillisMeasure(){
  return _millisMeasure;
}
uint32_t DYPA22::getMillisReceive(){
  return _millisReceive;
}
