/*
* nRF24 receiver example code for the Uno when paired with an ESP32
* 
* MISO 19
* MOSI 23
* SCK 18
* CE 5
* CSN 4
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t pipeOut = 0xE8E8F0F0E1LL; // This code should be the same as the transmitter

#define CE_pin 5	// Can be any pin as long as the wiring matches the code
#define CSN_pin 4	// Can be any pin as long as the wiring matches the code

RF24 radio(CE_pin, CSN_pin);

void setup() {
    Serial.begin(9600);
    radio.begin();
    radio.setChannel(109);		//0-125 available channels, spaced 1 MHz apart, allowing a range from 2.400 to 2.525 GHz 
    radio.setDataRate(RF24_250KBPS);	//same as the transmitter

    radio.openReadingPipe(1, pipeOut);
    radio.startListening();
}

void loop() {
    if (radio.available()) {
        byte receivedData[8];
        radio.read(&receivedData, sizeof(receivedData)); // Read the values of variable 'receivedData'

        float temperature;
        float humidity;
        memcpy(&temperature, &receivedData[0], sizeof(float)); //because the ESP32 and Uno handle float data differently
        memcpy(&humidity, &receivedData[4], sizeof(float));

        Serial.print("Received - Temperature: ");
        Serial.print(temperature);
        Serial.print(" C, Humidity: ");
        Serial.print(humidity);
        Serial.println(" %");
    }
}
