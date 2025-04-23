#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME "Air Pollution Monitoring System"
#define BLYNK_AUTH_TOKEN ""

#include <BlynkSimpleStream.h>

#define BLYNK_PRINT Serial1

BlynkTimer timer;

#define gasPin A0
#define fanPin 7
#define redLED 13

const char auth[] = BLYNK_AUTH_TOKEN;
const int pollutionLevel = 250;
const int threshold = 16;
const int maxTime = 5000; // 5 seconds

bool MONITOR = false;
bool DANGER = false;
bool DANGERLED = true;

int gasValue = 0;
long dangerStartTime = -1;
long safeStartTime = -1;

BLYNK_WRITE(V0)
{
  int pinValue = param.asInt();
  
  if (pinValue == 1) {
    BLYNK_PRINT.println("Air Pollution Monitoring is ON!");
    MONITOR = true;

  } else {
    BLYNK_PRINT.println("Air Pollution Monitoring is OFF.");
    MONITOR = false;
  }
}

void myTimerEvent(){
  Blynk.virtualWrite(V1, gasValue);

  BLYNK_PRINT.print("Gas Value: ");
  BLYNK_PRINT.println(gasValue);

  if (DANGER) {
    Blynk.virtualWrite(V2, DANGERLED);

    if(DANGERLED) {
      digitalWrite(redLED, HIGH);
    } else {
      digitalWrite(redLED, LOW);
    }

    DANGERLED = !DANGERLED;
  } else {
    Blynk.virtualWrite(V2, 0);
  }
}

void setup() {
  BLYNK_PRINT.begin(115200);

  BLYNK_PRINT.println("Connecting...");
  Serial.begin(9600);
  Blynk.begin(Serial, auth);
  BLYNK_PRINT.println("Connected.");

  timer.setInterval(200L, myTimerEvent);

  pinMode(gasPin, INPUT);

  pinMode(fanPin, OUTPUT);
  digitalWrite(fanPin, LOW);

  pinMode(redLED, OUTPUT);
  digitalWrite(redLED, LOW);
}

void loop() {
  Blynk.run();
  timer.run(); // Initiates BlynkTimer

  if (MONITOR){
    gasValue = analogRead(gasPin);
  
    if (gasValue > pollutionLevel){

      if (dangerStartTime < 0) {
        dangerStartTime = millis();
        safeStartTime = -1;
        DANGER = true;

        BLYNK_PRINT.println("Air is Polluted!");

      } else if (millis() - dangerStartTime > maxTime) {
        digitalWrite(fanPin, HIGH);
      }

    } else if (gasValue < pollutionLevel - threshold) {
      digitalWrite(redLED, LOW);
      dangerStartTime = -1;
      DANGER = false;
      DANGERLED = true;

      if (safeStartTime < 0) {
        safeStartTime = millis();

      } else if (millis() - safeStartTime > maxTime) {
        digitalWrite(fanPin, LOW);
        safeStartTime = -1;
      } 
    }

  } else {
    gasValue = 0;
  }
}
