#define heartratePin A1
#include "DFRobot_Heartrate.h"
// This is the one where everything works

DFRobot_Heartrate heartrate(DIGITAL_MODE);

#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
#define BACKLIGHT_PIN 10

int buttonVoltage = 0;
bool buttonState = LOW;
String prompts[] = {"Select your age: ", "Age set!", "Select your RHR", "RHR set!", "Hit R to start, RST to reset"};
int age = 20;
int rhr = 65;
int lastButton = -1; // To track last button state

// Pin Assignment
const int buzzerPin = 3;
const int redPin = 2;
const int greenPin = 12;
const int bluePin = 13;

// TO DO: remove (consts for testing)
// int userAge = 100;
// int rhr = 20;

int prevZone = 0;
// int lastSample = 0;

int setAge(int currentAge)
{
  int selectedAge = currentAge;

  while (true)
  {
    int buttonVoltage = analogRead(A0);

    if (buttonVoltage > 70 && buttonVoltage < 120)
    {
      // Up
      selectedAge++;
      delay(200); // Debounce
    }
    else if (buttonVoltage > 230 && buttonVoltage < 270)
    {
      // Down
      selectedAge--;
      delay(200);
    }
    else if (buttonVoltage > 600 && buttonVoltage < 650)
    {
      // Select
      age = selectedAge;
      return selectedAge;
    }

    // Optional: constrain age range
    if (selectedAge < 0)
      selectedAge = 0;
    if (selectedAge > 120)
      selectedAge = 120;

    // Display current age
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(prompts[0]);
    lcd.setCursor(0, 1);
    lcd.print(selectedAge);

    delay(50); // Small delay to reduce LCD flicker
  }
}

int setRhr(int currentRhr)
{
  int selectedRhr = currentRhr;

  while (true)
  {
    int buttonVoltage = analogRead(A0);

    if (buttonVoltage > 70 && buttonVoltage < 120)
    {
      // Up
      selectedRhr++;
      delay(200); // Debounce
    }
    else if (buttonVoltage > 230 && buttonVoltage < 270)
    {
      // Down
      selectedRhr--;
      delay(200);
    }
    else if (buttonVoltage > 600 && buttonVoltage < 650)
    {
      // Select
      rhr = selectedRhr;
      return selectedRhr;
    }

    // Optional: constrain rhr range
    if (selectedRhr < 30)
      selectedRhr = 30;
    if (selectedRhr > 120)
      selectedRhr = 120;

    // Display current rhr
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(prompts[2]);
    lcd.setCursor(0, 1);
    lcd.print(selectedRhr);

    delay(50); // Small delay to reduce LCD flicker
  }
}

void setup()
{
  // TODO: initialize LCD, RGB LED pins, buzzer pin, buttons, HR sensor
  Serial.begin(115200);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(buzzerPin, OUTPUT);

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

  // Serial.begin(9600);
  // Serial.println("Enter heart rate number:");
  // lastSample =
}

void loop()
{
  // if (Serial.available() > 0) {
  // { // replace this with:   //
  int hr = readFromHeartRateSensor();
  int zone = getCurrentZone(hr);
  if (zone != prevZone && hr > 0) // Only buzz on real change & valid HR
  {
    setZoneColour(zone);
    buzz();
    prevZone = zone; // Only update here after successful buzz
  }

  // TODO: isplay heartRate & currZone on LCD @tali @olivia
  // setLCDDisplay()

  if (hr)
  {
    lcd.clear();
    lcd.print("Zone: ");
    lcd.print(zone);
    lcd.print("  HR: ");
    lcd.print(hr);
  }

  // Serial.println("Enter heart rate:");

  int currentButton = -1;
  int buttonVoltage = analogRead(A0);

  if (buttonVoltage < 20)
  {
    currentButton = 0; // Right
  }
  else if (buttonVoltage > 70 && buttonVoltage < 120)
  {
    currentButton = 1; // Up
  }
  else if (buttonVoltage > 230 && buttonVoltage < 270)
  {
    currentButton = 2; // Down
  }
  else if (buttonVoltage > 600 && buttonVoltage < 650)
  {
    currentButton = 3; // Select
  }

  if (currentButton != -1 && currentButton != lastButton)
  {
    switch (currentButton)
    {
    case 0:
      Serial.println("Right");
      break;
    case 1:
      Serial.println("Up");
      break;
    case 2:
      Serial.println("Down");
      break;
    case 3:
      Serial.println("Select");
      break;
    }
  }

  lastButton = currentButton;

  delay(50); // Small delay to debounce
  // }
}

/* ---------- Zone Calculation ---------- */
int getMaxHeartRate(int age)
{
  return (int)(208 - 0.7 * age);
}

// TODO: Replace this with actual sensor reading logic @tali @olivia
int readFromHeartRateSensor()
{
  // String input = Serial.readStringUntil('\n');
  // return input.toInt();
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

int getCurrentZone(int heartRate)
{
  int maxHR = getMaxHeartRate(age);
  int HRR = maxHR - rhr;

  float zones[][2] = {
      {0.50, 0.60}, // Zone 1
      {0.60, 0.70}, // Zone 2
      {0.70, 0.80}, // Zone 3
      {0.80, 0.90}, // Zone 4
      {0.90, 1.00}  // Zone 5
  };

  int zone1Lower = rhr + HRR * zones[0][0];
  int zone5Lower = rhr + HRR * zones[4][0];

  // If below Zone 1
  if (heartRate < zone1Lower)
  {
    return 0;
  }

  // Zones 1 to 4
  for (int i = 0; i < 4; i++)
  {
    int lowerBound = rhr + HRR * zones[i][0];
    int upperBound = rhr + HRR * zones[i][1];

    if (heartRate >= lowerBound && heartRate < upperBound)
    {
      return i + 1;
    }
  }

  // Zone 5: BPM >= lower bound
  if (heartRate >= zone5Lower)
  {
    return 5;
  }

  return 0; 
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
// void setLEDDisplay() // TODO @tali @olivia

// void setLEDDisplay(int zone, int hr) { // TODO @tali @olivia
//   lcd.clear();
//   lcd.print("Zone: ");
//   lcd.print(zone);
//   lcd.print("  HR: ");
//   lcd.print(hr);
// }

void buzz()
{

  digitalWrite(buzzerPin, HIGH); // Turn buzzer ON
  delay(500);                    // Wait for 0.5 seconds
  digitalWrite(buzzerPin, LOW);  // Turn buzzer OFF
}