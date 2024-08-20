/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 6E400001-B5A3-F393-E0A9-E50E24DCCA9E
   Has a characteristic of: 6E400002-B5A3-F393-E0A9-E50E24DCCA9E - used for receiving data with "WRITE"
   Has a characteristic of: 6E400003-B5A3-F393-E0A9-E50E24DCCA9E - used to send data with  "NOTIFY"

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   In this example rxValue is the data received (only accessible inside that function).
   And txValue is the data to be sent, in this example just a byte incremented every second.
*/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;
#define DHT11_PIN 17

BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;
//String strArray[3] = "";
String strVal;
int strLen;
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"  // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer *pServer) {
    deviceConnected = false;
  }
};

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    String rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0) {
      Serial.println("*********");
      Serial.print("Received Value: ");
      for (int i = 0; i < rxValue.length(); i++) {
        Serial.print(rxValue[i]);
      }

      Serial.println();
      Serial.println("*********");
    }
  }
};

void setup() {
  Serial.begin(115200);
  analogSetAttenuation(ADC_11db);  //150mV - 2600mV
  analogReadResolution(12);
  // Create the BLE Device
  BLEDevice::init("cije32WSTX");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);

  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
    delay(500);                   // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising();  // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
  }
  
  if (deviceConnected) {
    
    DHT.read(DHT11_PIN);
    //_________________________________________
    //Temperature convert - send
    txValue = '*';  //"*"
    pTxCharacteristic->setValue(&txValue, 1);
    pTxCharacteristic->notify();
    Serial.print(txValue);

    txValue = 'T'; //"T"
    pTxCharacteristic->setValue(&txValue, 1);
    pTxCharacteristic->notify();
    Serial.print(txValue);
    
    delay(20);
    strVal = String(DHT.temperature);
    strLen = strVal.length();
    Serial.println(strLen);

    for(int ptr = 0; ptr < strLen; ptr++)
    {
      txValue = strVal.charAt(ptr);;
      pTxCharacteristic->setValue(&txValue, 1);
      pTxCharacteristic->notify();
      Serial.print(txValue);
    }
    
    //Send a carriage return and line feed
    delay(20);
    txValue = 10;
    pTxCharacteristic->setValue(&txValue, 1);
    pTxCharacteristic->notify();
    Serial.print(txValue);
    
    delay(20);
    txValue = 13;
    pTxCharacteristic->setValue(&txValue, 1);
    pTxCharacteristic->notify();
    Serial.print(txValue);
     
    //____________________________________________
    //Humidity convert - send
    
    txValue = '*';
    pTxCharacteristic->setValue(&txValue, 1);
    pTxCharacteristic->notify();
    Serial.print(txValue);

    txValue = 'H'; //"H"
    pTxCharacteristic->setValue(&txValue, 1);
    pTxCharacteristic->notify();
    Serial.print(txValue);
    
    delay(20);
    strVal = String(DHT.humidity);
    strLen = strVal.length();
    Serial.println(strLen);

    for(int ptr = 0; ptr < strLen; ptr++)
    {
      txValue = strVal.charAt(ptr);;
      pTxCharacteristic->setValue(&txValue, 1);
      pTxCharacteristic->notify();
      Serial.print(txValue);
    }
    //Send a carriage return and line feed
    delay(20);
    txValue = 10;
    pTxCharacteristic->setValue(&txValue, 1);
    pTxCharacteristic->notify();
    Serial.print(txValue);
    
    delay(20);
    txValue = 13;
    pTxCharacteristic->setValue(&txValue, 1);
    pTxCharacteristic->notify();
    Serial.print(txValue);
  //_____________________________________________
  // Wind Speed convert - send

  


  //____________________________________________  
    Serial.print(DHT.temperature);
    Serial.print("  humi:");
    Serial.println(DHT.humidity);

    delay(100);  // bluetooth stack will go into congestion, if too many packets are sent
  }

  
}
