// based on the code by James Coxon on github:
// PicoAtlas / Pico6MK3 / sketch_apr02b / sketch_apr02b.ino 
// thanks to all the UKHAS members and support on #highaltitude

#include <SD.h>
#include <util/crc16.h>
#include <Wire.h>
#include <OneWire.h>

#define ADDRESS 0x76  // MS5607 sensor address

//Variables
int32_t lat = 0, lon = 0, alt = 0;
uint8_t hour = 0, minute = 0, second = 0, lock = 0, sats = 0;
unsigned long startGPS = 0;
int GPSerror = 0, count = 0, n, gpsstatus, lockcount = 0, SDerror= 0;
int temp1 = 0, temp2 = 0;
int32_t press1 = 0;
int error = 0;

//RTTY CONFIG
const int PIN_RTTY_SPACE = 2;
const int PIN_RTTY_MARK = 3;
const int PIN_RTTY_ENABLE = 6; // Active High (was 4)
const int PIN_PWR_LED = 6; // was 13
const int bitRate = 50;

uint8_t buf[70]; //GPS receive buffer
char superbuffer [128]; //Telem string buffer

char SD_LOG_FILENAME[] = "PD3EM.LOG";

const int PIN_SD_CS = 10;
const int PIN_DS18B20 = 7; //DS18S20 Signal pin on digital pin 7

OneWire ds(PIN_DS18B20); // on digital pin

void setup() {
  Serial.begin(9600);
  setupGPS();
  delay(100);
  pinMode(PIN_SD_CS, OUTPUT);
  if (!SD.begin(PIN_SD_CS)) {SDerror=15;}
  pinMode(PIN_RTTY_ENABLE, OUTPUT);
  pinMode(PIN_RTTY_SPACE, OUTPUT);
  pinMode(PIN_RTTY_MARK, OUTPUT);
  digitalWrite(PIN_RTTY_ENABLE, HIGH);
  digitalWrite(PIN_RTTY_MARK, HIGH);
  digitalWrite(PIN_RTTY_SPACE, LOW);
  
  // setup part for MS5607 sensor  
  PORTC |= (1 << 4);
  PORTC |= (1 << 5);
  Wire.begin();
  delay(100);
  initial(ADDRESS);
}

void loop() {
  gps_check_lock();
  gps_get_position();
  gps_get_time();
  getTemp2();
  getMS5607();
  error = SDerror + GPSerror;
  
  n=sprintf (superbuffer, "$$$PD3EM,%d,%02d:%02d:%02d,%ld,%ld,%ld,%d,%d,%d,%d,%d,%d", count, hour, minute, second, lat, lon, alt, sats, temp1, press1, temp2, error,1);
  sdcard_log(superbuffer);   // Write packet to SD card
  txString(superbuffer); //finally, call the RTTY routine to checksum and then transmit the sentence.
  
  count++;
  delay(1000);
}
