#include <ArduinoJson.h>
#include "DuckPlotter.h"

#define BAUD 9600

float lastX, lastY, lastZ;
DuckPlotter plotter = DuckPlotter();

void setup() {
  Serial.begin(BAUD);
  plotter.init();

  lastX = lastY = lastZ = 0;
}

void start() {
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

    String type = doc["type"];

    if (type == "reset") {
      plotter.reset();
    }
    else if (type == "movement") {
      String moveType = doc["move_type"];
      float toX = doc["toX"];
      float toY = doc["toY"];
      bool clockwise = doc["clockwise"];
      if (moveType == "linear") {
        plotter.moveLinear(lastX, lastY, toX, toY);
      }
      else if (moveType == "arc") {
        if (doc.containsKey("offsetX")) {
          float offsetX = doc["offsetX"];
          float offsetY = doc["offsetY"];
          plotter.moveArc(lastX, lastY, toX, toY, offsetX, offsetY, clockwise);
        }
        else {
          float radius = doc["radius"];
          plotter.moveArc(lastX, lastY, toX, toY, radius, clockwise);
        }
      }
      lastX = toX;
      lastY = toY;
    }
    else if (type == "pen") {
      bool down = doc["down"];
      plotter.movePen(down);
    }

    Serial.println("Done");
  }
}

void loop()
{
  start();
}
