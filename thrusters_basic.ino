#include <Servo.h>
/*
 * 6 degrees of freedom
 * heave thrusters(hL,hR)
 * Surge Thrusters(sFL, sFR)(servo front left, servo front right)
 * Surge Thrusters 2(sBL, sBR)(servo front left, servo front right)
 * 
 *Thruster Combinations
 ** "-" here means reversing thruster polarity
 * //created with the assumption of surge thrusters being oriented in same direction, facing forward
 * forward meaning that when the thrusters are activated, they will propel the body forward
 * //created with assumption that heave thrusters are oriented facing up
 *Surge Back: (-sFL,-sBL)+(-sFR,-sBR)
 *Surge Forward: (sFL,sBL)+(sFR,sBR)
 *Heave Up: (hR,hL)
 *Heave Down: (-hR,-hL)
 *Sway Left: (sFR,-sBR)+(-sFL,sBL)
 *Sway Right: (sFL,-sBL)+(-sFR,sBR)
 *Yawing left would be turning the entire body to face left
 *Yaw Left: (sFR,sBR)+(-sFL,-sBL)
 *Yaw Right: (-sFR,-sBR)+(sFL,sBL)
 **rolling left would be tilting the left side of the body down. v.v
 *Roll Left: (-hL,hR)
 *Roll Right: (hL,-hR)
 
*/
//Create Servos
Servo hL;
Servo hR;
Servo sFL;
Servo sBL;
Servo sFR;
Servo sBR;
//Create respecitve pins
int hLp;
int hRp;
int sFLp;
int sFRp;
int sBLp;
int sBRp;

void setup() {
  hL.attach(hLp);
  hR.attach(hRp);
  sFL.attach(sFLp);
  sBL.attach(sBLp);
  sFR.attach(sFRp);
  sBR.attach(sBRp);
  
  servo.writeMicroseconds(1500); // send "stop" signal to ESC.

  delay(7000); // delay to allow the ESC to recognize the stopped signal
}

void loop() {
  int signal = 1700; // Set signal value, which should be between 1100 and 1900
  int 
  hL.writeMicroseconds(signal);
  hR.writeMicroseconds(signal);
 
  
  // Send signal to ESC.
}
