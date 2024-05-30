// {
//   "mac_address": "00:1A:2B:3C:4D:5E",
//   "data": {
//     "vibration": [
//       {
//         "ax": [0.3, -0.215, 0.658],
//         "ay": [-0.456, 0.134, -0.891],
//         "az": [0.721, -0.609, 0.475],
//         "cps": 13,
//         "sensor_id": 1
//       }
//     ],
//     "ssids": {
//       "names": ["NCAIR IOT", "NCAIR IOT 5g"],
//       "strengths": [55, 60]
//     }
//   }
// }

//CAPACITY = JSON_ARRAY_SIZE(1) + 2*JSON_ARRAY_SIZE(2) + 3*JSON_ARRAY_SIZE(3) + 3*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(5)

/** 
* Data structures	400	Bytes needed to stores the JSON objects and arrays in memory 
* Strings	41	Bytes needed to stores the strings in memory 
* Total (minimum)	441	Minimum capacity for the JsonDocument.
* Total (recommended)	512	Including some slack in case the strings change, and rounded to a power of two
**/

#include <ArduinoJson.h>

void setup(){

  Serial.begin(115200);
  StaticJsonDocument<512> doc;

  doc["mac_address"] = "00:1A:2B:3C:4D:5E";

  JsonObject data = doc.createNestedObject("data");

  JsonObject data_vibration_0 = data["vibration"].createNestedObject();

  JsonArray data_vibration_0_ax = data_vibration_0.createNestedArray("ax");
  data_vibration_0_ax.add(0.3);
  data_vibration_0_ax.add(-0.215);
  data_vibration_0_ax.add(0.658);

  JsonArray data_vibration_0_ay = data_vibration_0.createNestedArray("ay");
  data_vibration_0_ay.add(-0.456);
  data_vibration_0_ay.add(0.134);
  data_vibration_0_ay.add(-0.891);

  JsonArray data_vibration_0_az = data_vibration_0.createNestedArray("az");
  data_vibration_0_az.add(0.721);
  data_vibration_0_az.add(-0.609);
  data_vibration_0_az.add(0.475);
  data_vibration_0["cps"] = 13;
  data_vibration_0["sensor_id"] = 1;

  JsonObject data_ssids = data.createNestedObject("ssids");

  JsonArray data_ssids_names = data_ssids.createNestedArray("names");
  data_ssids_names.add("NCAIR IOT");
  data_ssids_names.add("NCAIR IOT 5g");

  JsonArray data_ssids_strengths = data_ssids.createNestedArray("strengths");
  data_ssids_strengths.add(55);
  data_ssids_strengths.add(60);

  serializeJsonPretty(doc, Serial);
}

void loop(){

}
