#include "Arduino.h"
#include "SoftwareSerial.h"
// #include "TinyGPS.h"
#include <Wire.h>
#define ADDRESS 0x76
#define PUBX "$PUBX,00*33"

const int debugRX = 8; //DEBUG software serial RX pin 8
const int debugTX = 9; //DEBUG software serial TX pin 9
const int dbgBaud = 9600;//DEBUG software serial baud rate
char SS_IN_BYTE;
const int gpsBaud = 9600; 
SoftwareSerial debugSerial(debugRX, debugTX);

void setup() {
  Wire.begin();
  delay(100);
//  initial(ADDRESS);
  debugSerial.begin(dbgBaud);
  Serial.begin(gpsBaud); //set up the GPS serial port

}

void loop() {
  Serial.println(PUBX);
  
  long startTime = millis();
  while (millis() < startTime + 3000) {
    if (Serial.available()) {
      char SS_IN_BYTE = Serial.read();
      debugSerial.print(SS_IN_BYTE);
      
    }
  }
}


