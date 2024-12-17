#include <Arduino.h>
#include <Wire.h>
#define SOFT_VERSION "A02_IIC-211130-A0"


// create the sensor object, passing the correct K value.
#define RANGE 2
//1-50 cm, 2-150 cm, 3-250cm, 4-350cm

#define CONE_ANGLE 1  //1-30grd, 2-40grd, 3-50grd, 4-60grd
uint8_t recv_buf[10] = { 0 };
uint16_t distance;

uint8_t address = 0x74;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(38400);
  Wire.begin();
  //Serial.println(SOFT_VERSION);
  //setConeAngle();
  //Wire.beginTransmission(address);
  //Wire.write(0x07);
  //Wire.endTransmission();
  //Wire.requestFrom(address, 1);
  //char x = Wire.read();
  //while(!Serial.available()) Serial.println("Changing I2C address"), delay(1000);
  //changeI2CAddressA22((0xE8)>>1,(0xE0)>>0); 
  //Serial.print("Valoare unghi: ");
  //Serial.println(x, HEX);
}
void changeI2CAddressA22(uint8_t initialAddress, uint8_t finalAddress){
  Wire.beginTransmission(initialAddress);
  Wire.write(0x05);
  Wire.write(finalAddress);
  Wire.endTransmission();
}
void setConeAngle() {
  Wire.beginTransmission(address);
  //Wire.write(0x02);  //doar cere valoarea stocata !!! nu are sens, do not touch
  byte b1 = 0x07;
  byte b2;
  switch (CONE_ANGLE) {
    case 1:
      b2 = (0x01);
      break;
    case 2:
      b2 = (0x02);
      break;
    case 3:
      b2 = (0x03);
      break;
    case 4:
      b2 = (0x04);
      break;
    default:
      break;
  }
  Wire.write(b1);
  Wire.write(b2);
  Wire.endTransmission();
}

void sendMeasureRequest() {
  Wire.beginTransmission(address);
  Wire.write(0x02);  //doar cere valoarea stocata !!! nu are sens, do not touch
  switch (RANGE) {
    case 1:
      Wire.write(0xBD);
      break;
    case 2:
      Wire.write(0xBC);
      break;
    case 3:
      Wire.write(0xB8);
      break;
    case 4:
      Wire.write(0xB4);
      break;
    default:
      break;
  }
  Wire.write(0xB0);  // HOLD
  Wire.endTransmission();
}
void sendReceiveRequest() {
  Wire.beginTransmission(address);
  Wire.write(0x02);  //cere valoare stocata
  int len = 0;
  //0x02 cere valoarea stocata in senzor, in functie de ce comanda a fost data
  //poate fi microsecunde sau mm
  Wire.endTransmission(0);
  Wire.requestFrom(address, 2);
  while (Wire.available()) {
    recv_buf[len++] = Wire.read();
  }
  if (len >= 2) {
    len = 0;
    distance = recv_buf[0] << 8 | recv_buf[1];
  }
}
void loop() {
  int nDevices = 0;

  Serial.println("Scanning...");

  for (byte address = 1; address < 127; ++address) {
    // The i2c_scanner uses the return value of
    // the Wire.endTransmission to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println("  !");

      ++nDevices;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("done\n");
  }
  delay(5000); // Wait 5 seconds for next scan
}
