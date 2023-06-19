#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>
#include <TimeLib.h>

unsigned long baseTime = 1649080488;

#define RS_PIN 12
#define EN_PIN 10
#define D4_PIN 5
#define D5_PIN 4
#define D6_PIN 3
#define D7_PIN 2

#define LED 22
#define BEEP 30

LiquidCrystal lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

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

Servo myservo;

void setup() {
  Serial.begin(9600);

  lcd.begin(20, 4);
  lcd.clear();

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

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  pinMode(BEEP, OUTPUT);
  digitalWrite(BEEP, LOW);

  myservo.attach(26);
  myservo.writeMicroseconds(1000);

  displayClock();
}

String ids[] = {"00", "11", "12", "13", "14", "15", "16"};
int stdList[] = {0,0,0,0,0,0};
int const stdNum = 6;

unsigned long classStartTime = 0;
int allowedTime = 30000;

int updateTime = 60000;
unsigned long lastUpdate = 0;

void loop() {
  if (millis() - lastUpdate > updateTime){
    displayClock();
    lastUpdate = millis();
  } 

  lcd.setCursor(0, 0);
  lcd.print("Enter your ID: ");
  
  char key = keypad.getKey();
  
  if (key){
    if(key == '#'){
      lcd.clear();
      displayClock();

      lcd.setCursor(0, 0);
      lcd.print("Class Ended. :)");
      lcd.setCursor(0, 1);
      lcd.print("List of Absents");
      lcd.setCursor(0, 2);
      lcd.print("will be Printed");

      for(int i=0; i<stdNum ; i++){
        if(!stdList[i])
          Serial.println(ids[i+1]);
      }

      delay(10000);
      
    } else {
      String id = "";
      while(key != '*'){
        lcd.print(key);
        id += key;
        key = keypad.waitForKey();
      }

      lcd.setCursor(0, 1);
      
      if(id.equals(ids[0])){
        classStartTime = millis();
        lcd.print(" Professor Entered! ");
        lcd.setCursor(0, 2);
        lcd.print("  Class Started :(  ");
        delay(2000);
        openDoor();
        
      } else if (classStartTime == 0 || millis() - classStartTime < allowedTime){
        int valid = 0;
        for(int i=1 ; i<=stdNum ; i++){
          if(id.equals(ids[i])){
            stdList[i-1] = 1;
            valid = 1;
          }
        }
  
        if(valid){
          lcd.print("  You Can Enter...  ");
          openDoor();
        } else {
          lcd.print("You are'nt a Student");
        }
        delay(2000);
        
      } else if (millis() - classStartTime > allowedTime){
        unsigned long late = (millis() - classStartTime)/1000;
        lcd.print(" You Can't Enter :( ");
        lcd.setCursor(0, 2);
        lcd.print("You are ");
        lcd.print(late);
        lcd.print(" s Late");
        buzzer();
        
        delay(2000);
        lcd.clear();
        displayClock();
      }
    }
  }
}

void openDoor(){
  digitalWrite(LED, HIGH);
  
  lcd.setCursor(0, 2);
  lcd.print("Door Opening... ");
  
  for (int p = 0; p <= 180; p += 1) {
    int deg = map(p, 0, 180, 1000, 2000);
    myservo.writeMicroseconds(deg);
    delay(10);                       
  }

  lcd.setCursor(0, 2);
  lcd.print("Door Open.      ");
  
  delay(1200);
  digitalWrite(LED, LOW);
  
  lcd.setCursor(0, 2);
  lcd.print("Door Closing... ");
  
  for (int p = 180; p >= 0; p -= 1) {
    int deg = map(p, 0, 180, 1000, 2000);
    myservo.writeMicroseconds(deg);
    delay(10);                       
  }

  lcd.setCursor(0, 2);
  lcd.print("Door closed.    ");
  delay(500);

  lcd.clear();
  displayClock();
}

void buzzer(){
  tone(BEEP, 1000);
  delay(1000);
  noTone(BEEP);
}

void displayClock(){
  time_t t = baseTime + millis()/1000;
  
  lcd.setCursor(0, 3);
  lcd.print(hour(t));

  int m = minute(t);
  lcd.print(":");
  if(m < 10)
    lcd.print('0');
    
  lcd.print(m);
  lcd.print("    ");

  lcd.print(year(t));
  lcd.print("-");
  lcd.print(month(t));
  lcd.print("-");
  lcd.print(day(t)); 
}
