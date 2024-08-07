
#include <BME280I2C.h>
#include <Wire.h>
#include "config.h"

#define IO_LOOP_DELAY 15000
unsigned long lastUpdate;
#define SERIAL_BAUD 115200
int windSensor = 34;    // select the input pin for the potentiometer

AdafruitIO_Feed *location = io.feed("location");
AdafruitIO_Feed *humidity = io.feed("humidity");
AdafruitIO_Feed *pressure = io.feed("pressure");
AdafruitIO_Feed *windspeed = io.feed("windspeed");
AdafruitIO_Feed *temperature = io.feed("temperature");


float voltage;
float timer;
float rpm;
float inmercury;
float localtemp;
float localhum;
float localpres;
float wspeed;  //meters per second
int value = 0;

// these double values will hold our fake GPS data
float lat = 40.684877;
float lon = -73.942996;
float ele = 18;
BME280I2C bme;

void setup() {
  Serial.begin(SERIAL_BAUD);
  while(!Serial) {} // Wait

  
  Wire.begin();

  while(!bme.begin())
  {
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }

  switch(bme.chipModel())
  {
     case BME280::ChipModel_BME280:
       Serial.println("Found BME280 sensor! Success.");
       break;
     case BME280::ChipModel_BMP280:
       Serial.println("Found BMP280 sensor! No Humidity available.");
       break;
     default:
       Serial.println("Found UNKNOWN sensor! Error!");
  }
  Serial.print("Connecting to Adafruit IO");
  io.connect();
  humidity->onMessage(handleCount);
  pressure->onMessage(handleCount);
  windspeed->onMessage(handleCount);
  temperature->onMessage(handleCount);
  location->onMessage(handleCount);
    // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  humidity->get();
  pressure->get();
  windspeed->get();
  temperature->get();
  location->get();
  location->save(value, lat, lon, ele);
}

void loop() {
   io.run();
   printBME280Data(&Serial);
   voltage = analogRead(windSensor)/4096.0*3.3;
   wspeed = 2.0*voltage;
   
   if (millis() > (lastUpdate + IO_LOOP_DELAY)) {
    Serial.println();
    windspeed->save(wspeed);
    Serial.print("Windspeed: ");
    Serial.print(wspeed);
    Serial.print("\t");
    temperature->save(localtemp);
    Serial.print("Temperature: ");
    Serial.print(localtemp);
    Serial.print("\t");
    humidity->save(localhum);
    Serial.print("Humidity: ");
    Serial.print(localhum);
    Serial.print("\t");
    pressure->save(inmercury);
    Serial.print("Pressure: ");
    Serial.print(inmercury);
    Serial.print("\t");


    // update timer
    lastUpdate = millis();
  }

      //Serial.print(voltage);

   //delay(1000);
}


void printBME280Data (Stream* client) {
   float temp(NAN), hum(NAN), pres(NAN);

   BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
   BME280::PresUnit presUnit(BME280::PresUnit_Pa);

   bme.read(pres, temp, hum, tempUnit, presUnit);
   inmercury = pres * 0.00029530;
   localtemp = temp;
   localhum = hum;
   localpres = pres;
/*   client->print("Temp: ");
   client->print(temp); 
   client->print("°"+ String(tempUnit == BME280::TempUnit_Celsius ? 'C' :'F'));
   client->print("\t\tHumidity: ");
   client->print(hum);
   client->print("% RH");
   client->print("\t\tPressure: ");
   client->print(inmercury);
   client->println("Pa\t");
*/
   //delay(1000);
}

// you can also attach multiple feeds to the same
// meesage handler function. both counter and counter-two
// are attached to this callback function, and messages
// for both will be received by this function.
void handleCount(AdafruitIO_Data *data) {

  Serial.print("received <- ");

  // since we are using the same function to handle
  // messages for two feeds, we can use feedName() in
  // order to find out which feed the message came from.
  Serial.print(data->feedName());
  Serial.print(" ");

  // print out the received count or counter-two value
  Serial.println(data->value());

}
