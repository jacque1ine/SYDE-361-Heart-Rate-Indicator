/*  Heart‑Rate Zone Monitor 
 *  -------------------------------------------------
 *  Input : LCD buttons,  heart‑rate sensor, switch
 *  Output: LCD (HR & zone), RGB LED (zone colour), buzzer (zone change)
 */


/* ---------- Pin assignments ---------- */
const int redPin    = 5;
const int greenPin  = 6;
const int bluePin   = 7;
// const int buzzerPin = ;
// const int switchPin = ;

//TODO: add the LCD select, up, down pins 
//TODO: heart rate pins 

/* ---------- Function prototypes ---------- */
int  getMaxHeartRate(int age);          // 208 – 0.7 × age
int  readFromHeartRateSensor();         // acquire HR from sensor
int  getCurrentZone(int heartRate);     // map HR → zone #
void setLEDColour(int zone);            // drive RGB LED for zone

/* ---------- Global state ---------- */
int           prevZone    = 0;
unsigned long lastSample  = 0;          // ms timestamp of last HR read

// User inputs 
int userAge = 25;
int restingHR = 60;

/* ---------- Arduino lifecycle ---------- */
void setup() {
  // TODO: initialize LCD, RGB LED pins, buzzer pin, buttons, HR sensor
}

void loop() {
  /* Every 5 s, sample heart‑rate and update outputs */
  if (millis() - lastSample >= 5000) {
    int heartRate = readFromHeartRateSensor();
    int currZone  = getCurrentZone(heartRate);

    if (currZone != prevZone) {
      // TODO: buzz to indicate zone change
      setLEDColour(currZone);
    }

    // TODO: display heartRate & currZone on LCD

    prevZone   = currZone;
    lastSample = millis();
  }

  // TODO: handle button input for age, other UI tasks
}

/* ---------- Helper function stubs ---------- */
int getMaxHeartRate(int age) {
  return (int)(208 - 0.7 * age);
}

int readFromHeartRateSensor() {
  // TODO: Replace this with actual sensor reading logic
  return 120; // Example HR for testing
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

void setLEDColour(int zone) {
  // TODO: set RGB LED to colour corresponding to zone
}
