const int buzzerPin = 3;
const int bluePin = 11;
const int greenPin = 12;
const int redPin = 13;

// TO DO: remove (consts for testing)
int userAge = 100;
int restingHR = 20;

int prevZone = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Serial.println("Enter heart rate number:");
}

void loop()
{
  if (Serial.available() > 0)
  {
    Serial.println("Enter heart rate:");
    int hr = readFromHeartRateSensor();
    int zone = getCurrentZone(hr);
    if (zone != prevZone)
    {
      setZoneColour(zone);
      buzz();
    }
    prevZone = zone;
  }
}

/* ---------- Zone Calculation ---------- */
int getMaxHeartRate(int age)
{
  return (int)(208 - 0.7 * age);
}

// TODO: Replace this with actual sensor reading logic @tali @olivia
int readFromHeartRateSensor()
{
  String input = Serial.readStringUntil('\n');
  return input.toInt();
}

// Calculate HRR and determine current zone using Karvonen
int getCurrentZone(int heartRate)
{
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
// void setLEDDisplay() // TODO @tali @olivia
void buzz()
{
  digitalWrite(buzzerPin, HIGH); // Turn buzzer ON
  delay(500);                    // Wait for 0.5 seconds
  digitalWrite(buzzerPin, LOW);  // Turn buzzer OFF
}