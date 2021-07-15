/**
* We have three led lights, red, yellow and green.
* By timer interrupt, each light will be turned on for 2 seconds.
* In a order from top to bottom red -> yellow -> green
* We also have two buttons and one PIR sensor.
* They can interrupt the order, 
* set the corresponding led as current led. 
* The button on the right - green
* The button on the left - yellow
* The PIR sensor - red
*/
const int ledRed = 5;
const int ledYellow = 6;
const int ledGreen = 7;
const int button = 2;// external interrupt INT0
const int pir = 3; // external interrupt INT1
const int button2 = 8; // pin change interrupt PCINT0
volatile int currentLed = ledGreen; // current on led

void setup() {
  Serial.begin(9600);
  // set up all the LEDs as OUTPUT
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  
  // set input
  pinMode(button, INPUT_PULLUP);
  pinMode(pir, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  
  noInterrupts();
  
  // Clear registers
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 =0;
  
  // switch light every 2 seconds
  // 46875 = 3/(62.5*10**-9)/1024
  
  // Set timer compare
  OCR1A = 31250;
  
  // Prescaler 1024
  TCCR1B |= (1 << CS12) | (1 << CS10);
  
  // outout compare match A interrupt enale
  TIMSK1 |= (1 << OCIE1A);
  
  // CTC
  TCCR1B |= (1 << WGM12);
  
  interrupts();
  
  attachInterrupt(digitalPinToInterrupt(button), greenOn, FALLING);
  attachInterrupt(digitalPinToInterrupt(pir), redOn, RISING);
  
  // turn innterrupts on port b
  PCICR |= 1 << PCIE0;
  
  // enable the pin8 for pin change interrupt
  PCMSK0 |= 1 << PCINT0; 
}

void loop() { 
  Serial.println("...");
  delay(1000);
}

void greenOn() {
  Serial.println("Button interrupt");
  digitalWrite(ledGreen, HIGH);
  digitalWrite(ledRed, LOW);
  digitalWrite(ledYellow, LOW);
  currentLed = ledGreen;
}

void redOn() {
  Serial.println("PIR interrupt");
  digitalWrite(ledRed, HIGH);
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledYellow, LOW);
  currentLed = ledRed;
}

ISR(TIMER1_COMPA_vect) {
  Serial.println("Timer interrupt");
  digitalWrite(currentLed, LOW);
  currentLed = currentLed + 1;
  if (currentLed > ledGreen)
    currentLed = ledRed;
  digitalWrite(currentLed, HIGH);
}

// yellow On
ISR(PCINT0_vect) {
  Serial.println("Pin change interrupt");
  digitalWrite(ledYellow, HIGH);
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledRed, LOW);
  currentLed = ledYellow;
}

