#include <ArduinoJson.h>
#include "operations.h"

#define BAUD 9600

#define SERVO_PIN 9


Servo penMotor;

float lastX, lastY, lastZ;

void setup() {
  Serial.begin(BAUD);

  penMotor.attach(SERVO_PIN);
  penMotor.write(50);

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
      reset();
    }
    else if (type == "movement") {
      String moveType = doc["move_type"];
      float toX = doc["toX"];
      float toY = doc["toY"];
      bool clockwise = doc["clockwise"];
      if (moveType == "linear") {
        moveLinear(lastX, lastY, toX, toY);
      }
      else if (moveType == "arc") {
        if (doc.containsKey("offsetX")) {
          float offsetX = doc["offsetX"];
          float offsetY = doc["offsetY"];
          moveArc(lastX, lastY, toX, toY, offsetX, offsetY, clockwise);
        }
        else {
          float radius = doc["radius"];
          moveArc(lastX, lastY, toX, toY, radius, clockwise);
        }
      }
      lastX = toX;
      lastY = toY;
    }
    else if (type == "pen") {
      bool down = doc["down"];
      movePen(down, penMotor);
    }

    Serial.println("Done");
  }
}

void loop()
{
  start();
}
