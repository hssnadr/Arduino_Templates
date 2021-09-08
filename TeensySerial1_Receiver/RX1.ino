void initRX1() {
  if (Serial1.available() > 0) {
    Serial1.clear();
    delay(50);
  }
}

void getRX1message() {
  static boolean recvInProgress_ = false;
  char rc_;

  while (Serial1.available() > 0 && newData == false) {
    rc_ = Serial1.read();

    if (recvInProgress_ == true) {
      if (rc_ != endMarker) {
        rx1Chars[charIndx] = rc_;
        charIndx++;
        if (charIndx >= numChars) {
          charIndx = numChars - 1;
        }
      }
      else {
        rx1Chars[charIndx] = '\0'; // terminate the string
        recvInProgress_ = false;
        newData = true;
      }
    }
    else if (rc_ == startMarker) {
      charIndx = 0;
      recvInProgress_ = true;
    }
    else {
      Serial.print("Unexpected character: ");
      Serial.println(rc_);
    }
  }
}
