#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9,10); // CE, CSN
const uint64_t address = 0xE8E8F0F0E1LL;

#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#include <Wire.h>
#include <SFE_BMP180.h>
SFE_BMP180 bmp180;

int BH1750_address = 0x23;
byte buff[2];

void setup() {
  Serial.begin(9600);
  radio.begin(); 
  radio.openWritingPipe(address); 
  radio.setPALevel(RF24_PA_MIN); 
  radio.setChannel(87); 
  radio.setDataRate(RF24_250KBPS); 
  radio.stopListening(); 

  dht.begin();
  bmp180.begin();
  
  Wire.begin();
  BH1750_Init(BH1750_address);
}

void BH1750_Init(int address){
  
  Wire.beginTransmission(address);
  Wire.write(0x10); // 1 [lux] aufloesung
  Wire.endTransmission();
}
byte BH1750_Read(int address){
  
  byte i=0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);
  while(Wire.available()){
    buff[i] = Wire.read(); 
    i++;
  }
  Wire.endTransmission();  
  return i;
}


struct Data
{
  float temp;
  float humid;
  float lightIntensity;
  float airPressure;
};


void loop() 
{
  Data data;
  
  float tempTmp=dht.readTemperature();
  float humidTmp=dht.readHumidity();
  if(isnan(tempTmp)||isnan(humidTmp))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }else
  {
    data.temp=tempTmp;
    data.humid=humidTmp;
    Serial.print("Temprature: ");Serial.print(data.temp);Serial.print("\u00B0");Serial.print("C     ");
    Serial.print("Humidity: ");Serial.print(data.humid);
  }

//Pressure
  char status;
  double T, P;
  status = bmp180.startTemperature();
  if (status != 0) {
    status = bmp180.getTemperature(T);
    if (status != 0) {
      status = bmp180.startPressure(3);
      if (status != 0) {
        delay(status);
        status = bmp180.getPressure(P, T);
        if (status != 0) {
          Serial.print("Pressure: ");
          Serial.print(P);
          Serial.print(" hPa   ");
        }
      }
    }
  }
  data.airPressure = P;


  //LightIntensity
  float valf=0;
  if(BH1750_Read(BH1750_address)==2){
    
    valf=((buff[0]<<8)|buff[1])/1.2;
    
    if(valf<0)Serial.print("> 65535");
    else Serial.print((int)valf,DEC); 
    
    Serial.println(" lux"); 
    data.lightIntensity=int(valf);
  }
  

 
  if(!radio.write(&data, sizeof(data)))
  {
    Serial.println("Fail to transmit");  
  }
  delay(100);
}
