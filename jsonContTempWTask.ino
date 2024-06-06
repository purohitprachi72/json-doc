#include <Arduino.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFiMulti.h>
#define HTU21D_ADDRESS 0x40

#include <esp_task.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
// #include "soc/rtc_wdt.h"
// #include "esp_int_wdt.h"
#include "esp_task_wdt.h"

const char *AP_SSID = "12345";
const char *AP_PWD = "123";

WiFiMulti wifiMulti;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  while (!Serial)
    continue;
  delay(1000);
  wifiMulti.addAP(AP_SSID, AP_PWD);

  xTaskCreate(
    sensorTask,    // Function that should be called
    "Sensor Task", // Name of the task (for debugging)
    10000,         // Stack size (bytes)
    NULL,          // Parameter to pass
    1,             // Task priority
    NULL,           // Task handle
  );

  vTaskDelete(NULL); // Delete the setup and loop task to free up resources

  // postDataToServer();
}

void loop() {

}

void sensorTask(void *pvParameters) {
  const TickType_t xDelay10s = pdMS_TO_TICKS(10000); // 10 seconds delay
  for (;;) {
    postDataToServer();
    vTaskDelay(xDelay10s);  
  }
}

float readTemperature() {
  Wire.beginTransmission(HTU21D_ADDRESS);
  Wire.write(0xE3);  // Temperature measurement command
  Wire.endTransmission();

  delay(50);  // Wait for measurement to complete

  Wire.requestFrom(HTU21D_ADDRESS, 2);
  if (Wire.available() == 2) {
    uint16_t rawTemperature = Wire.read() << 8 | Wire.read();
    // Convert raw temperature to Celsius
    float temperature = -46.85 + 175.72 * (rawTemperature / 65536.0);
    return temperature;
  } else {
    return NAN;  // Return NaN if data is not available
  }
}

void postDataToServer() {
  HTTPClient http;
  Serial.println("Posting JSON data to server...");
  // Block until we are able to connect to the WiFi access point
  if (wifiMulti.run() == WL_CONNECTED) {

    float temperatureSensorReading = readTemperature();

    http.begin("http://10.185.151.167/api/api/iot/data");
    http.addHeader("Content-Type", "application/json");

    //Json

    StaticJsonDocument<512> docJson;
    // String wifiMacString = WiFi.macAddress();
    docJson["mac_address"] = WiFi.macAddress();  //modified
    // docJson["mac_address"] = "00:1a:2b:3c:4d:5e";
    JsonObject tempData = docJson["data"].to<JsonObject>();

    JsonObject data_temperature_0 = tempData["temperature"].add<JsonObject>();

    //temp data in continous mode
    data_temperature_0["temp"] = temperatureSensorReading;

    data_temperature_0["sensor_id"] = 1;

    JsonObject ssidData = tempData.createNestedObject("ssids");

    JsonArray ssidNames = ssidData.createNestedArray("names");
    ssidNames.add("NCAIR IOT");
    ssidNames.add("NCAIR IOT-5G");

    JsonArray ssidStrength = ssidData.createNestedArray("strengths");
    ssidStrength.add(55);
    ssidStrength.add(60);

    String output;
    serializeJson(docJson, output);
    serializeJsonPretty(docJson, Serial);

    int httpResponseCode = http.POST(output);

    if (httpResponseCode > 0) {

      String response = http.getString();

      Serial.println(httpResponseCode);
      Serial.println(response);

    } else {

      // Serial.printf("Error occurred while sending HTTP POST: %s\n", httpClient.errorToString(statusCode).c_str());
      Serial.println("error");
    }
    //http.end(); ??? && output = ""; @96
  }
}
