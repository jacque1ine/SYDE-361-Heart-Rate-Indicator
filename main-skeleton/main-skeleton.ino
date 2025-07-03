// LED Pin Assignment 
const int redPin    = 13;
const int greenPin  = 12;
const int bluePin   = 11;

// TO DO: remove (consts for testing)
int userAge = 25;
int restingHR = 60;


// /* ---------- Global state ---------- */
// int           prevZone    = 0;
// unsigned long lastSample  = 0;          // ms timestamp of last HR read


void setup() {
  // TODO: initialize LCD, RGB LED pins, buzzer pin, buttons, HR sensor

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  Serial.begin(9600);
  Serial.println("Enter heart rate number:");
}

void loop() {
    if (Serial.available() > 0) {
        int hr = readFromHeartRateSensor();
        int zone = getCurrentZone(hr);

        getZoneColour(zone);


      Serial.println("Enter heart rate:");
    }
  /* Every 5 s, sample heart‑rate and update outputs */
  // if (millis() - lastSample >= 5000) {
  //   int heartRate = readFromHeartRateSensor();
  //   int currZone  = getCurrentZone(heartRate);

  //   if (currZone != prevZone) {
  //     // TODO: buzz to indicate zone change
  //     setLEDColour(currZone);
  //   }

  //   // TODO: display heartRate & currZone on LCD

  //   prevZone   = currZone;
  //   lastSample = millis();
  // }

    
}

/* ---------- Helper function stubs ---------- */
int getMaxHeartRate(int age) {
  return (int)(208 - 0.7 * age);
}

int readFromHeartRateSensor() {
  // TODO: Replace this with actual sensor reading logic
  return Serial.parseInt();
}

// Calculate HRR and determine current zone using Karvonen
int getCurrentZone(int heartRate) {
  int maxHR = getMaxHeartRate(userAge);
  int HRR = maxHR - restingHR;

  float zones[][2] = {
    {0.50, 0.60}, // Zone 1
    {0.60, 0.70}, // Zone 2
    {0.70, 0.80}, // Zone 3
    {0.80, 0.90}, // Zone 4
    {0.90, 1.00}  // Zone 5
  };

  for (int i = 0; i < 5; i++) {
    int lowerBound = restingHR + HRR * zones[i][0];
    int upperBound = restingHR + HRR * zones[i][1];

    if (heartRate >= lowerBound && heartRate < upperBound) {
      return i + 1; // zones are 1-indexed
    }
  }

  return 0; // No zone matched
}

void getZoneColour(int zone) {
  switch (zone) {
    case 1:
      setColor(170, 0, 255); // Yellow
      break;
    case 2:
      setColor(0, 0, 255); // Green
      break;
    case 3: 
      setColor(0,  255, 0); // Blue
      break;
    case 4: 
      setColor(255, 155 , 0);// Purple
      break;
    case 5: 
      setColor(255, 0, 0); // Red 
      break;
    // default: 
    //   setColor(255, 255, 255); // White
    //   break; 
  }
}

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}
