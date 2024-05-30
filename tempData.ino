#include <Arduino.h>
#include <Wire.h>
#include <ArduinoJson.h>
#define HTU21D_ADDRESS 0x40

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();

  while (!Serial)
    continue;

  StaticJsonDocument<512> docJson;
  docJson["mac_address"] = "00:1a:2b:3c:4d:5e";
  JsonObject tempData = docJson["data"].to<JsonObject>();
  JsonObject data_temperature_0 = tempData["temperature"].add<JsonObject>();
  JsonArray data_temperature_0_temp = data_temperature_0["temp"].to<JsonArray>();
  float tempVar[5];
  for (int i = 0; i < 5; i++) {
    tempVar[i] = readTemperature();
    data_temperature_0_temp.add(tempVar[i]);
  }
  data_temperature_0["cps"] = 13;
  data_temperature_0["sensor_id"] = 1;

  JsonObject ssidData = tempData.createNestedObject("ssids");

  JsonArray ssidNames = ssidData.createNestedArray("names");
  ssidNames.add("NCAIR IOT");
  ssidNames.add("NCAIR IOT-5G");

  JsonArray ssidStrength = ssidData.createNestedArray("strengths");
  ssidStrength.add(55);
  ssidStrength.add(60);

  String output;
  // serializeJson(docJson, output);
  serializeJsonPretty(docJson, Serial);
}

void loop() {
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
