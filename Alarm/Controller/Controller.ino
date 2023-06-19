#include <SoftwareSerial.h>
#include <Keypad.h>

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

SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

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
  
  Serial.println("Controller:");
}

void loop() {
  char key = keypad.getKey();
  
  if (key){
    mySerial.print(key);
  }

  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}
