#include <Wire.h>
//#include<LiquidCrystal.h>

//LiquidCrystal lcd(12,11,5,4,3,2);

// DS1621 demo
// -- by Jon McPhalen (www.jonmcphalen.com)
// -- 21 DEC 2007

// SDA pin is 20 (Arduino Mega) -Updated by Sunny Srinidhi
// SCL pin is 21 (Arduino Mega) -Updated by Sunny Srinidhi
// DS1621 has A2, A1, and A0 pins connected to GND

// device ID and address

#define DEV_TYPE   0x90 >> 1                    // shift required by wire.h
#define DEV_ADDR   0x00                         // DS1621 address is 0
#define SLAVE_ID   DEV_TYPE | DEV_ADDR

// DS1621 Registers & Commands

#define RD_TEMP    0xAA                         // read temperature register
#define ACCESS_TH  0xA1                         // access high temperature register
#define ACCESS_TL  0xA2                         // access low temperature register
#define ACCESS_CFG 0xAC                         // access configuration register
#define RD_CNTR    0xA8                         // read counter register
#define RD_SLOPE   0xA9                         // read slope register
#define START_CNV  0xEE                         // start temperature conversion
#define STOP_CNV   0X22                         // stop temperature conversion

// DS1621 configuration bits

#define DONE       B10000000                    // conversion is done
#define THF        B01000000                    // high temp flag
#define TLF        B00100000                    // low temp flag
#define NVB        B00010000                    // non-volatile memory is busy
#define POL        B00000010                    // output polarity (1 = high, 0 = low)
#define ONE_SHOT   B00000001                    // 1 = one conversion; 0 = continuous conversion

float highTresh=29.00;
float lowTresh=27.00;

int buzzer=52;                                  //A piezo buzzer connected to pin 52 of Arduino Mega. This will be used to 

void setup()
{
 Wire.begin();                                 // connect I2C
 startConversion(false);                       // stop if presently set to continuous
 setConfig(POL | ONE_SHOT);                    // Tout = active high; 1-shot mode
 setThresh(ACCESS_TH, highTresh);                     // high temp threshold = 80F
 setThresh(ACCESS_TL, lowTresh);                     // low temp threshold = 75F

 Serial.begin(9600);
 //Serial.println("Ambient Temperature using Arduino and DS1621 I2C Temperature Sensor");
 //Serial.println("Code modified by Sunny Srinidhi");

 /*lcd.begin(16,2);
 lcd.setCursor(0,0);
 lcd.print("AmbTemp - Sunny");
 lcd.setCursor(0,1);*/

 int tHthresh = getTemp(ACCESS_TH);
 Serial.print("High threshold = ");
 Serial.print(tHthresh);
 Serial.println(" degC");

 int tLthresh = getTemp(ACCESS_TL);
 Serial.print("Low threshold = ");
 Serial.print(tLthresh);
 Serial.println(" degC");
 
 pinMode(buzzer,OUTPUT);
 
}


void loop()
{
 float tC, tFrac;
 float tF;
 
  tC = getHrTemp();                             // read high-resolution temperature
  //tC = getTemp(RD_TEMP);                             // read Low-resolution temperature

//  delay(5);
//  Serial.print(" ");
//  delay(5);
  
 if (tC < 0) {
   tC = -tC;                                   // fix for integer division
   Serial.print("-");                          // indicate negative
 }
 
  Serial.print(tC/100);
  Serial.print("C, ");
  
  /*lcd.setCursor(0,1);
  lcd.print(tC/100);
  lcd.print("C");*/


//tFrac = tC % 100;                             // extract fractional part
tC /= 100;                                    // extract whole part

tF= (tC*9.0)/5.0 +32;                                //Convert to Fahrenheit  
  
  Serial.print(tF);
  Serial.println("F.");
  
  /*lcd.setCursor(7,1);
  lcd.print(tF);
  lcd.print("F");*/
  
  /*if(tC>=highTresh)
  {
    digitalWrite(buzzer,HIGH);
    lcd.setCursor(14,1);
    lcd.print("Hi");
  }
  else if(tC<=lowTresh)
  {
    lcd.setCursor(14,1);
    lcd.print("Lo");
    digitalWrite(buzzer,LOW);    
  }
  else
  {
    lcd.setCursor(14,1);
    lcd.print("  ");
    digitalWrite(buzzer,LOW);    
  }*/
    
  delay(500);
}


// Set configuration register

void setConfig(byte cfg)
{
 Wire.beginTransmission(SLAVE_ID);
 Wire.write(ACCESS_CFG);
 Wire.write(cfg);
 Wire.endTransmission();
 delay(15);                                    // allow EE write time to finish
}


// Read a DS1621 register

byte getReg(byte reg)
{
 Wire.beginTransmission(SLAVE_ID);
 Wire.write(reg);                               // set register to read
 Wire.endTransmission();
 Wire.requestFrom(SLAVE_ID, 1);
 byte regVal = Wire.read();
 return regVal;
}


// Sets temperature threshold
// -- whole degrees C only
// -- works only with ACCESS_TL and ACCESS_TH

void setThresh(byte reg, int tC)
{
 if (reg == ACCESS_TL || reg == ACCESS_TH) {
   Wire.beginTransmission(SLAVE_ID);
   Wire.write(reg);                             // select temperature reg
   Wire.write(byte(tC));                        // set threshold
   Wire.write(0);                               // clear fractional bit
   Wire.endTransmission();
   delay(15);
 }
}


// Start/Stop DS1621 temperature conversion

void startConversion(boolean start)
{
 Wire.beginTransmission(SLAVE_ID);
 if (start == true)
   Wire.write(START_CNV);
 else
   Wire.write(STOP_CNV);
 Wire.endTransmission();
}


// Reads temperature or threshold
// -- whole degrees C only
// -- works only with RD_TEMP, ACCESS_TL, and ACCESS_TH

int getTemp(byte reg)
{
 int tC;

 if (reg == RD_TEMP || reg == ACCESS_TL || reg == ACCESS_TH) {
   byte tVal = getReg(reg);
   if (tVal >= B10000000) {                    // negative?
     tC = 0xFF00 | tVal;                       // extend sign bits
   }
   else {
     tC = tVal;
   }
   return tC;                                  // return threshold
 }
 return 0;                                     // bad reg, return 0
}


// Read high resolution temperature
// -- returns temperature in 1/100ths degrees
// -- DS1620 must be in 1-shot mode

int getHrTemp()
{
 startConversion(true);                        // initiate conversion
 byte cfg = 0;
 while (cfg < DONE) {                          // let it finish
   cfg = getReg(ACCESS_CFG);
 }

 int tHR = getTemp(RD_TEMP);                   // get whole degrees reading
 byte cRem = getReg(RD_CNTR);                  // get counts remaining
 byte slope = getReg(RD_SLOPE);                // get counts per degree

 if (tHR >= 0)
   tHR = (tHR * 100 - 25) + ((slope - cRem) * 100 / slope);
 else {
   tHR = -tHR;
   tHR = (25 - tHR * 100) + ((slope - cRem) * 100 / slope);
 }
 return tHR;
}
