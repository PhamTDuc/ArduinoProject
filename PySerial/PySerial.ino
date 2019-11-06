void setup() {
  Serial.begin(9600);

}

void loop() {
  Serial.println(random(-273,60));
  Serial.println(random(1,200));
  delay(200);
}
