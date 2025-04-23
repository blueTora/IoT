#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME "Blynk Personal Smart Parking"
#define BLYNK_AUTH_TOKEN ""

#include <BlynkSimpleStream.h>
#include <Keypad.h>
#include <Servo.h>

#define BLYNK_PRINT Serial1

BlynkTimer timer;
Servo myservo;

#define buzPin 51
#define TrigPin 22
#define EchoPin 23
#define GreenLED 4

const byte ROWS = 4;
const byte COLS = 3; 
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {41, 42, 43, 44};
byte colPins[COLS] = {36, 35, 34}; 

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

const char auth[] = BLYNK_AUTH_TOKEN;
const int parkedDist = 100; //1 meter distance
const char pass[] = "2024";

bool SECURE = false;
bool PARKED = false;
bool DANGERLED = false;

BLYNK_WRITE(V0)
{
  int pinValue = param.asInt();
  
  if (pinValue == 1) {
    BLYNK_PRINT.println("Parking's Security is ON!");
    SECURE = true;

  } else {
    BLYNK_PRINT.println("Parking's Security is OFF.");
    SECURE = false;
  }
}

void myTimerEvent(){
  Blynk.virtualWrite(V1, PARKED);

  Blynk.virtualWrite(V2, DANGERLED);
}

void setup() {
  BLYNK_PRINT.begin(115200);

  BLYNK_PRINT.println("Connecting...");
  Serial.begin(9600);
  Blynk.begin(Serial, auth);
  BLYNK_PRINT.println("Connected.");

  timer.setInterval(1000L, myTimerEvent);

  myservo.attach(13);
  myservo.writeMicroseconds(1000);
  
  pinMode(buzPin, OUTPUT);
  noTone(buzPin);

  pinMode(GreenLED, OUTPUT);
  digitalWrite(GreenLED, LOW);

  pinMode(TrigPin, OUTPUT);  
  pinMode(EchoPin, INPUT);

  pinMode(41, OUTPUT);
  digitalWrite(41, HIGH);
  pinMode(42, OUTPUT);
  digitalWrite(42, HIGH);
  pinMode(43, OUTPUT);
  digitalWrite(43, HIGH);
  pinMode(44, OUTPUT);
  digitalWrite(44, HIGH);

  pinMode(34, INPUT_PULLUP);
  pinMode(35, INPUT_PULLUP);
  pinMode(36, INPUT_PULLUP);
}

void loop(){
  Blynk.run();
  timer.run(); // Initiates BlynkTimer

  char key = keypad.getKey();
  if (key) {
    if (checkPass(key)){
      noTone(buzPin);
      DANGERLED = false;

      if (!SECURE) {
        openDoor();
      } else {
        BLYNK_PRINT.println("Parking's Security is ON!");
        BLYNK_PRINT.println("Cannot Open the Door!");
      }

    } else {
      DANGERLED = true;
      tone(buzPin, 1000);
    }
  }

  long dist = calDist();
  if (dist < parkedDist) {
    if (!PARKED) {
      BLYNK_PRINT.print("The Car Distance is (cm): ");
      BLYNK_PRINT.println(dist);

      digitalWrite(GreenLED, HIGH);
      PARKED = true;
      
      BLYNK_PRINT.println("The Car is Parked.");
      
      closeDoor();
    }
  } else {
    digitalWrite(GreenLED, LOW);
    PARKED = false;
  }
}

bool checkPass(char key){
  char p[4];
  p[0] = key;

  BLYNK_PRINT.print("Key Entered is: ");
  BLYNK_PRINT.print(key);
  
  for (int i = 1; i < 4 ; i++){
    key = keypad.waitForKey();

    BLYNK_PRINT.print(key);
    p[i] = key;
  }
  
  BLYNK_PRINT.println();

  return strcmp(pass, p) == 0;
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

void openDoor(){
  BLYNK_PRINT.println("Door Opening...");
  
  for (int p = 0; p <= 90; p += 1) {
    int deg = map(p, 0, 180, 1000, 2000);
    myservo.writeMicroseconds(deg);
    delay(10);                       
  }
  
  BLYNK_PRINT.println("Door Opened.");
}

void closeDoor(){
  BLYNK_PRINT.println("Door Closing...");
  
  for (int p = 90; p >= 0; p -= 2) {    
    int deg = map(p, 0, 180, 1000, 2000);
    myservo.writeMicroseconds(deg);
    delay(10);                       
  }

  BLYNK_PRINT.println("Door Closed.");
}
