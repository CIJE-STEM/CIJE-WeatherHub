#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t pipeOut = 0xE8E8F0F0E1LL; // This pipe should be the same as the transmitter

#define CE_pin 5
#define CSN_pin 4

RF24 radio(CE_pin, CSN_pin);

void setup() {
    Serial.begin(9600);
    Serial.println("Serial started");
    
    if (!radio.begin()) {
        Serial.println("Radio hardware not responding!");
        while (1) {} // halt the program
    }
    Serial.println("Radio hardware is responding");

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

        float temperature;
        float humidity;
        memcpy(&temperature, &receivedData[0], sizeof(float));
        memcpy(&humidity, &receivedData[4], sizeof(float));

        Serial.print("Received - Temperature: ");
        Serial.print(temperature);
        Serial.print(" C, Humidity: ");
        Serial.print(humidity);
        Serial.println(" %");
    }

    delay(1000); // Small delay to avoid flooding the serial output
}
