#include <AFMotor.h> // L293D Motor Driver Shield lib
#define STEPPER_SPEED 10 // rpm

#define SWITCHX A0  //swtich analogic pin 
#define SWITCHY A1  //swtich analogic pin 
//#define MODE MICROSTEP // Micrsostep steps
#define MODE MICROSTEP // Micrsostep steps
#define SERIAL 1    //enable serial transmission

// Number of steps per output rotation
// Change this as per your motor's specification
const int stepsPerRevolution = 200; // For NEMA 17 this value is 200 

// connect motor to port #2 (M3 and M4)
AF_Stepper motorY(stepsPerRevolution, 2);
// connect motor to port #1 (M1 and M2)
AF_Stepper motorX(stepsPerRevolution, 1);

void setup() {
  motorX.setSpeed(STEPPER_SPEED); 
  motorY.setSpeed(STEPPER_SPEED);  
  if(SERIAL) 
    Serial.begin(9600);
  //set switch pins as input with pullup resistor
  pinMode(SWITCHX,INPUT_PULLUP);
  pinMode(SWITCHY,INPUT_PULLUP);
  pinMode(13,HIGH); //LED pin
}

void loop(){
  delay(7000);
  homeXY();
  delay(100);
}

bool limitSwitch(char sw){
  //with pull-up resistors, LOW means the switch is opened and HIGH that is closed
  bool res;
  digitalRead(sw) == HIGH ? res = true : res = false;
  return res;
}

void homeXY(){  //move the end effector in home position
  homeX();
  homeY();
}

void homeX(){
  int steps_x = 0;
  unsigned long time_x = millis();
  while(!limitSwitch(SWITCHX)){
    motorX.step(1, FORWARD, MODE); //direction to test
    steps_x++;  //count steps
  }
  time_x = millis() - time_x;
  if(SERIAL){ //print time and steps values on the Serial
    Serial.print("X time: ");
    Serial.print(time_x);
    Serial.print("\t steps: ");
    Serial.println(steps_x);
  }
}

void homeY(){
  int steps_y = 0;
  unsigned long time_y = millis();
  while(!limitSwitch(SWITCHY)){
    motorY.step(1, BACKWARD, MODE); //direction to test
    steps_y++;  //count steps
  }
  time_y = millis() - time_y;

  if(SERIAL){ //print time and steps values on the Serial
    Serial.print("Y time: ");
    Serial.print(time_y);
    Serial.print("\t steps: ");
    Serial.println(steps_y);
  }
}
