void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.write(192);
  Serial.print("text ");
  Serial.print(millis());
  Serial.print(" ");
  Serial.print(random(0,10));
  Serial.write(192);

  Serial.println("");
  delay(50);
}
