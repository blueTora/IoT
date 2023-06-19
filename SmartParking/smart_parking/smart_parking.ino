#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>

#define RS_PIN 28
#define EN_PIN 29
#define D4_PIN 30
#define D5_PIN 31
#define D6_PIN 32
#define D7_PIN 33

#define PIRpin1 21
#define PIRpin2 19
#define PIRpin3 17
#define PIRpin4 0
#define PIRpin5 8
#define PIRpin6 13
#define PIRpin7 A0
#define PIRpin8 A2
#define PIRpin9 A14

#define LED1 20
#define LED2 18
#define LED3 16
#define LED4 1
#define LED5 9
#define LED6 12
#define LED7 A1
#define LED8 A3
#define LED9 A15

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
  lcd.begin(16, 4);
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

  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, LOW);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED2, LOW);
  pinMode(LED3, OUTPUT);
  digitalWrite(LED3, LOW);
  pinMode(LED4, OUTPUT);
  digitalWrite(LED4, LOW);
  pinMode(LED5, OUTPUT);
  digitalWrite(LED5, LOW);
  pinMode(LED6, OUTPUT);
  digitalWrite(LED6, LOW);
  pinMode(LED7, OUTPUT);
  digitalWrite(LED7, LOW);
  pinMode(LED8, OUTPUT);
  digitalWrite(LED8, LOW);
  pinMode(LED9, OUTPUT);
  digitalWrite(LED9, LOW);

  myservo.attach(23);
  myservo.writeMicroseconds(1000);

  pinMode(PIRpin1, INPUT);
  pinMode(PIRpin2, INPUT);
  pinMode(PIRpin3, INPUT);
  pinMode(PIRpin4, INPUT);
  pinMode(PIRpin5, INPUT);
  pinMode(PIRpin6, INPUT);
  pinMode(PIRpin7, INPUT);
  pinMode(PIRpin8, INPUT);
  pinMode(PIRpin9, INPUT);
}

String ids[] = {"26", "30"};
String passwords[] = {"1234", "5678"};
int const NUM = 2;
unsigned long enterTime[NUM]={0, 0};

int A = 3; //parks: 1-2-3 -> blue led
int B = 3; //parks: 4-5-6 -> yellow led
int C = 3; //parks: 7-8-9 -> red led
int t = 9;

int preState[] = {0,0,0,0,0,0,0,0,0};

int const costPerMin = 3;

void loop(){  
  displayParks();
  checkParks();
  
  char key = keypad.getKey();
  
  if (key){
    
    if(key == '*'){ // for entering
      if(t>0){
        lcd.setCursor(0, 2);
        lcd.print("  Entering...   ");
        delay(200);
        
        int res = checkPassword();

        while(res == -1){
          res = checkPassword();
        }
  
        openDoor();
        lcd.clear();
        
      } else {
        lcd.setCursor(0, 2);
        lcd.print("Parking is Full!");
        lcd.setCursor(0, 3);
        lcd.print("You Can't Enter.");
        delay(500);
        
        lcd.clear();
        displayParks();
      }
      
    } else if (key == '#'){ // for exiting
      lcd.setCursor(0, 2);
      lcd.print("   Exiting...   ");
      delay(200);
      
      int res = checkPassword();

      while(res == -1){
        res = checkPassword();
      }

      float duration = (millis() - enterTime[res])/1000.0;
      float pay = duration/60.0 * costPerMin;
      enterTime[res] = 0;

      lcd.setCursor(0, 2);
      lcd.print("Parking Cost:   ");
      lcd.setCursor(0, 3);
      lcd.print("                ");
      lcd.setCursor(0, 3);
      lcd.print(pay);
      lcd.print(" $");
      delay(1500);

      openDoor();
      lcd.clear();
    }
  }
}

int checkPassword(){//press '#' at the end of entering password and id
  lcd.clear();
  displayParks();
  lcd.setCursor(0, 2);
  lcd.print("ID:");

  String id = "";
  char i = keypad.waitForKey();
  while(i != '#'){
    lcd.print(i);
    id += i;
    i = keypad.waitForKey();
  }
  
  lcd.setCursor(0, 3);
  lcd.print("Pass:");

  String pass = "";
  char j = keypad.waitForKey();
  while(j != '#'){
    lcd.print("*");
    pass += j;
    j = keypad.waitForKey();
  }

  lcd.setCursor(0, 2);
  lcd.print("                ");

  for(int x=0; x<NUM; x++){
    if(id.equals(ids[x])){
      if(pass.equals(passwords[x])){
        lcd.setCursor(0, 3);
        lcd.print("Correct Password");
        delay(300);

        if(enterTime[x] == 0){
          enterTime[x] = millis();
        }
        
        return x;
      } else {
        lcd.setCursor(0, 3);
        lcd.print("Wrong Password!!");
        delay(300);
        return -1;
      }
    }
  }

  lcd.setCursor(0, 3);
  lcd.print("   Wrong ID!!   ");
  delay(300);
  return -1;
}

