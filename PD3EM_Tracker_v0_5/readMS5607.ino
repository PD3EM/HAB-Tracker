void getMS5607()
{
   int32_t T2 = 0;
   int64_t OFF2 = 0;
   int64_t SENS2 = 0;
    D1 = getVal(ADDRESS, 0x48); // Pressure raw
    D2 = getVal(ADDRESS, 0x58);// Temperature raw
  
    dT = (D2 - C[4] * 256);
    TEMP = 2000 + dT * C[5]/8388608LL;
    OFF  = C[1] * 131072LL + (dT *C[3])/64; // was /128
    SENS = C[0] * 65536LL + (C[2] * dT)/128; // was /256
    
    if ( TEMP < 2000){
      T2 = pow(dT,2) / 2147483648LL;
      OFF2 = 61 * pow((TEMP-2000),2) / 16;
      SENS2 = 2 * pow((TEMP-2000),2);
    
    if (TEMP < -1500){
      OFF2 = OFF2 + 15 * pow((TEMP +1500),2);
      SENS2 = SENS2 + 8 * pow((TEMP +1500),2);
    }
    }
    TEMP = TEMP - T2;
    OFF = OFF - OFF2;
    SENS = SENS - SENS2;

    P = (D1 * SENS/2097152LL - OFF)/32768LL;
    temp1 = TEMP/100.00;
    press1 = P/100.0;
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
