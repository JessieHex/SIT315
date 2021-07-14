/**
 * The LED blinks when the button is pushed
 * The main loop is busying at counting from 0 to 99
 * when the button is pushed, the interruptPin sense
 * its value goes from high to low
 * so the counting is paused and
 * the 'blink' function is called.
 * After the execution of 'blink', 
 * it goes back to continue counting.
 * When the button is released, the interruptPin sense
 * its value goes from low to high, 
 * so the 'blink' is called again
 * 
*/
const byte ledPin = 7;
const byte interruptPin = 2;
volatile byte state = HIGH;


void setup()
{  
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
}

void loop() {
  for (int i = 0; i < 100; i++)
  {
    Serial.println(i);
    delay(500);
  }
}

void blink() {
  // code that runs when external interrupt is triggered
  Serial.println("External interrupt");
  state = !state;
  digitalWrite(ledPin, state);
}
