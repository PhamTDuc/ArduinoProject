void setup() {
  Serial.begin(9600);

}

void loop() {
  Serial.println(random(0,60));
  Serial.println(random(0,100));
  Serial.println(random(300,1100));
  Serial.println(random(0,70000));
  delay(200);
}
