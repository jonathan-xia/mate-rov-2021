#include <Servo.h>

// Joystick Settings
static const int JS_CENTER_0 = 511; // Analog reading at center, 0-1023
static const int JS_CENTER_1 = 511;
static const int JS_CENTER_2 = 511;
static const int JS_RANGE_0 = 128; // Analog range, 0-1023
static const int JS_RANGE_1 = 128; // Set to 128 for Parallax joystick
static const int JS_RANGE_2 = 128;
static const int JS_DIR_0 = 1; // +1 or -1
static const int JS_DIR_1 = 1;
static const int JS_DIR_2 = 1;

// ESC/Thruster Settings
static const int MAX_FWD_REV_THROTTLE = 400; // Value between 0-400
static const int MAX_TURN_THROTTLE = 400; // Value between 0-400
static const int MAX_VERTICAL_THROTTLE = 400; // Value between 0-400
static const int CENTER_THROTTLE = 1500;

// Arduino Pins
static const byte JS_ADC_0 = A0;
static const byte JS_ADC_1 = A1;
static const byte JS_ADC_2 = A2;
static const byte THRUSTER_F_LEFT = 9;
static const byte THRUSTER_F_RIGHT = 10;
static const byte THRUSTER_B_LEFT = 11;
static const byte THRUSTER_B_RIGHT = 12;
static const byte THRUSTER_M_LEFT = 13;
static const byte THRUSTER_M_RIGHT = 14;

// Servos
Servo frontLeft;
Servo frontRight;
Servo backLeft;
Servo backRight;
Servo midLeft;
Servo midRight;

void setup() {
  // Set up serial port to print inputs and outputs
  Serial.begin(38400);

  // Set up Arduino pins to send servo signals to ESCs
  thrusterFrontLeft.attach(THRUSTER_F_LEFT);
  thrusterFrontRight.attach(THRUSTER_F_RIGHT);
  thrusterBackLeft.attach(THRUSTER_B_LEFT);
  thrusterBackRight.attach(THRUSTER_B_RIGHT);
  thrusterMidLeft.attach(THRUSTER_M_LEFT);
  thrusterMidRight.attach(THRUSTER_M_RIGHT);

  // Set output signal to 1500 microsecond pulse (stopped command)
  thrusterFrontLeft.writeMicroseconds(CENTER_THROTTLE);
  thrusterFrontRight.writeMicroseconds(CENTER_THROTTLE);
  thrusterBackLeft.writeMicroseconds(CENTER_THROTTLE);
  thrusterBackRight.writeMicroseconds(CENTER_THROTTLE);
  thrusterMidLeft.writeMicroseconds(CENTER_THROTTLE);
  thrusterMidRight.writeMicroseconds(CENTER_THROTTLE);

  // Delay to allow time for ESCs to initialize
  delay(7000); 
}

void loop() {
  // Read the joysticks and use the Arduino "map" function to map the raw values
  // to the desired output commands.
  int forwardCommand    = map(analogRead(JS_ADC_0), // Read raw joystick value
                              JS_CENTER_0-JS_DIR_0*JS_RANGE_0, // Joystick low value
                              JS_CENTER_0+JS_DIR_0*JS_RANGE_0, // Joystick high value
                              -MAX_FWD_REV_THROTTLE, // Command low value
                              MAX_FWD_REV_THROTTLE); // Command high value
  int turnCommand       = map(analogRead(JS_ADC_1), // Read raw joystick value
                              JS_CENTER_1-JS_DIR_1*JS_RANGE_1, // Joystick low value
                              JS_CENTER_1+JS_DIR_1*JS_RANGE_1, // Joystick high value
                              -MAX_TURN_THROTTLE, // Command low value
                              MAX_TURN_THROTTLE); // Command high value
  int verticalCommand   = map(analogRead(JS_ADC_2), // Read raw joystick value
                              JS_CENTER_2-JS_DIR_2*JS_RANGE_2, // Joystick low value
                              JS_CENTER_2+JS_DIR_2*JS_RANGE_2, // Joystick high value
                              -MAX_VERTICAL_THROTTLE, // Command low value
                              MAX_VERTICAL_THROTTLE); // Command high value

  // Combine the "stopped" command with forward, turn, and vertical and send 
  // to the ESCs.
  thrusterLeft.writeMicroseconds(CENTER_THROTTLE+forwardCommand+turnCommand);
  thrusterRight.writeMicroseconds(CENTER_THROTTLE+forwardCommand-turnCommand);
  thrusterVertical.writeMicroseconds(CENTER_THROTTLE+verticalCommand);

  // Output via serial
  Serial.print("Fwd: "); Serial.print(forwardCommand);
  Serial.print("Turn: "); Serial.print(turnCommand);
  Serial.print("Vert: "); Serial.print(verticalCommand);
  Serial.println("");

  // Delay 1/10th of a second. No need to update at super fast rates.
  delay(100);
}
