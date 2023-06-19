#include <BlynkSimpleStream.h>
#include <Servo.h>

#define BLYNK_PRINT Serial1

#define BLYNK_TEMPLATE_ID "TMPLNoqI1HQL"
#define BLYNK_DEVICE_NAME "Question9 smart home"
#define BLYNK_AUTH_TOKEN "KZoLAP_5FgpQK0C24xAzeO8WG6Fy4-69"

char auth[] = BLYNK_AUTH_TOKEN;

#define tempPin A0
#define LED 21

BlynkTimer timer;
Servo myservo;

int desiredTemp = 0;
int currentTemp = 0;
const int threshold = 5;

BLYNK_WRITE(V0){
  int pinValue = param.asInt();
  Serial1.print("Slider's value is: ");
  Serial1.println(pinValue);
  desiredTemp = pinValue;
}

void myTimerEvent(){
  Blynk.virtualWrite(V1, currentTemp);
}

void setup(){
  Serial1.begin(115200);

  Serial.begin(9600);
  Blynk.begin(Serial, auth);

  timer.setInterval(1000L, myTimerEvent);
  
  myservo.attach(13);
  myservo.writeMicroseconds(1393);// default value = 25 degree C

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  pinMode(tempPin, INPUT);
}

void loop(){
  Blynk.run();
  setServo(desiredTemp);

  currentTemp = checkTemp();
  timer.run();
  
  if(abs(currentTemp - desiredTemp) > threshold){
    digitalWrite(LED, HIGH);
    
  } else {
    digitalWrite(LED, LOW);
  }
}

int checkTemp(){
 int reading = analogRead(tempPin);  
 
 float v = reading * 5.0;
 v /= 1024.0; 
 
 float temperatureC = (v - 0.5) * 100 ;
 int temp = int(temperatureC);
 
 return temp;
}

void setServo(int goal){
  
  int deg = map(goal, -40, 125, 1000, 2000);
  myservo.writeMicroseconds(deg);
}
