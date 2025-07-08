#define heartratePin A1
#include "DFRobot_Heartrate.h"
DFRobot_Heartrate heartrate(DIGITAL_MODE);

#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
#define BACKLIGHT_PIN 10

// Pin Assignment
// arduino pin, shield pin
//2 (), 3, 12, 13
const int buzzerPin = 3;
const int redPin= 2;
const int greenPin = 12;
const int bluePin = 13;
const int resetButtonPin = 2; // NEED TO CHANGE THIS IS WRONG

// TO DO: remove (consts for testing)
int userAge;
int restingHR;

int prevZone = 0;

void setup()
{
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  
  lcd.begin(16, 2);
  pinMode(BACKLIGHT_PIN, OUTPUT);
  digitalWrite(BACKLIGHT_PIN, HIGH); // or LOW, as needed for your LCD

  // Prompt user for age and resting HR on startup
  setAge(userAge);
  lcd.clear();
  lcd.print(prompts[1]);
  delay(1000);

  setRhr(restingHR);
  lcd.clear();
  lcd.print(prompts[3]);
  delay(1000);

  lcd.clear();
  lcd.print(prompts[4]); // "Hit R to start, RST to reset"
  delay(1000);

  // Turn off LCD/backlight if you wish, or leave prompt
  lcd.clear();
  digitalWrite(BACKLIGHT_PIN, LOW); // optional

  Serial.begin(115200);

}

void loop()
{
  if (digitalRead(resetButtonPin) == LOW) {
    promptUserReset();
    // When done, continue to main loop logic with updated values
  }
    // { // replace this with:   // if (millis() - lastSample >= 5000) {  --> Every 5 s, sample heart‑rate and update outputs
  int hr = readFromHeartRateSensor();
  int zone = getCurrentZone(hr);
  if (zone != prevZone and zone != 0)
  {
    setZoneColour(zone);
    buzz();
  }

  // setLCDDisplay(zone, hr);

  //  lastSample = millis();

  prevZone = zone;
}

/* ---------- Zone Calculation ---------- */
int getMaxHeartRate(int age)
{
  return (int)(208 - 0.7 * age);
}

int readFromHeartRateSensor(){
  uint8_t rateValue;
  heartrate.getValue(heartratePin); ///< A1 foot sampled values
  rateValue = heartrate.getRate();  ///< Get heart rate value
  if (rateValue)
  {
    Serial.println(rateValue);
  }
  delay(20);

  return int(rateValue);
}

int getCurrentZone(int heartRate){
  int maxHR = getMaxHeartRate(userAge);
  int HRR = maxHR - restingHR;

  float zones[][2] = {
      {0.50, 0.60}, // Zone 1
      {0.60, 0.70}, // Zone 2
      {0.70, 0.80}, // Zone 3
      {0.80, 0.90}, // Zone 4
      {0.90, 1.00}  // Zone 5
  };

  for (int i = 0; i < 5; i++)
  {
    int lowerBound = restingHR + HRR * zones[i][0];
    int upperBound = restingHR + HRR * zones[i][1];

    if (heartRate >= lowerBound && heartRate < upperBound)
    {
      return i + 1; // zones are 1-indexed
    }
  }

  return 0; // No zone matched
}

/* ---------- Zone Colours ---------- */

void setZoneColour(int zone)
{
  switch (zone)
  {
  case 1:
    setColor(170, 0, 255); // Yellow
    break;
  case 2:
    setColor(0, 0, 255); // Green
    break;
  case 3:
    setColor(0, 255, 0); // Blue
    break;
  case 4:
    setColor(255, 155, 0); // Purple
    break;
  case 5:
    setColor(255, 0, 0); // Red
    break;
    // default:
    //   setColor(255, 255, 255); // White
    //   break;
  }
}

void setColor(int redValue, int greenValue, int blueValue)
{
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}

/* ---------- Buzz and LCD Display ---------- */

void buzz(){
  digitalWrite(buzzerPin, HIGH); // Turn buzzer ON
  delay(500);                    // Wait for 0.5 seconds
  digitalWrite(buzzerPin, LOW);  // Turn buzzer OFF
}

void setLEDDisplay(int zone, int hr) { // TODO @tali @olivia
  lcd.clear();
  lcd.print("Zone: ");
  lcd.print(zone);
  lcd.print("  HR: ");
  lcd.print(hr);
}

void promptUserReset() {
    // Debounce if you want, e.g. delay(20) and check still LOW
  lcd.clear();
  lcd.print("Resetting...");
  delay(500);

  digitalWrite(BACKLIGHT_PIN, HIGH); // Turn backlight on
  setAge(userAge);
  lcd.clear();
  lcd.print(prompts[1]); // "Age set!"
  delay(1000);

  setRhr(restingHR);
  lcd.clear();
  lcd.print(prompts[3]); // "RHR set!"
  delay(1000);

  lcd.clear();
  lcd.print(prompts[4]); // "Hit R to start, RST to reset"
  delay(1000);
  lcd.clear();
  digitalWrite(BACKLIGHT_PIN, LOW); // Optional: turn off after
}

int setAge(int currentAge) {
  int selectedAge = currentAge;
  while (true) {
    int buttonVoltage = analogRead(A0);
    if (buttonVoltage > 70 && buttonVoltage < 120) {
      selectedAge++;
      delay(200);
    } else if (buttonVoltage > 230 && buttonVoltage < 270) {
      selectedAge--;
      delay(200);
    } else if (buttonVoltage > 600 && buttonVoltage < 650) {
      userAge = selectedAge;
      return selectedAge;
    }
    if (selectedAge < 0) selectedAge = 0;
    if (selectedAge > 120) selectedAge = 120;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(prompts[0]);
    lcd.setCursor(0, 1);
    lcd.print(selectedAge);
    delay(50);
  }
}

int setRhr(int currentRhr) {
  int selectedRhr = currentRhr;
  while (true) {
    int buttonVoltage = analogRead(A0);
    if (buttonVoltage > 70 && buttonVoltage < 120) {
      selectedRhr++;
      delay(200);
    } else if (buttonVoltage > 230 && buttonVoltage < 270) {
      selectedRhr--;
      delay(200);
    } else if (buttonVoltage > 600 && buttonVoltage < 650) {
      restingHR = selectedRhr;
      return selectedRhr;
    }
    if (selectedRhr < 30) selectedRhr = 30;
    if (selectedRhr > 120) selectedRhr = 120;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(prompts[2]);
    lcd.setCursor(0, 1);
    lcd.print(selectedRhr);
    delay(50);
  }
}

