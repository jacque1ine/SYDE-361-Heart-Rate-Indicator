#include <LiquidCrystal.h>
#include <string.h>

// analogRead A0 to find voltage ranges for the buttons
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
#define BACKLIGHT_PIN 10
int buttonVoltage = 0;
bool buttonState = LOW;
String prompts[] = {"Select your age: ", "Age set!", "Select your RHR", "RHR set!", "Hit R to start, RST to reset"};
int age = 20;
int rhr = 65;

int lastButton = -1; // To track last button state

int setAge(int currentAge) {
  int selectedAge = currentAge;

  while (true) {
    int buttonVoltage = analogRead(A0);

    if (buttonVoltage > 70 && buttonVoltage < 120) {
      // Up
      selectedAge++;
      delay(200); // Debounce
    } else if (buttonVoltage > 230 && buttonVoltage < 270) {
      // Down
      selectedAge--;
      delay(200);
    } else if (buttonVoltage > 600 && buttonVoltage < 650) {
      // Select
      age = selectedAge;
      return selectedAge;
    }

    // Optional: constrain age range
    if (selectedAge < 0) selectedAge = 0;
    if (selectedAge > 120) selectedAge = 120;

    // Display current age
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(prompts[0]);
    lcd.setCursor(0, 1);
    lcd.print(selectedAge);

    delay(50); // Small delay to reduce LCD flicker
  }
}


int setRhr(int currentRhr) {
  int selectedRhr = currentRhr;

  while (true) {
    int buttonVoltage = analogRead(A0);

    if (buttonVoltage > 70 && buttonVoltage < 120) {
      // Up
      selectedRhr++;
      delay(200); // Debounce
    } else if (buttonVoltage > 230 && buttonVoltage < 270) {
      // Down
      selectedRhr--;
      delay(200);
    } else if (buttonVoltage > 600 && buttonVoltage < 650) {
      // Select
      rhr = selectedRhr;
      return selectedRhr;
    }

    // Optional: constrain rhr range
    if (selectedRhr < 30) selectedRhr = 30;
    if (selectedRhr > 120) selectedRhr = 120;

    // Display current rhr
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(prompts[2]);
    lcd.setCursor(0, 1);
    lcd.print(selectedRhr);

    delay(50); // Small delay to reduce LCD flicker
  }
}


void setup() {
  // code runs once on reset
  pinMode(A0, INPUT);
  pinMode(BACKLIGHT_PIN, OUTPUT);
  digitalWrite(BACKLIGHT_PIN, HIGH); // optional, turns the backlight off to save power (can later assign a button in settings to toggle this)
  lcd.begin(16, 2);
  Serial.begin(9600);
  setAge(age);
  Serial.print(age); // annie's code will directly access global age variable 
  delay(1000);
  lcd.clear();
  lcd.print(prompts[1]);

  delay(1000);
  setRhr(rhr);
  Serial.println(rhr); // annie's code will directly access global rhr variable
  delay(1000);
  lcd.clear();
  lcd.print(prompts[3]);
}

void loop() {
  int currentButton = -1;
  int buttonVoltage = analogRead(A0);

  if (buttonVoltage < 20) {
    currentButton = 0; // Right
  } else if (buttonVoltage > 70 && buttonVoltage < 120) {
    currentButton = 1; // Up
  } else if (buttonVoltage > 230 && buttonVoltage < 270) {
    currentButton = 2; // Down
  } else if (buttonVoltage > 600 && buttonVoltage < 650) {
    currentButton = 3; // Select
  }

  if (currentButton != -1 && currentButton != lastButton) {
    switch (currentButton) {
      case 0: Serial.println("Right"); break;
      case 1: Serial.println("Up"); break;
      case 2: Serial.println("Down"); break;
      case 3: Serial.println("Select"); break;
    }
  }

  lastButton = currentButton;

  delay(50); // Small delay to debounce
}
