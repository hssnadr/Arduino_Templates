int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor

#define N 50
int dataColl[N];
int index = 0;

long timePrint0 = 0;
int dlyPrint = 10;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  // init data collection
  for(int i=0; i<N; i++) {
    dataColl[i] = 0;
  }
}

void loop() {
  // update index
  if(index < N && index >= 0) {
    index++;
  }
  else {
    index = 0;
  }

  // update data collection
  dataColl[index] = analogRead(sensorPin);

  // get moving mean
  float mean_ = 0.0f;
  for (int i=0; i<N;i++) {
    mean_ += dataColl[i];
  }
  mean_ /= N;
  
  if(millis() - timePrint0 > dlyPrint) {
    Serial.println(mean_);
    analogWrite(ledPin, mean_);
    timePrint0 = millis();
  }
}
