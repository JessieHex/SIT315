/**
 * The LED turns on when the button is held
 * The LED turns off when the button is released
 * when the button is pushed, the buttonPin sense
 * its value goes from high to low
 * so the led state will be reversed. (off to on)
 * When the button is released, the interruptPin sense
 * its value goes from low to high, 
 * so the led state will be reversed again. (on to off)
 * 
*/
const byte ledPin = 7;
const byte buttonPin = 2;
volatile byte prevState = HIGH;


void setup()
{  
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  byte state = digitalRead(buttonPin);
  if (state != prevState) {
    digitalWrite(ledPin, !state);
    prevState = state;     
  }
  Serial.print("input: ");
  Serial.print(state);
  Serial.print(" output: ");
  Serial.println(!state);
  delay(500);
}

