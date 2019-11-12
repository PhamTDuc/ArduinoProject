#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE, CSN
const uint64_t address = 0xE8E8F0F0E1LL;
void setup() {
  Serial.begin(9600);
  radio.begin(); 
  radio.openWritingPipe(address); 
  radio.setPALevel(RF24_PA_MIN); 
  radio.setChannel(87); 
  radio.setDataRate(RF24_250KBPS); 
  radio.stopListening(); 
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
  data.temp=4.44;
  data.humid=6.25;
  data.lightIntensity=223.5;
  data.airPressure =100.139;
  if(!radio.write(&data, sizeof(data)))
  {
    Serial.println("Fail to transmit");  
  }
  delay(1000);
}
