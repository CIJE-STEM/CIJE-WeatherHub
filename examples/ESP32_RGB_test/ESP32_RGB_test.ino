// RGB LED Common is 3V

const int redPin = 14;   // GPIO pin for the red LED
const int greenPin = 27; // GPIO pin for the green LED
const int bluePin = 26;  // GPIO pin for the blue LED

void setup() {
    // Initialize serial communication for debugging
    Serial.begin(115200);
    
    // Set the LED pins as outputs
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);

    Serial.println("RGB LED Test Starting...");
}

void loop() {
    // Test red color
    Serial.println("Red");
    analogWrite(redPin, 0); // Full brightness
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 255);
    delay(1000);

    // Test green color
    Serial.println("Green");
    analogWrite(redPin, 255);
    analogWrite(greenPin, 0); // Full brightness
    analogWrite(bluePin, 255);
    delay(1000);

    // Test blue color
    Serial.println("Blue");
    analogWrite(redPin, 255);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 0); // Full brightness
    delay(1000);

    // Test white color (all LEDs on)
    Serial.println("White");
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
    delay(1000);

    // Test off (all LEDs off)
    Serial.println("Off");
    analogWrite(redPin, 255);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 255);
    delay(1000);
}