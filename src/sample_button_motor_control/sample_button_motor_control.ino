#define STEP_PIN 3
#define DIR_PIN  4
#define ON_BOARD_LED_PIN 2
#define SWITCH_2_PIN 13
#define SWITCH_3_PIN 12


void setup() {
  delay(2000); // ms

  Serial.begin(9600);
  while (!Serial); // wait for USB to be ready

  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ON_BOARD_LED_PIN, OUTPUT);
  pinMode(SWITCH_2_PIN, INPUT);
  pinMode(SWITCH_3_PIN, INPUT);

    // Blink LED to indicate program started!
  for (int i = 0; i < 3; i++) {
    Serial.println("starting blinking");
    digitalWrite(ON_BOARD_LED_PIN, HIGH);
    delay(200); // ms
    digitalWrite(ON_BOARD_LED_PIN, LOW);
    delay(200); // ms
  }
  
}

void stepOnce() {
  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(50);   // pulse width
  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(50);
}

void stepMultiple(bool rotate_clockwise, uint16_t num_turns) {

  if (rotate_clockwise){
    digitalWrite(DIR_PIN, HIGH);  // set direction
    Serial.println("Step direction clockwise rotation");

  } else {
    digitalWrite(DIR_PIN, LOW);  // set direction
    Serial.println("Step direction counter-clockwise rotation");

  }

  Serial.printf("Rotating %u steps \r\n", num_turns);
  
  for (int i = 0; i < num_turns; i++) {
    stepOnce();
  }
}


static uint32_t lastTime[32] = {0};
static bool lastStable[32] = {HIGH};

bool debouncePressed(uint8_t pin, uint32_t debounce_ms = 50) {
  bool reading = digitalRead(pin);

  if (reading != lastStable[pin]) {
    lastTime[pin] = millis();
  }

  if ((millis() - lastTime[pin]) > debounce_ms) {

    if (reading != lastStable[pin]){
      lastStable[pin] = reading;
    }
    
    if (reading == LOW) {
      return true;
    }
  }

  return false;
}


void loop() {
  // Turn the servo clockwise
  if (debouncePressed(SWITCH_2_PIN)){
    stepMultiple(true, 1000);
  }

  // Turn the servo counter-clockwise
  if (debouncePressed(SWITCH_3_PIN)){
    stepMultiple(false, 1000);
  }
}
