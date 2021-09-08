/*
   https://forum.pjrc.com/threads/67454-UART-I2C-display-gt-not-working-over-1200-baud-rate-for-UART?p=281054#post281054
*/

int pin0 = A0;
int pin1 = A1;
unsigned long my_time;
int reset_switch = 16;
int reset_pin = 0;

int messageCount = 0;

elapsedMillis timerScrRefresh;

void setup() {
  // put your setup code here, to run once:
  pinMode(reset_switch, INPUT_PULLUP);
  Serial.begin(9600);
  Serial1.begin(1000000);
  Serial.println("Start Send");
  if (Serial.available() > 0) {
    Serial.clear();
    delay(50);
  }
  if (Serial1.available() > 0) {
    Serial1.clear();
    delay(50);
  }
}

void loop() {
  if (timerScrRefresh > 500) {
    String message_ = "c23f34g434X";
    sendTX1(message_);
    Serial.println(message_);

    timerScrRefresh = 0;
  }
}

void sendTX1(String mess_) {
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
