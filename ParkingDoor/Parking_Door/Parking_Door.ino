#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial mySerial(10, 11); // RX, TX
Servo myservo;

#define LED 21
#define TrigPin 38
#define EchoPin 36
 
void setup(){
  mySerial.begin(9600);
  Serial.begin(9600);

  myservo.attach(13);
  myservo.writeMicroseconds(1000);

  pinMode(TrigPin, OUTPUT);  
  pinMode(EchoPin, INPUT);
  
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

long openedTime = -1;
const int maxOpenTime = 10000; // 10 seconds

void loop(){
  if (mySerial.available()) {
    char inByte = mySerial.read();
    Serial.print("Key Entered is: ");
    Serial.println(inByte);
    
    if(openedTime < 0 && inByte == '1'){
      openDoor(0);
            
    } else if(openedTime > 0 && inByte == '3'){
      if(checkDist())
        closeDoor();
    }
  }

  if(openedTime > 0 && millis() - openedTime >= maxOpenTime){
      if(checkDist())
        closeDoor();
  }
  
  if(openedTime > 0){
      Serial.print("Door Timer: ");
      Serial.print((millis() - openedTime)/1000.0);
      Serial.println(" s");
      delay(500);
  }
}

const int dangerDist = 100; //1 meter distance

bool checkDist(){
   long dist = calDist();
   
   if(dist <= dangerDist){
      Serial.println("There is a Car Blocking the Door!");
      return false;
   }

   return true;
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

void openDoor(int startAngle){
  Serial.println("Door Opening...");
  openedTime = millis();
  digitalWrite(LED, HIGH);
  
  for (int p = startAngle; p <= 90; p += 1) {
    int deg = map(p, 0, 180, 1000, 2000);
    myservo.writeMicroseconds(deg);
    delay(10);                       
  }
  
  digitalWrite(LED, LOW);
  Serial.println("Door Opened.");
}

void closeDoor(){
  Serial.println("Door Closing...");
  openedTime = -1;
  digitalWrite(LED, HIGH);
  
  for (int p = 90; p >= 0; p -= 2) {
    if(!checkDist()){
      openDoor(p);
      return;
    }
    
    int deg = map(p, 0, 180, 1000, 2000);
    myservo.writeMicroseconds(deg);
    delay(10);                       
  }

  digitalWrite(LED, LOW);
  Serial.println("Door Closed.");
}
