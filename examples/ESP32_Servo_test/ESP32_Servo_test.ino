#include <ESP32Servo.h>

// Define pins
#define SERVOPIN 15     // Pin connected to the servo

Servo myservo;

void setup() {
  Serial.begin(115200);

  // Attach servo to the specified pin
  myservo.attach(SERVOPIN);
}

void loop() {
  // Move to 90 degrees
  myservo.write(90);
  delay(1000);
  Serial.println("Moved to 90 degrees");

  // Move to 125 degrees
  myservo.write(125);
  delay(1000);
  Serial.println("Moved to 125 degrees");

  // Move back to 90 degrees
  myservo.write(90);
  delay(1000);
  Serial.println("Moved to 90 degrees");

  // Move to 55 degrees
  myservo.write(55);
  delay(1000);
  Serial.println("Moved to 55 degrees");

  // Wait a bit before the next loop iteration
  delay(2000);
}
