/*
A22 x8

In setup asteapta comanda "start" primita pe serial, dupa activate.

Exemplu:

activate 1 2 3 4 5

noise S2:L3 S1:L2 S5:L4
cone S2:L2
range S5:L1

delay 5

start

*/
#include "DYPA22.h"
#define MIN_TIME 50
#define DEBUG false

#define RANGE 1
#define LISTEN 0
int sonarDelay = 10;

const char start_command[] = "start";
bool active_status = false;

const char set_delay_command[] = "delay";
const char set_active_sonars[] = "activate";
const char set_noise_command[] = "noise";
const char set_range_command[] = "range";
const char set_cone_command[] = "cone";
const char stop_command[] = "stop";



DYPA22 Sensors[8] = {};
uint8_t addressArray[8] = { 0xDE, 0xDC, 0xDA, 0xD8, 0xD6, 0xD4, 0xD2, 0xE0 };
uint8_t firingSequence[8] = { 0, 4, 2, 6, 1, 5, 3, 7 };  // -1 fata de realitate, array 0-index
//firingSequence IRL : 1, 5, 3, 7, 2, 6, 4, 8
bool activeSonar[8] = {};

uint8_t distances[8] = {};

void setup() {

  Serial.begin(9600);

  Wire.begin();
  delay(10);

  for (int i = 0; i < 8; i++) {
    Sensors[i].setAddress(addressArray[i]);
    delay(1);
    Sensors[i].setRange(0x03);
    delay(1);
    Sensors[i].setPowerNoiseReductionLevel(2);
    delay(1);
    Sensors[i].setConeAngle(3);  //1-2-3-4
    delay(1);
  }
}
void loop() {

  static bool rangeListenToggle = RANGE;  //trece prima data prin array de senzori ca sa le trimita ranging request, apoi mai trece o data sa culeaga rezultatele
  static int currentSonarId = 1;
  static uint32_t millisSonarChange = 0;

  process_commands();
  if (active_status) {
    if (rangeListenToggle == RANGE) {
      if (millis() - millisSonarChange > sonarDelay) {
        int firingId = firingSequence[currentSonarId - 1];  //array cu index de la 0, ex S1 e index 0
        if (activeSonar[currentSonarId - 1]) Sensors[firingId].sendMeasureRequest();
        if (currentSonarId == 8) {
          rangeListenToggle = LISTEN;
          currentSonarId = 1;
        } else {
          currentSonarId++;
        }
        millisSonarChange = millis();
      }
    } else {  //LISTEN
      if (millis() - millisSonarChange > sonarDelay) {
        int firingId = firingSequence[currentSonarId - 1];  //array cu index de la 0
        if (activeSonar[currentSonarId - 1]) {
          Sensors[firingId].sendReceiveRequest();
          delay(1);  //Sa poata raspunde sonarul
          Serial.printf("S%d : %u \n", currentSonarId, Sensors[firingId].getDistance());
        }
        if (currentSonarId == 8) {
          rangeListenToggle = RANGE;
          currentSonarId = 1;
        } else {
          currentSonarId++;
        }
        millisSonarChange = millis();
      }
    }
  }
}

#define BUF_LEN 32
char sdata[BUF_LEN], *pSdata = &sdata[0];

void process_commands() {
  byte ch;
  int val;

  if (Serial.available()) {
    ch = Serial.read();
    Serial.print((char)ch);

    // -1 for null terminator space
    if ((pSdata - sdata) >= BUF_LEN - 1) {
      pSdata = sdata;
      Serial.print("BUFFER OVERRUN\n");
    }

    *pSdata++ = (char)ch;

    if (ch == '\r' || ch == '\n') {  // Command received and ready.

      pSdata--;        // Don't add \r to string.
      *pSdata = '\0';  // Null terminate the string.
      pSdata = sdata;  // Reset pointer to start of string.


      if (strncmp(sdata, set_active_sonars, strlen(set_active_sonars)) == 0) {  //active sonars
        uint8_t arg_offset = strlen(set_active_sonars);
        const char delimiter[] = " ";
        char *id = strtok(sdata + arg_offset, delimiter);
        while (id != NULL) {
          uint8_t id_index = atoi(id) - 1;
          activeSonar[id_index] = 1;
          Serial.printf("Activated S%d (index value %d)\n", id_index + 1, id_index);
          id = strtok(NULL, delimiter);
        }
      } else if (strncmp(sdata, set_noise_command, strlen(set_noise_command)) == 0) {  //noise
        uint8_t arg_offset = strlen(set_noise_command);
        const char delimiter[] = " ";
        char *indiv_command = strtok(sdata + arg_offset, delimiter);
        while (indiv_command != NULL) {
          uint8_t id_index = (indiv_command[1] - '0') - 1;
          uint8_t value = (indiv_command[4] - '0');
          Sensors[id_index].setPowerNoiseReductionLevel(value);
          Serial.printf("Set noise reduction level of S%d to %d\n", id_index + 1, value);
          indiv_command = strtok(NULL, delimiter);
        }
      } else if (strncmp(sdata, set_cone_command, strlen(set_cone_command)) == 0) {  //cone
        uint8_t arg_offset = strlen(set_cone_command);
        const char delimiter[] = " ";
        char *indiv_command = strtok(sdata + arg_offset, delimiter);
        while (indiv_command != NULL) {
          uint8_t id_index = (indiv_command[1] - '0') - 1;
          uint8_t value = (indiv_command[4] - '0');
          Sensors[id_index].setConeAngle(value);
          Serial.printf("Set cone angle of S%d to %d\n", id_index + 1, value);
          indiv_command = strtok(NULL, delimiter);
        }
      } else if (strncmp(sdata, set_range_command, strlen(set_range_command)) == 0) {  //range
        uint8_t arg_offset = strlen(set_range_command);
        const char delimiter[] = " ";
        char *indiv_command = strtok(sdata + arg_offset, delimiter);
        while (indiv_command != NULL) {
          uint8_t id_index = (indiv_command[1] - '0') - 1;
          uint8_t value = (indiv_command[4] - '0');
          Sensors[id_index].setRange(value);
          Serial.printf("Set range of S%d to %d\n", id_index + 1, value);
          indiv_command = strtok(NULL, delimiter);
        }
      } else if (strncmp(sdata, set_delay_command, strlen(set_delay_command)) == 0) {  //delay
        uint8_t arg_offset = strlen(set_delay_command);
        if (strlen(sdata) > arg_offset) {
          val = atoi(&sdata[arg_offset]);
          sonarDelay = val;
          Serial.printf("Changed sweep delay between sonars to %d", sonarDelay);
        }
      } else if (strncmp(sdata, start_command, strlen(start_command)) == 0) {  //start_command
        active_status = true;
        Serial.printf("Received start command");
      } else if (strncmp(sdata, stop_command, strlen(stop_command)) == 0) {  //stop command
        active_status = false;
        Serial.printf("Received stop command");
      }
    }  // if \r
    else {
    }
  }  // available
}