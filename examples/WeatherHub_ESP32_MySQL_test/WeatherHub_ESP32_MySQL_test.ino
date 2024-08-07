#include <WiFi.h>
#include <HTTPClient.h>

// Your network credentials
const char* ssid = "PutnamChalet";
const char* password = "67355976";

// Server URL (make sure this URL points to your WordPress AJAX handler)
const char* servername = "https://cijeweatherhub.site/wp-admin/admin-ajax.php";

// User-defined variables
const int station_id = 1;
const char* station_name = "453PUTNAM";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(servername);

    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Data to be sent
    String httpRequestData = "action=post_weather_data";
    httpRequestData += "&station_id=" + String(station_id);
    httpRequestData += "&station_name=" + String(station_name);
    httpRequestData += "&datetime=2024-07-01%2012:00:00"; // URL encode the space
    httpRequestData += "&temperature=25.5";
    httpRequestData += "&humidity=60.0";
    httpRequestData += "&pressure=1013.25";
    httpRequestData += "&wind_speed=5.5";
    httpRequestData += "&latitude=40.7128";
    httpRequestData += "&longitude=-74.0060";

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

void loop() {
  // Empty loop
}
