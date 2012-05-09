//Temperature chip i/o

void getTemp2(){
//returns the temperature from one DS18S20 in DEG Celsius

byte data[12];
byte addr[8];

if ( !ds.search(addr)) {
//no more sensors on chain, reset search
ds.reset_search();
}

ds.reset();
ds.select(addr);
ds.write(0x44,1); // start conversion, with parasite power on at the end

byte present = ds.reset();
ds.select(addr);
ds.write(0xBE); // Read Scratchpad

for (int i = 0; i < 9; i++) { // we need 9 bytes
data[i] = ds.read();
}

ds.reset_search();

byte MSB = data[1];
byte LSB = data[0];

float tempRead = ((MSB << 8) | LSB); //using two's compliment
temp2 = tempRead / 16;

}
