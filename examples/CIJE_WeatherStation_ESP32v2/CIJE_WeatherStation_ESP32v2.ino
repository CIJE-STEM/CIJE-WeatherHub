
#include <BME280I2C.h>
#include <Wire.h>

#define SERIAL_BAUD 115200
int windSensor = 34;    // select the input pin for the potentiometer

float voltage;
float timer;
float ratio;
float rpm;
float inmercury;
float localtemp;
float localhum;
float localpres;

float radius = 0.05; // meters
float diameter; //meters
float windspeed;  //meters per second

BME280I2C bme;

void setup() {
  Serial.begin(SERIAL_BAUD);
  while(!Serial) {} // Wait
  ratio = 3.7/5;
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
}

void loop() {
  
  printBME280Data(&Serial);
     voltage = analogRead(windSensor)/4096.0*3.3;
      windspeed = 2.0*voltage;
      //Serial.print(voltage);
      Serial.print("Windspeed: ");
      Serial.print(windspeed);
      Serial.print("\t");
      Serial.print("Temp: ");
   Serial.print(localtemp); 
   Serial.print("\t\tHumidity: ");
   Serial.print(localhum);
   Serial.print("% RH");
   Serial.print("\t\tPressure: ");
   Serial.print(inmercury);
   Serial.println("inHg\t");
   delay(1000);
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
   /*client->print("Temp: ");
   client->print(temp); 
   client->print("°"+ String(tempUnit == BME280::TempUnit_Celsius ? 'C' :'F'));
   client->print("\t\tHumidity: ");
   client->print(hum);
   client->print("% RH");
   client->print("\t\tPressure: ");
   client->print(inmercury);
   client->println("Pa\t");*/

   //delay(1000);
}
