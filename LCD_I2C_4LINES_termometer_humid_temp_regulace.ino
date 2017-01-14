//divisl 24.11.2014 22:03
//#include <OneWireTempSensor.h>
//#include <DallasTemperature.h>
#include <OneWire.h>
//#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#define DHTPIN 12
#define DHTTYPE DHT11

int DS18S20_Pin = 13; //DS18S20 signal na pinu 7
int BLED = 2;
int GLED = 3;
int RLED = 4;
int nadrz = 5;
LiquidCrystal_I2C lcd(0x27,20,4);  // adresa 0x27 na sbernici I2C
OneWire ds(DS18S20_Pin); // digitalni pin 7
DHT dht(DHTPIN, DHTTYPE);
//float nadrztemp = 0;

void setup()
{
  lcd.init();      //  zapinam display
  lcd.backlight(); //  zapinam podsviceni
  //lcd.clear(); 
// start with a blank screen
  dht.begin();
  Serial.begin(9600);
  
  pinMode(nadrz, OUTPUT);
  pinMode(BLED, OUTPUT); // RGB LED's
  pinMode(GLED, OUTPUT);
  pinMode(RLED, OUTPUT);
  digitalWrite(BLED, HIGH); // set nadrz LED's off
  digitalWrite(GLED, HIGH);
  digitalWrite(RLED, HIGH);
  
}

void loop() {
  float temperature = getTemp();
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.println(temperature);
  lcd.home (); //lcd.setCursor(0, 0); //tady je prvni radek, prvni znak lcd
  lcd.print("Nadrz:");
  lcd.setCursor(17, 0);
  lcd.print("oC");
  lcd.setCursor(0, 1);
  lcd.print("Vzduch:");
  lcd.setCursor(10, 1);
  lcd.print(t);
  lcd.setCursor(17, 1);
  lcd.print("oC");
  lcd.setCursor(0, 2);
  lcd.print("Vlhkost:");
  lcd.setCursor(17, 2);
  lcd.print("%");
  lcd.setCursor(10, 2);
  lcd.print(h);
  lcd.setCursor(10, 0);
  lcd.print(temperature);
  lcd.setCursor(0, 3);
  lcd.print("Topeni:");
  //lcd.setCursor(10, 3);
  //lcd.print("ON/OFF");
  
  delay(500); 
 
 if (temperature <=26)
{
  digitalWrite(nadrz, LOW);
  digitalWrite(BLED, LOW);
  digitalWrite(GLED, LOW);
  digitalWrite(RLED, HIGH);
  lcd.setCursor(10, 3);
  lcd.print("zapnuto  ");
}

if (temperature  >= 26.01 && temperature  <= 26.99)

{
  
  digitalWrite(nadrz, HIGH);
  digitalWrite(GLED, HIGH);
  digitalWrite(RLED, HIGH);
  digitalWrite(BLED, HIGH);
  lcd.setCursor(10, 3);
  
  lcd.print("v rozsahu");
}

if (temperature  >= 27)
{
  digitalWrite(nadrz, HIGH);
  digitalWrite(RLED, LOW);
  digitalWrite(GLED, HIGH);
  digitalWrite(BLED, LOW);
  lcd.setCursor(10, 3);
  lcd.print("vypnuto  ");
}
   
 
 }

float getTemp(){
  //vraci teplotu ve stupnich celsia

  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
    //zadne dalsi senzory, reset hledani
    ds.reset_search();
    return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
    Serial.println("Neplatne CRC!");
    return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
    Serial.print("Zarizeni nerozpoznano");
    return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);
  ds.write(0xBE); // Read Scratchpad


  for (int i = 0; i < 9; i++) { // potrebuju 9 bytes
    data[i] = ds.read();
  }

  ds.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;

  return TemperatureSum;



}

