#include <Servo.h>
#include <Process.h>
#include <PID_v1.h>
#include <ICM_20948.h>

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
int hLp=37;
int hRp=43;
int sFLp=35;
int sFRp=41;
int sBLp=39;
int sBRp=45;

//Create PID
double Setpoint, Input, Output;
double Kp=2, Ki=2, Kd=2;
PID thePID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void setup() {
  //create python crossover

  //create serial port, 9600bps
  Serial.begin(19200);
  int pwm = 0;
  //attach pin ints to servos
  hL.attach(hLp);
  hR.attach(hRp);
  sFL.attach(sFLp);
  sBL.attach(sBLp);
  sFR.attach(sFRp);
  sBR.attach(sBRp);
  //Initialize all thrusters
  hL.writeMicroseconds(1500); 
  hR.writeMicroseconds(1500);
  sFL.writeMicroseconds(1500);
  sFR.writeMicroseconds(1500);
  sBL.writeMicroseconds(1500);
  sBR.writeMicroseconds(1500);
  //delay for ESC recognization
  delay(7500);
  Serial.println("ready");
  //delay to allow the ESC to recognize the stopped signal
}

void loop() {
  String str = "  ";
  int pwm=0;
  /*Commands are: 
   * Y-yaw, R-roll, S-sway, SS-surge, H-heave
   * combined with
   * L-left, R-right, F-forward, B-back,U-up, D-down
   * command format is "direction-thrust"
   * thrust is rated from 01-05, 05 being most powerful
   * e.g LR-02
  */
  String previous="";
   while(Serial.available()){
      Serial.println("command available");
      //isolate direction string
      str=Serial.readStringUntil('-');
      Serial.println(str);
      //isolate thrust int
      pwm=Serial.readString().toInt();
      Serial.println("pwm ");
      //output command
      Serial.print(str);
      Serial.print(" at pwm: ");
      Serial.print(pwm);
      Serial.println();
      /*max reverse pwm is 1150, max forward pwm is 1850, deadband is at 1500*/
      /*range is 350 each way, so increments of 70 every time*/
      //for now, assign each direction a number for switch statement(wont matter for xbox)
      //LY-1 RY-2 LR-3 RR-4 LS-5 RS-6 FSS-7 BSS-8 UH-9 DH-10
      if(str=="LY"){
        //Yaw Left: (sFR,sBR)+(-sFL,-sBL)
        sFR.writeMicroseconds(1500 + (pwm * 35));
        sBR.writeMicroseconds(1500 + (pwm * 35));
        sFL.writeMicroseconds(1500 - (pwm * 35));
        sBL.writeMicroseconds(1500 - (pwm * 35));
        delay(500);
        sFR.writeMicroseconds(1500);
        sBR.writeMicroseconds(1500);
        sFL.writeMicroseconds(1500);
        sBL.writeMicroseconds(1500);
        Serial.print("yaw left");   
       }
      else if(str=="RY"){
        //Yaw Right: (-sFR,-sBR)+(sFL,sBL)
        sFR.writeMicroseconds(1500 - (pwm * 35));
        sBR.writeMicroseconds(1500 - (pwm * 35));
        sFL.writeMicroseconds(1500 + (pwm * 35));
        sBL.writeMicroseconds(1500 + (pwm * 35));
        delay(500);
        sFR.writeMicroseconds(1500);
        sBR.writeMicroseconds(1500);
        sFL.writeMicroseconds(1500);
        sBL.writeMicroseconds(1500);
        Serial.print("yaw right");  
        }
      else if(str=="RL"){
        //Roll Left: (-hL,hR)
        hL.writeMicroseconds(1500 - (pwm * 70));
        hR.writeMicroseconds(1500 + (pwm * 70));
        delay(500);
        hR.writeMicroseconds(1500);
        hL.writeMicroseconds(1500);
        Serial.print("roll left");  
        }
      else if(str=="RR"){
        //Roll Right: (hL,-hR)
        hL.writeMicroseconds(1500 + (pwm * 70));
        hR.writeMicroseconds(1500 - (pwm * 70));
        delay(500);
        hR.writeMicroseconds(1500);
        hL.writeMicroseconds(1500);
        Serial.print("roll right");  
        }
      else if(str=="LS"){
        //Sway Left: (sFR,-sBR)+(-sFL,sBL)
        sFR.writeMicroseconds(1500 + (pwm * 35));
        sBR.writeMicroseconds(1500 - (pwm * 35));
        sFL.writeMicroseconds(1500 - (pwm * 35));
        sBL.writeMicroseconds(1500 + (pwm * 35));
        delay(500);
        sFR.writeMicroseconds(1500);
        sBR.writeMicroseconds(1500);
        sFL.writeMicroseconds(1500);
        sBL.writeMicroseconds(1500);
        Serial.print("sway left");  
        }
      else if(str=="RS"){
        //Sway Right: (sFL,-sBL)+(-sFR,sBR)
        sFR.writeMicroseconds(1500 - (pwm * 35));
        sBR.writeMicroseconds(1500 + (pwm * 35));
        sFL.writeMicroseconds(1500 + (pwm * 35));
        sBL.writeMicroseconds(1500 - (pwm * 35));
        delay(500);
        sFR.writeMicroseconds(1500);
        sBR.writeMicroseconds(1500);
        sFL.writeMicroseconds(1500);
        sBL.writeMicroseconds(1500);
        Serial.print("sway right");  
        }
      else if(str=="FSS"){
        //Surge Forward: (sFL,sBL)+(sFR,sBR)
        sFR.writeMicroseconds(1500 + (pwm * 35));
        sBR.writeMicroseconds(1500 + (pwm * 35));
        sFL.writeMicroseconds(1500 + (pwm * 35));
        sBL.writeMicroseconds(1500 + (pwm * 35));
        delay(500);
        sFR.writeMicroseconds(1500);
        sBR.writeMicroseconds(1500);
        sFL.writeMicroseconds(1500);
        sBL.writeMicroseconds(1500);
        Serial.print("surge forward");  
        }
      else if(str=="BSS"){
        //Surge Back: (-sFL,-sBL)+(-sFR,-sBR)
        sFR.writeMicroseconds(1500 - (pwm * 35));
        sBR.writeMicroseconds(1500 - (pwm * 35));
        sFL.writeMicroseconds(1500 - (pwm * 35));
        sBL.writeMicroseconds(1500 - (pwm * 35));
        delay(500);
        sFR.writeMicroseconds(1500);
        sBR.writeMicroseconds(1500);
        sFL.writeMicroseconds(1500);
        sBL.writeMicroseconds(1500);
        Serial.print("surge back"); 
        }
      else if(str=="UH"){
        //Heave Up: (hR,hL)
        hR.writeMicroseconds(1500 + (pwm * 70));
        hL.writeMicroseconds(1500 + (pwm * 70));
        delay(500);
        hR.writeMicroseconds(1500);
        hL.writeMicroseconds(1500);
        Serial.print("heave up"); 
        }
      else if(str=="DH"){
        //Heave Down: (-hR,-hL)
        hR.writeMicroseconds(1500 - (pwm * 70));
        hL.writeMicroseconds(1500 - (pwm * 70));
        delay(500);
        hR.writeMicroseconds(1500);
        hL.writeMicroseconds(1500);
        Serial.print("heave down");
        }
      previous=str;
      //^convert to switch statement after xbox integrate
    }
}
