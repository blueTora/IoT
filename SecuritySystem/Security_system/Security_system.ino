#include <BlynkSimpleStream.h>
#include <Servo.h>

#define BLYNK_TEMPLATE_ID "TMPLJkbMJEgG"
#define BLYNK_DEVICE_NAME "Question10 Security System"
#define BLYNK_AUTH_TOKEN "SoSgaPjrlpvHEBwJXbFu2OSAi_ouGNyQ"

#define BLYNK_PRINT Serial1

char auth[] = BLYNK_AUTH_TOKEN;

BlynkTimer timer;
Servo myservo;

#define LED 21
#define BEEP 22
#define TrigPin 38
#define EchoPin 36

int distance = 0;
int cameraAngle = 0;
bool ALARM = false;

BLYNK_WRITE(V1)
{
  int pinValue = param.asInt();
  Serial1.print("Camera's Angle is ");
  Serial1.print(pinValue);
  Serial1.println(" degree.");
  cameraAngle = pinValue;
}

BLYNK_WRITE(V2)
{
  if(param.asInt() == 0) {
    alarmOFF();
    
  } else {
    alarmON();
  }
}

void myTimerEvent(){
  Blynk.virtualWrite(V0, distance);

  if(ALARM)
    Blynk.virtualWrite(V2, 1);
}

void setup(){
  Serial1.begin(115200);

  Serial.begin(9600);
  Blynk.begin(Serial, auth);

  timer.setInterval(1000L, myTimerEvent);

  myservo.attach(13);
  myservo.writeMicroseconds(1500);

  pinMode(TrigPin, OUTPUT);  
  pinMode(EchoPin, INPUT);
  
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  pinMode(BEEP, OUTPUT);
  digitalWrite(BEEP, LOW);
}

const int threshold = 6;
const int objectDist = 335; //3.35 meter distance - 50% res

void loop(){
  Blynk.run();
  setServo(cameraAngle);

  distance = calDist();
  timer.run();

  if(abs(objectDist - distance) > threshold && !ALARM){
    alarmON();
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

   return dist;
}

void setServo(int goal){
  
  int deg = map(goal, -90, 90, 1000, 2000);
  myservo.writeMicroseconds(deg);
}

void alarmON(){
  Serial1.println("an object was detected.");
  Serial1.println("Security System is turned ON.");
  ALARM = true;
  
  digitalWrite(LED, HIGH);
  tone(BEEP, 1000);
}

void alarmOFF(){
  Serial1.println("Security System is turned OFF.");
  ALARM = false;
  
  digitalWrite(LED, LOW);
  noTone(BEEP);
}
