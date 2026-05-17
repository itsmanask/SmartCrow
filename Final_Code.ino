//2 pirs 2 servo 5 second lockout final

#include <Servo.h>

Servo servo1;
Servo servo2;

const int pir1 = 7;
const int pir2 = 6;
const int ledPin = 8;

bool lockout = false;
unsigned long lockoutStart = 0;

void setup() {
  servo1.attach(2);
  servo2.attach(3);

  pinMode(pir1, INPUT);
  pinMode(pir2, INPUT);
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  // ---------- LOCKOUT MODE ----------
  if (lockout) {
    // LED should remain OFF when idle
    digitalWrite(ledPin, LOW);

    // If 5 seconds passed → end lockout
    if (millis() - lockoutStart >= 5000) {
      lockout = false;
      Serial.println("PIR Lockout Ended");
    }
    return;   // Do NOT read PIRs during lockout
  }

  // ---------- READ BOTH PIRs (only if NOT in lockout) ----------
  int motion1 = digitalRead(pir1);
  int motion2 = digitalRead(pir2);

  if (motion1 == HIGH || motion2 == HIGH) {

    Serial.println("Motion Detected!");

    // ⚡ START LOCKOUT IMMEDIATELY (IMPORTANT)
    lockout = true;
    lockoutStart = millis();
    Serial.println("PIR Lockout Started (5 seconds)");

    digitalWrite(ledPin, HIGH); // LED ON during action

    // ------ Servo Action ------
    for (int pos = 0; pos <= 180; pos++) {
      servo1.write(pos);
      servo2.write(180 - pos);
      delay(15);
    }

    delay(500);

    for (int pos = 180; pos >= 0; pos--) {
      servo1.write(pos);
      servo2.write(180 - pos);
      delay(15);
    }

    digitalWrite(ledPin, LOW); // OFF immediately after action
    Serial.println("Motion Action Completed");
  }

  delay(100);
}