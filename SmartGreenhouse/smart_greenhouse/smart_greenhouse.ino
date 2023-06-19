#include <Keypad.h>
#include <Servo.h>

#define BEEP 26

#define flameS1 7
#define flameS2 12
#define flameS3 A15

#define soilS1 A0
#define soilS2 A1
#define soilS3 A2

#define BUTTON 31

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

Servo myservo1;
Servo myservo2;
Servo myservo3;

void setup() {
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

  pinMode(flameS1, INPUT);
  pinMode(flameS2, INPUT);
  pinMode(flameS3, INPUT);

  pinMode(soilS1, INPUT_PULLUP);
  pinMode(soilS2, INPUT_PULLUP);
  pinMode(soilS3, INPUT_PULLUP);

  pinMode(BEEP, OUTPUT);
  digitalWrite(BEEP, LOW);

  myservo1.attach(22);
  myservo1.writeMicroseconds(1000);
  myservo2.attach(13);
  myservo2.writeMicroseconds(1000);
  myservo3.attach(53);
  myservo3.writeMicroseconds(1000);

  pinMode(BUTTON, INPUT);
}

void loop() {

  int button = digitalRead(BUTTON);
    
  if(button == HIGH){
    char key = keypad.getKey();
    
    if (key != NO_KEY){
      if(key == '1'){
        char op = keypad.waitForKey();
        
        if(op == '*')
          myservo1.writeMicroseconds(2000);
        else if(op == '#')
          myservo1.writeMicroseconds(1000);
        
      } else if(key == '2'){
        char op = keypad.waitForKey();
        
        if(op == '*')
          myservo2.writeMicroseconds(2000);
        else if(op == '#')
          myservo2.writeMicroseconds(1000);
        
      } else if(key == '3'){
        char op = keypad.waitForKey();
        
        if(op == '*')
          myservo3.writeMicroseconds(2000);
        else if(op == '#')
          myservo3.writeMicroseconds(1000);
      }
    }
  } else {
    checkSoilSensor();
  }
  checkFlameSensor();

  delay(300);
}

void checkSoilSensor(){
  int s1 = analogRead(soilS1);
  int deg1 = map(s1, 0, 1023, 1000, 2000);
  myservo1.writeMicroseconds(deg1);

  int s2 = analogRead(soilS2);
  int deg2 = map(s2, 0, 1023, 1000, 2000);
  myservo2.writeMicroseconds(deg2);

  int s3 = analogRead(soilS3);
  int deg3 = map(s3, 0, 1023, 1000, 2000);
  myservo3.writeMicroseconds(deg3);
}

void checkFlameSensor(){
  int s1 = digitalRead(flameS1);
  if(s1 == HIGH){
    tone(BEEP, 1000);
    myservo1.writeMicroseconds(2000);
    
    delay(1000);
    
    noTone(BEEP);
    myservo1.writeMicroseconds(1000);
  }

  int s2 = digitalRead(flameS2);
  if(s2 == HIGH){
    tone(BEEP, 1000);
    myservo2.writeMicroseconds(2000);
    
    delay(1000);
    
    noTone(BEEP);
    myservo2.writeMicroseconds(1000);
  }

  int s3 = digitalRead(flameS3);
  if(s3 == HIGH){
    tone(BEEP, 1000);
    myservo3.writeMicroseconds(2000);
    
    delay(1000);
    
    noTone(BEEP);
    myservo3.writeMicroseconds(1000);
  }
}
