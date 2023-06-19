#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

#define BEEP 17
 
void setup(){
  mySerial.begin(9600);
  Serial.begin(9600);
  
  pinMode(BEEP, OUTPUT);
  digitalWrite(BEEP, LOW);

  Serial.println("Alarm:");
}

const char snoozeKey = '*';
const char stopKey = '#';
const int snoozeTime = 5;

int timerDuration = -1;
String input = "";
long startTime = -1;
bool alarmON = false;

const char stopchar = '.';

void loop(){
  if (mySerial.available()) {
    char inByte = mySerial.read();
    
    if((startTime > 0 && inByte == stopKey) && alarmON){
      noTone(BEEP);
      startTime = -1;
      timerDuration = -1;  
      alarmON = false;
      
      Serial.println("Alarm Stoped.");
      
    } else if((startTime > 0 && inByte == snoozeKey) && alarmON){
      noTone(BEEP);
      startTime = millis();
      timerDuration = snoozeTime;
      alarmON = false;

      Serial.println("Alarm Snoozed...");
      
    } else if(startTime < 0 && inByte != stopchar){
      input += inByte;
      
    } else if(startTime < 0 && inByte == stopchar){
      startTime = millis();
      timerDuration = input.toInt();
      input = "";
      
      Serial.print("Timer is set to ");
      Serial.print(timerDuration);
      Serial.println(" seconds.");
    }
  }

  if((startTime > 0 && !alarmON) && (millis() - startTime)/1000 >= timerDuration){
    Serial.println("Alarm is Ringing...!");
    tone(BEEP, 1000);
    alarmON = true;
  
  } else if(startTime > 0 && !alarmON){
    Serial.print("Timer: ");
    Serial.print((millis() - startTime)/1000.0);
    Serial.println(" s");
  }
}
