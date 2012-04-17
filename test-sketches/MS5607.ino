// Simplest code to interface the MS5611/5607

#include <Wire.h>

#define ADDRESS 0x76

uint32_t D1 = 0;
uint32_t D2 = 0;
int32_t dT = 0;
int32_t TEMP = 0;
int64_t OFF = 0;
int64_t SENS = 0;
int32_t P = 0;
float temp1 =0;
float press1 =0;
uint16_t C[7];

void setup() {

  // Disable internal pullups, 10Kohms are on the breakout
    PORTC |= (1 << 4);
    PORTC |= (1 << 5);
    
    Wire.begin();
    Serial.begin(9600);
    delay(100);
    initial(ADDRESS);
    Serial.println("temp1 press1");
}

void loop()
{
    D1 = getVal(ADDRESS, 0x48); // Pressure raw
    D2 = getVal(ADDRESS, 0x58);// Temperature raw
  
    dT = (D2 - C[4] * 256);
    TEMP = 2000 + dT * C[5]/8388608;
    OFF  = C[1] * 131072LL + (dT *C[3])/64; // was /128
    SENS = C[0] * 65536LL + (C[2] * dT)/128; // was /256
    P = (D1 * SENS/2097152 - OFF)/32768;
    temp1 = TEMP/100.00;
    press1 = P/100.00;
    Serial.print(temp1,1);
    Serial.print(" ");
    Serial.println(press1,1);
    delay(5000);
}

long getVal(int address, byte code)
{
    unsigned long ret = 0;
    Wire.beginTransmission(address);
    Wire.write(code);
    Wire.endTransmission();
    delay(10);
    // start read sequence
    Wire.beginTransmission(address);
    Wire.write((byte) 0x00);
    Wire.endTransmission();

    Wire.beginTransmission(address);
    Wire.requestFrom(address, (int)3);
    if (Wire.available() >= 2) {
    ret = Wire.read () ;
    ret = (ret << 8) | Wire.read () ;
    ret = (ret << 8) | Wire.read () ;
    
    }
    else {
        ret = -1;
    }
    Wire.endTransmission();
    return ret;
}

void initial(uint8_t address)
{
    Serial.println();
    Serial.println("PROM COEFFICIENTS");
    
    Wire.beginTransmission(address);
    Wire.write(0x1E); // reset
    Wire.endTransmission();
    delay(10);

    for (int i=0; i<7; i++) {
        Wire.beginTransmission(address);
        Wire.write(0xA2 + (i * 2));
        Wire.endTransmission();

        Wire.beginTransmission(address);
        Wire.requestFrom(address, (uint8_t) 6);
        delay(1);
        if (Wire.available() >= 2) {
            C[i] = Wire.read() *256 + Wire.read();
        }
        else {
            Serial.println("Error reading PROM 1"); // error reading the PROM or communicating with the device
        }
        Serial.println(C[i]);
    }
    Serial.println();
}
