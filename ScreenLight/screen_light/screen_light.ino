#define LDRPin A5
#define LED 4

void setup() {
  pinMode(LDRPin, INPUT);
  pinMode(LED,OUTPUT);
}

int light = 0;

void loop() {
  light = analogRead(LDRPin);
  
  light = map(light, 0, 1023, 0, 255);
  analogWrite(LED, light);
  
  delay(100);
}
