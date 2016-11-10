// Serial variables
int inByte = 0;  // incoming serial byte
char buffer[40]; // buffer to store incoming bytes
int index = 0; // current index of the buffer
char msgAdr = 'X'; // address of received messages
int msgVal = -1; // values of received messages

//RGB values
int r = 0;
int g = 0;
int b = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  //!\\ Each incoming messages must be separated by a space ('32' in byte)
  getSerialMsg(); // update msgAdr & msgMsg
}

void getSerialMsg() {
  // CHECK SERIAL AND UPDATE msgAdr & msgMsg
  //----------------------------------------
  
  //First thing is to check if some messages have been sent
  if (Serial.available() > 0) {
    msgAdr = (char)Serial.read(); // The first incoming byte correspond to the address message, it's supposed to be a char
    
    index = 0;
    do
    {
      buffer[index] = Serial.read();// get a byte from the serial port
      if (buffer[index] != -1) {
        index++; // -1 if no byte is present
      }
    } while (buffer[index - 1] != 32); //keep collecting bytes until a space is received

    msgVal = atoi(buffer);       // interpret buffer string as an integer and get the value corresponding to the address
    updateValues(); // this function is placed here for the example but it's up to you
  }
}

void updateValues() {
  Serial.print("Message address: ");
  Serial.print(msgAdr);
  Serial.println("");
  Serial.print("Message value: ");
  Serial.print(msgVal);
  Serial.println("");
  
  // the address is used to route the message value
  switch (msgAdr) {
    case 'r':
      r = msgVal;
      break;
    case 'g':
      g = msgVal;
      break;
    case 'b':
      b = msgVal;
      break;
    default:
      Serial.println("No matching address");
      break;
  }

  Serial.print("RGB: ");
  Serial.print("\t");
  Serial.print(r);
  Serial.print("\t");
  Serial.print(g);
  Serial.print("\t");
  Serial.print(b);
  Serial.println("");
  Serial.println("----------------------------");
}
