#include <AFMotor.h> // L293D Motor Driver Shield lib
#define STEPPER_SPEED 10 // rpm
//select one and commet the others:
//#define MODE SINGLE // Single coil steps
//#define MODE DOUBLE // Double coil steps
//#define MODE INTERLEAVE // Interleave coil steps
#define MODE MICROSTEP // Micrsostep steps

// Number of steps per output rotation
// Change this as per your motor's specification
const int stepsPerRevolution = 200; // For NEMA 17 this value is 200 

// connect motor to port #2 (M3 and M4)
AF_Stepper motorX(stepsPerRevolution, 2);
// connect motor to port #1 (M1 and M2)
AF_Stepper motorY(stepsPerRevolution, 1);

void setup() {
  motorX.setSpeed(STEPPER_SPEED); 
  motorY.setSpeed(STEPPER_SPEED);    
}

void loop() {
  motorX.step(100, FORWARD, MODE);
  motorY.step(100, FORWARD, MODE);
  delay(100);  
  motorX.step(100, BACKWARD, MODE); 
  motorY.step(100, BACKWARD, MODE);
  delay(100);
}
