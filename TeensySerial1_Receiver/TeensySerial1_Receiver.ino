/*
   https://forum.pjrc_.com/threads/67454-UART-I2C-display-gt-not-working-over-1200-baud-rate-for-UART?p=281054#post281054
*/

//Receiver Code

const byte numChars = 32;
char rx1Chars[numChars];
char tempChars[numChars];        // temporary array for use when parsing
static byte charIndx = 0;
boolean newData = false;
char startMarker = '<';
char endMarker = '>';

void setup() {
  Serial.begin(9600);
  Serial1.begin(1000000);

  Serial.println("Start Receive");
  if (Serial.available() > 0) {
    Serial.clear();
    delay(50);
  }

  initRX1();
  Serial.println("start");

}

void loop() {
  getRX1message();
  if (newData == true) {
    strcpy(tempChars, rx1Chars);

    String msg_ = "";
    for (int i = 0; i < charIndx; i++) {
      msg_ += (char)tempChars[i];
    }
    Serial.println(msg_);

    newData = false;
  }
}
