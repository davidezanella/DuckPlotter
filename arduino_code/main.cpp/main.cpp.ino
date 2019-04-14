#include "ArduinoJson-v6.10.0.h"

#define BAUD 9600

void setup() {
  Serial.begin(BAUD);
}

void loop() {
  String input;
  StaticJsonDocument<200> doc;
  if (Serial.available() > 0) {
    input = Serial.readStringUntil('\n');

    DeserializationError error = deserializeJson(doc, input);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }

    const char* type = doc["type"];
    Serial.println(type);
  }
}
