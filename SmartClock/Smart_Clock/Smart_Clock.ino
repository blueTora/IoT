#include <LiquidCrystal.h>

const int rs = 24, en = 27, d4 = 32, d5 = 33, d6 = 34, d7 = 35;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int btnPin = 40;
const int buzPin = 51;
const int ldrPin = A14;
const int lightThreshold = 500;

void setup() {
  pinMode(btnPin, INPUT_PULLUP);
  pinMode(buzPin, OUTPUT);
  pinMode(ldrPin, INPUT);

  noTone(buzPin);

  lcd.begin(16, 1);
  lcd.clear();
}

int counter = 0;
bool sleep = true;

void loop() {
  int ldrStatus = analogRead(ldrPin);

  if (ldrStatus >= lightThreshold){
    if(sleep){
      tone(buzPin, 1000);
  
      while (true) {
        lcd.clear();
        lcd.print(counter);
  
        if (digitalRead(btnPin) == LOW) {
          noTone(buzPin);
          sleep = false;
          break;
        }
        
        delay(1000);
        counter = counter + 1;
      }  
    }
    
  } else if(!sleep){
    sleep = true;
    counter = 0;
    lcd.clear();
  }
}
