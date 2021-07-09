// LED blinks while motion detected by PIR
// external interrupt version
const byte ledPin = 13;
const byte interruptPin = 2;
volatile byte state = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
}

void loop() {
  if (state == HIGH)
  {
    Serial.println("motion detected...");
    digitalWrite(ledPin, state); // led on
    delay(500);
  }
  else
  {
    Serial.println("inactive...");
  }
  digitalWrite(ledPin, LOW); // led off
  delay(500);
  
}

void blink() {
  state = !state;
  
}
