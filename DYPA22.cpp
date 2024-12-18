#include "DYPA22.h"
void DYPA22::setAddress(uint8_t address){
  _address = (address)>>1;
}
DYPA22::DYPA22(uint8_t address8 = 0xE8, uint8_t range = 0x02){
  setRange(range);
  setAddress(address8);
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