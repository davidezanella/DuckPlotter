#include <ArduinoJson.h>
#include "DuckPlotter.h"

#define BAUD 9600

Position last;
DuckPlotter plotter = DuckPlotter();

void setup() {
  Serial.begin(BAUD);
  plotter.init();

  last.x = 0;
  last.y = 0;
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
    else if (type == "sync") {
      last.x = doc['x'];
      last.y = doc['y']
    }
    else if (type == "movement") {
      String moveType = doc["move_type"];
      Position to;
      to.x = doc["toX"];
      to.y = doc["toY"];
      bool clockwise = doc["clockwise"];
      if (moveType == "linear") {
        last = plotter.moveLinear(last, to);
      }
      else if (moveType == "arc") {
        if (doc.containsKey("offsetX")) {
          Position offset;
          offset.x = doc["offsetX"];
          offset.y = doc["offsetY"];
          last = plotter.moveArc(last, to, offset, clockwise);
        }
        else {
          double radius = doc["radius"];
          last = plotter.moveArc(last, to, radius, clockwise);
        }
      }
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
