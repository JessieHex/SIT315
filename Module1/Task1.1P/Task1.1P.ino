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
  if (val == HIGH)
      Serial.println("motion detected...");
  else
      Serial.println("inactive...");
      
  digitalWrite(13, val);
  delay(500); // Wait for 500 millisecond(s)
  digitalWrite(13, LOW); // turn off the LED anyway to make it blinks
  delay(500); // Wait for 500 millisecond(s)
}
