/*
* nRF24 receiver example code
* ESP32 pins
* MISO 19
* MOSI 23
* SCK 18
* CE 5
* CSN 4
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t pipeOut = 0xE8E8F0F0E1LL; // This pipe should be the same as the transmitter

#define CE_pin 5
#define CSN_pin 4

RF24 radio(CE_pin, CSN_pin);

void setup() {
    Serial.begin(9600);
    radio.begin();
    radio.setChannel(109);
    radio.setDataRate(RF24_250KBPS);

    radio.openReadingPipe(1, pipeOut);
    radio.startListening();
}

void loop() {
    if (radio.available()) {
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
}
