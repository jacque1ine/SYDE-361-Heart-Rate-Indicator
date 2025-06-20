#include <LiquidCrystal.h>
// 8,9,4,5,6,7
// analogRead A0 to find voltage ranges for the buttons
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
int buttonVoltage = 0;
bool buttonState = LOW;

void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  lcd.begin(16, 2);
  Serial.begin(9600);
  lcd.print("Print statement 1");
  delay(2000);
  lcd.clear();
  lcd.print("Test LCD clear");
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonVoltage = analogRead(A0);
  // right is 0
  // down is 255
  // up is 99
  // left is 407
  // select is 637 
  
  if (buttonVoltage < 700) {
    Serial.print("Button Voltage: ");
    Serial.println(buttonVoltage);
  }
}
