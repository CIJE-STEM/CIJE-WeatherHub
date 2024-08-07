#include <WiFi.h>
#include <HTTPClient.h>

// Replace with your network credentials
const char* ssid = "PutnamChalet";
const char* password = "67355976";

// Server URL
const char* serverName = "https://www.cijeweatherhub.site/post_weather_data.php";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Post weather data
  postWeatherData();
}

void loop() {
  // Your code to collect weather data
  // You can call postWeatherData() periodically to send new data
}

void postWeatherData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String httpRequestData = "station_id=2"
                           "&station_name=WeatherStation2"
                           "&temperature=25.5"
                           "&humidity=60.5"
                           "&pressure=1013.25"
                           "&wind_speed=5.5"
                           "&latitude=40.7128"
                           "&longitude=-74.0060";
    
    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}