#include "ArduinoJson-v6.10.0.h"
#include "operations.h"

#define BAUD 9600
#define STEPPER_SPEED 10 // rpm
#define MODE MICROSTEP // Micrsostep steps

// Number of steps per output rotation
// Change this as per your motor's specification
const int stepsPerRevolution = 200; // For NEMA 17 this value is 200

// connect motor to port #2 (M3 and M4)
AF_Stepper motorX(stepsPerRevolution, 2);
// connect motor to port #1 (M1 and M2)
AF_Stepper motorY(stepsPerRevolution, 1);

float lastX, lastY, lastZ;

void setup() {
  Serial.begin(BAUD);
  motorX.setSpeed(STEPPER_SPEED);
  motorY.setSpeed(STEPPER_SPEED);
  lastX = lastY = lastZ = 0;
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

    char* type = doc["type"];
    Serial.println(type);

    if (type == "movement") {
      char* moveType = doc["move_type"];
      float toX = doc["toX"];
      float toY = doc["toY"];
      bool clockwise = doc["clockwise"];
      if (moveType == "linear") {
        moveLinear(motorX, motorY, lastX, lastY, toX, toY);
      }
      else if (moveType == "arc") {
        if (doc.containsKey("offsetX")) {
          float offsetX = doc["offsetX"];
          float offsetY = doc["offsetY"];
          moveArc(motorX, motorY, lastX, lastY, toX, toY, offsetX, offsetY, clockwise);
        }
        else {
          float radius = doc["radius"];
          moveArc(motorX, motorY, lastX, lastY, toX, toY, radius, clockwise);
        }
      }
      lastX = toX;
      lastY = toY;
    }
    else if (type == "pen") {
      bool down = doc["down"];
      movePen(down);
    }
  }
}
