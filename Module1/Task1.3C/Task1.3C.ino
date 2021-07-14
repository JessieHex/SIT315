/**
 * The LED can be controlled by the button or the PIR sensor
 * 
*/
const byte ledPin = 7;
const byte buttonPin = 2;
const byte pirPin = 3;
volatile byte state = LOW;


void setup()
{  
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(pirPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), toggle, FALLING);
  attachInterrupt(digitalPinToInterrupt(pirPin), toggle1, RISING);
}

void loop() {
  digitalWrite(ledPin, state);
}

void toggle() {
  // code that runs when external interrupt0 is triggered
  Serial.println("interrupt by button");
  state = !state;
}

void toggle1() {
  // code that runs when external interrupt1 is triggered
  Serial.println("interrupt by pir");
  state = !state;
}
