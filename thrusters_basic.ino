#include<Servo.h>
 //2, 3, 4, 5, 6, 7, 46

 Servo front_right;
 Servo front_left;
 Servo back_right;
 Servo back_left;
 Servo mid_right;
 Servo mid_left;
 Servo claw;
 
 void setup() {
   // put your setup code here, to run once:
     Serial.begin(9600);  
     Serial.println("RESET");
     front_right.attach(41);
     front_left.attach(35);
     back_right.attach(45);
     back_left.attach(39);
     mid_right.attach(43);
     mid_left.attach(37);
     claw.attach(12);
     front_right.writeMicroseconds(1500);
     front_left.writeMicroseconds(1500);
     back_right.writeMicroseconds(1500);
     back_left.writeMicroseconds(1500);
     mid_right.writeMicroseconds(1500);
     mid_left.writeMicroseconds(1500);
     claw.write(20);
     delay(3000);
     Serial.println("ready");
 }

 void loop() {
   while (Serial.available()){
    String input = Serial.readStringUntil('x');
    int dir = input.substring(0, 1).toInt();
    int thruster_value = input.substring(2, input.length() - 1).toFloat() * -180;
    int claw_value = input.substring(2, input.length() - 1).toFloat() * 145;
   
    if (dir == 5){
      Serial.println("direction " + String(dir) + " claw_value " + String(claw_value));
    }
    else if (dir == 1 || dir == 3 || dir == 4){  
      Serial.println("direction "+ String(dir) + " thruster_value "+String(thruster_value));
    }
    //sway
    if (dir == 0){
      front_right.writeMicroseconds(1500);
      back_right.writeMicroseconds(1500);
      front_left.writeMicroseconds(1500);
      back_left.writeMicroseconds(1500);
      
      front_right.writeMicroseconds(-1 * thruster_value * 0.8 + 1500);
      back_right.writeMicroseconds(thruster_value * 0.8 + 1500);
      front_left.writeMicroseconds(thruster_value * 0.8 + 1500);
      back_left.writeMicroseconds(-1 * thruster_value * 0.8 + 1500);
      
    }
    //surge 
    else if (dir == 1){
      front_right.writeMicroseconds(1500);
      back_right.writeMicroseconds(1500);
      front_left.writeMicroseconds(1500);
      back_left.writeMicroseconds(1500);
      
      front_right.writeMicroseconds(thruster_value * 0.8 * -1 + 1500);
      front_left.writeMicroseconds(thruster_value * 0.8 * -1 + 1500);
      back_right.writeMicroseconds(thruster_value * 0.8 * -1 + 1500);
      back_left.writeMicroseconds(thruster_value * 0.8 * -1 + 1500);
    }
    
    //yaw
    else if (dir == 3){
      front_right.writeMicroseconds(1500);
      back_right.writeMicroseconds(1500);
      front_left.writeMicroseconds(1500);
      back_left.writeMicroseconds(1500);
      
      front_right.writeMicroseconds(thruster_value * 0.8 * -1 + 1500);
      back_right.writeMicroseconds(thruster_value * 0.8 * -1 + 1500);
      front_left.writeMicroseconds(thruster_value * 0.8 + 1500);
      back_left.writeMicroseconds(thruster_value * 0.8 + 1500);
    }
    //heave
    else if (dir == 4){
      mid_right.writeMicroseconds(1500);
      mid_left.writeMicroseconds(1500);    
      mid_right.writeMicroseconds(thruster_value + 1500);
      mid_left.writeMicroseconds(thruster_value * -1 + 1500);

    }
    //claw open and close
    else if (dir == 5){
      claw.write(20);
      
      claw.write(claw_value + 20);
    }
   }   
 }
 
