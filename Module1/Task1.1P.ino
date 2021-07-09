// if the PIR sensor detects any motion
// then the build-in LED will blink
int val = LOW;
void setup()
{
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(13, OUTPUT);
}

void loop()
{
  val = digitalRead(2);
  if (val == HIGH) {
      Serial.println("motion detected...");
      digitalWrite(13, HIGH); // turn on the LED
      delay(500); // Wait for 500 millisecond(s)
  }
  else {
      Serial.println("inactive...");
  }
  
  digitalWrite(13, LOW); // turn off the LED anyway to make it blinks if it was on
  delay(500); // Wait for 500 millisecond(s)
}
