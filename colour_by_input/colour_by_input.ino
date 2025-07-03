int redPin = 7;
int greenPin = 6;
int bluePin = 5;

// Helper function to get colour values based on zone number
void getZoneColour(int zone) {
  switch (zone) {
    case 1:
      setColor(170, 0, 255); // Yellow
      break;
    case 2:
      setColor(0, 0, 255); // Green
      break;
    case 3: // Yellow
      setColor(0,  255, 0); // Blue
      break;
    case 4: // Purple
      setColor(255, 155 , 0);
      break;
    case 5: // Red
      setColor(255, 0, 0); // Red Color
      break;
  }
}

// Sets the colour on the RGB LED
void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Enter heart rate zone number (1-5):");
}

void loop() {
  if (Serial.available() > 0) {
    int zone = Serial.parseInt();
    getZoneColour(zone);

    Serial.println("Enter heart rate zone number (1-5):");
  }
}