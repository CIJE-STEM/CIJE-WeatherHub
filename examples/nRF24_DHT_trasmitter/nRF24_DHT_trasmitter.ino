#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DHT.h>

#define DHTPIN 2     // Pin where the DHT sensor is connected
#define DHTTYPE DHT22   // DHT 22 (AM2302), AM2321

const uint64_t pipeOut = 0xE8E8F0F0E1LL; // The same pipe/channel should be in the receiver as well

#define CE_pin 10
#define CSN_pin 9

RF24 radio(CE_pin, CSN_pin);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(9600);
    dht.begin();
    radio.begin();
    radio.setDataRate(RF24_250KBPS); // Set data rate
    radio.setChannel(109); // Set channel, 0-125 are valid channels
    radio.openWritingPipe(pipeOut); // Open writing pipe
    radio.stopListening(); // Set the module to TX mode
}

void loop() {
    float temperature = dht.readTemperature();  // Read temperature as Celsius
    float humidity = dht.readHumidity();  // Read humidity

    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    byte dataToSend[8];
    memcpy(&dataToSend[0], &temperature, sizeof(float));
    memcpy(&dataToSend[4], &humidity, sizeof(float));

    bool result = radio.write(&dataToSend, sizeof(dataToSend)); // Send the data

    if (result) {
        Serial.print("Data sent - Temperature: ");
        Serial.print(temperature);
        Serial.print(" C, Humidity: ");
        Serial.print(humidity);
        Serial.println(" %");
    } else {
        Serial.println("Data sending failed");
    }

    delay(2000); // Wait a few seconds before sending the next data
}
