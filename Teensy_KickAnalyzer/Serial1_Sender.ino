int messageCount = 0;

void setupSerial1() {
  Serial1.begin(1000000);
  if (Serial1.available() > 0) {
    Serial1.clear();
    delay(50);
  }
}

void sendToSerial1(String mess_) {
  sendTX1(mess_);
  // Serial.println(mess_);
}

void sendTX1(String mess_) {
  Serial.println(mess_);
  
  Serial1.print("<");
  Serial1.print(mess_);
  Serial1.print(">");

  // once every 5000 messages, allow a brief silent time
  // for the receiver to detect line idle and then start bit
  if (++messageCount >= 5000) {
    Serial1.flush(); // wait for buffered data to transmit
    delayMicroseconds(10); // then wait approx 10 bit times
    messageCount = 0;
  }
}
