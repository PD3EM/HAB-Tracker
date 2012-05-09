uint32_t D1 = 0;
uint32_t D2 = 0;
int32_t dT = 0;
int32_t TEMP = 0;
int64_t OFF = 0;
int64_t SENS = 0;
int32_t P = 0;
uint16_t C[7];
int32_t T2 = 0;
int64_t OFF2 = 0;
int64_t SENS2 = 0;

void getMS5607()
{
    D1 = getVal(ADDRESS, 0x48); // Pressure raw
    D2 = getVal(ADDRESS, 0x58);// Temperature raw
  
    dT = (D2 - C[4] * 256);
    TEMP = 2000 + dT * C[5]/8388608;
    OFF  = C[1] * 131072LL + (dT *C[3])/64; // was /128
    SENS = C[0] * 65536LL + (C[2] * dT)/128; // was /256
    P = (D1 * SENS/2097152 - OFF)/32768;
    temp1 = TEMP/100.00;
    press1 = P/100.0 + 10.0;
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
    }
}