void openDoor(){
  lcd.clear();
  displayParks();
  lcd.setCursor(0, 3);
  lcd.print("Door Opening... ");
  
  for (int p = 0; p <= 180; p += 1) {
    int deg = map(p, 0, 180, 1000, 2000);
    myservo.writeMicroseconds(deg);
    delay(5);                       
  }

  lcd.setCursor(0, 3);
  lcd.print("Door Open.      ");
  delay(1000);
  lcd.setCursor(0, 3);
  lcd.print("Door Closing... ");
  
  for (int p = 180; p >= 0; p -= 1) {
    int deg = map(p, 0, 180, 1000, 2000);
    myservo.writeMicroseconds(deg);
    delay(5);                       
  }

  lcd.setCursor(0, 3);
  lcd.print("Door closed.    ");
  delay(200);
}

void displayParks(){
  lcd.setCursor(0,0);
  lcd.print("A:");
  lcd.print(A);
  lcd.print("    ");
  lcd.print("B:");
  lcd.print(B);

  lcd.setCursor(0,1);
  lcd.print("C:");
  lcd.print(C);
  lcd.print("    ");
  lcd.print("t:");
  lcd.print(t);
}

void checkParks(){
  int pir1 = digitalRead(PIRpin1); 
  if (pir1 == HIGH) {
  
   if(preState[0] == 0){
    digitalWrite(LED1, HIGH);
    preState[0] = 1;
    A--;
    t--;
   }
   
  } else {
    
   if(preState[0] == 1){
    digitalWrite(LED1, LOW);
    preState[0] = 0;
    A++;
    t++;
   }
  }

  int pir2 = digitalRead(PIRpin2); 
  if (pir2 == HIGH) {
  
   if(preState[1] == 0){
    digitalWrite(LED2, HIGH);
    preState[1] = 1;
    A--;
    t--;
   }
   
  } else {
    
   if(preState[1] == 1){
    digitalWrite(LED2, LOW);
    preState[1] = 0;
    A++;
    t++;
   }
  }

  int pir3 = digitalRead(PIRpin3); 
  if (pir3 == HIGH) {
  
   if(preState[2] == 0){
    digitalWrite(LED3, HIGH);
    preState[2] = 1;
    A--;
    t--;
   }
   
  } else {
    
   if(preState[2] == 1){
    digitalWrite(LED3, LOW);
    preState[2] = 0;
    A++;
    t++;
   }
  }

  int pir4 = digitalRead(PIRpin4); 
  if (pir4 == HIGH) {
  
   if(preState[3] == 0){
    digitalWrite(LED4, HIGH);
    preState[3] = 1;
    B--;
    t--;
   }
   
  } else {
    
   if(preState[3] == 1){
    digitalWrite(LED4, LOW);
    preState[3] = 0;
    B++;
    t++;
   }
  }

  int pir5 = digitalRead(PIRpin5); 
  if (pir5 == HIGH) {
  
   if(preState[4] == 0){
    digitalWrite(LED5, HIGH);
    preState[4] = 1;
    B--;
    t--;
   }
   
  } else {
    
   if(preState[4] == 1){
    digitalWrite(LED5, LOW);
    preState[4] = 0;
    B++;
    t++;
   }
  }

  int pir6 = digitalRead(PIRpin6); 
  if (pir6 == HIGH) {
  
   if(preState[5] == 0){
    digitalWrite(LED6, HIGH);
    preState[5] = 1;
    B--;
    t--;
   }
   
  } else {
    
   if(preState[5] == 1){
    digitalWrite(LED6, LOW);
    preState[5] = 0;
    B++;
    t++;
   }
  }

  int pir7 = digitalRead(PIRpin7); 
  if (pir7 == HIGH) {
   
   if(preState[6] == 0){
    digitalWrite(LED7, HIGH);
    preState[6] = 1;
    C--;
    t--;
   }
   
  } else {
   if(preState[6] == 1){
    digitalWrite(LED7, LOW);
    preState[6] = 0;
    C++;
    t++;
   }
  }

  int pir8 = digitalRead(PIRpin8); 
  if (pir8 == HIGH) {
   
   if(preState[7] == 0){
    digitalWrite(LED8, HIGH);
    preState[7] = 1;
    C--;
    t--;
   }
   
  } else {
   if(preState[7] == 1){
    digitalWrite(LED8, LOW);
    preState[7] = 0;
    C++;
    t++;
   }
  }

  int pir9 = digitalRead(PIRpin9); 
  if (pir9 == HIGH) {
   
   if(preState[8] == 0){
    digitalWrite(LED9, HIGH);
    preState[8] = 1;
    C--;
    t--;
   }
   
  } else {
   if(preState[8] == 1){
    digitalWrite(LED9, LOW);
    preState[8] = 0;
    C++;
    t++;
   }
  }
}
