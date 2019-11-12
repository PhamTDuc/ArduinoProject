#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(10,9); // CE, CSN
const uint64_t address = 0xE8E8F0F0E1LL;
void setup() {
  Serial.begin(9600);
  radio.begin(); 
  radio.openReadingPipe(0, address); 
  radio.setPALevel(RF24_PA_MIN); 
  radio.setChannel(87);
  radio.setDataRate(RF24_250KBPS); 
  radio.startListening();
}

struct Data
{
  float temp;
  float humid;
  float lightIntensity;
  float airPressure;
};

void loop() {
  if (radio.available()) 
  {
    Data data;
    radio.read(&data,sizeof(data));
    Serial.print("Temprature: ");Serial.print(data.temp);Serial.print("\u00B0");Serial.print("C     ");
    Serial.print("Humidity: ");Serial.print(data.humid);
    Serial.print("     Light intensity: ");Serial.print(data.lightIntensity);Serial.print("lux");
    Serial.print("     Air Pressure: ");Serial.print(data.airPressure);Serial.println("hPa");
    
  }else
  {
      Serial.println("Fail to recieve");
     
  }
  delay(1000);
}
