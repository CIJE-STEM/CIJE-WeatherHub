#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32Servo.h>

// WiFi credentials
const char* ssid = "PutnamChalet";
const char* password = "67355976";

// Server URL (make sure this URL points to your WordPress AJAX handler)
const char* servername = "https://cijeweatherhub.site/wp-admin/admin-ajax.php";

#define SERVOPIN 15     // Pin connected to the servo
Servo myservo;

// RGB LED Common is 3V
const int redPin = 14;   // GPIO pin for the red LED
const int greenPin = 27; // GPIO pin for the green LED
const int bluePin = 26;  // GPIO pin for the blue LED

// nRF24 setup
const uint64_t pipeOut = 0xE8E8F0F0E1LL; // This pipe should be the same as the transmitter
#define CE_pin 5
#define CSN_pin 4
RF24 radio(CE_pin, CSN_pin);

// User-defined variables
const int station_id = 1;
const char* station_name = "453PUTNAM";
float latitude = 40.7128; // Placeholder, replace with actual latitude
float longitude = -74.0060; // Placeholder, replace with actual longitude
float pressure = 0; // Placeholder, replace with actual pressure
float wind_speed = 0; // Placeholder, replace with actual wind speed

int servoMin = 135;    // Minimum servo position
int servoMax = 45;  // Maximum servo position
int tempMin = -4000; // Corresponds to -40.00°C
int tempMax = 8000;  // Corresponds to 80.00°C
int mappedValue = 0;

float temperature;
float humidity;
unsigned long lastPostTime = 0; // To keep track of the last post time
const unsigned long postInterval = 60000; // 10 seconds interval for testing

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    myservo.attach(SERVOPIN);

    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    radio.begin();
    radio.setChannel(109);
    radio.setDataRate(RF24_250KBPS);
    radio.openReadingPipe(1, pipeOut);
    radio.startListening();

    Serial.println("Receiver setup complete, listening...");
}

void loop() {
    if (radio.available()) {
        Serial.println("Data available!");
        byte receivedData[8];
        radio.read(&receivedData, sizeof(receivedData)); // Read the values of variable 'receivedData'

        memcpy(&temperature, &receivedData[0], sizeof(float));
        memcpy(&humidity, &receivedData[4], sizeof(float));
        int tempInt = (int)(temperature * 100);
        mappedValue = map(tempInt, tempMin, tempMax, servoMin, servoMax);
        Serial.print("Servo angle:");
        Serial.println(mappedValue);
        myservo.write(mappedValue);

        int redValue = map(temperature, -40, 80, 255, 0);
        int blueValue = map(temperature, -40, 80, 0, 255);
        int greenValue = 255; // Optional: Adjust green value for better gradient

    // Apply the color to the RGB LED
        analogWrite(redPin, redValue);
        analogWrite(greenPin, greenValue);
        analogWrite(bluePin, blueValue);
        
    //Serial printing
        Serial.print("Received - Temperature: ");
        Serial.print(temperature);
        Serial.print(" C, Humidity: ");
        Serial.print(humidity);
        Serial.println(" %");

        unsigned long currentTime = millis();
        if (currentTime - lastPostTime >= postInterval) {
            postToDatabase(temperature, humidity);
            lastPostTime = currentTime;
        }
    }

    delay(1000); // Small delay to avoid flooding the serial output
}

void postToDatabase(float temperature, float humidity) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;

        http.begin(servername);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        // Data to be sent
        String httpRequestData = "action=post_weather_data";
        httpRequestData += "&station_id=" + String(station_id);
        httpRequestData += "&station_name=" + String(station_name);
        httpRequestData += "&datetime=2024-07-01%2012:00:00"; // Placeholder datetime, replace with actual datetime if needed
        httpRequestData += "&temperature=" + String(temperature, 2);
        httpRequestData += "&humidity=" + String(humidity, 2);
        httpRequestData += "&pressure=" + String(pressure, 2);
        httpRequestData += "&wind_speed=" + String(wind_speed, 2);
        httpRequestData += "&latitude=" + String(latitude, 4);
        httpRequestData += "&longitude=" + String(longitude, 4);

        Serial.print("Sending request: ");
        Serial.println(httpRequestData);

        int httpResponseCode = http.POST(httpRequestData);

        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println(httpResponseCode);
            Serial.println("Response payload: " + response);
        } else {
            Serial.print("Error on sending POST: ");
            Serial.println(httpResponseCode);
        }

        http.end();
    } else {
        Serial.println("WiFi Disconnected");
    }
}
