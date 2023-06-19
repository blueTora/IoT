#include <Servo.h>

Servo binServo;

#define TrigPin 26
#define EchoPin 27 
 
void setup() {
  Serial.begin(9600);
  binServo.attach(48);
  binServo.writeMicroseconds(1000);
  
  pinMode(TrigPin, OUTPUT);  
  pinMode(EchoPin, INPUT);
}

int const holdTime = 3000;
int startTime = 0;
 
void loop() {
   delay(500);
  
   long dist = calDist();
   if(dist <= 20){
    
    if(startTime == 0)
      startTime = millis();
      
    if(millis() - startTime > holdTime){
      Serial.println("Bin Open.");
      Serial.println();
      binServo.writeMicroseconds(1500);//open
    }
    
   } else {
    Serial.println("Bin Close.");
    Serial.println();
    binServo.writeMicroseconds(1000);//close
    startTime = 0;
   }
}

int calDist(){
   digitalWrite(TrigPin, LOW);
   delayMicroseconds(2);
   
   digitalWrite(TrigPin, HIGH);
   delayMicroseconds(10);
   
   digitalWrite(TrigPin, LOW);
   
   long duration = pulseIn(EchoPin, HIGH);
   long dist = duration / 29 / 2;
   
   Serial.print(dist);
   Serial.println(" cm");

   return dist;
}
