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
     back_right.attach(45); //4 and  46
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
     claw.write(90);
     delay(7000);
     Serial.println("ready");
 }

 void loop() {
   while (Serial.available()){
    String input = Serial.readStringUntil('x');
    int dir = input.substring(0, 1).toInt();
    int thruster_value = input.substring(2, input.length() - 1).toFloat() * 230;
    int claw_value = input.substring(2, input.length() - 1).toFloat() * 85;
   
    if (dir == 2 or dir == 5){
      Serial.println("direction " + String(dir) + " claw_value " + String(claw_value));
    }
    else{  
      Serial.println("direction "+ String(dir) + " thruster_value "+String(thruster_value));
    }
    //surge sway
    if (dir == 0){
    //  Serial.println("writing values to FRH, BRH, FLH, BLH");
      front_right.writeMicroseconds(1500);
      back_right.writeMicroseconds(1500);
      front_left.writeMicroseconds(1500);
      back_left.writeMicroseconds(1500);
      
      front_right.writeMicroseconds(thruster_value + 1500);
      back_right.writeMicroseconds(thruster_value + 1500);
      front_left.writeMicroseconds(-1 * thruster_value + 1500);
      back_left.writeMicroseconds(-1 * thruster_value + 1500);
      
    }
    //surge 
    else if (dir == 1){
      front_right.writeMicroseconds(1500);
      back_right.writeMicroseconds(1500);
      front_left.writeMicroseconds(1500);
      back_left.writeMicroseconds(1500);
      
      front_right.writeMicroseconds(thruster_value + 1500);
      front_left.writeMicroseconds(thruster_value + 1500);
      back_right.writeMicroseconds(thruster_value * -1 + 1500);
      back_left.writeMicroseconds(thruster_value * -1 + 1500);
    }
    //claw open
    //else if (dir == 2){
    //  claw.write(90);
      
    //  claw.write(claw_value * -1 + 90);
    //}
    //heave
    else if (dir == 3){
      mid_right.writeMicroseconds(1500);
      mid_left.writeMicroseconds(1500);    
      mid_right.writeMicroseconds(thruster_value + 1500);
      mid_left.writeMicroseconds(thruster_value + 1500);

    }
    //yaw
    else if (dir == 4){
      front_right.writeMicroseconds(1500);
      back_right.writeMicroseconds(1500);
      front_left.writeMicroseconds(1500);
      back_left.writeMicroseconds(1500);
      
      front_right.writeMicroseconds(thruster_value + 1500);
      back_right.writeMicroseconds(thruster_value + 1500);
      front_left.writeMicroseconds(thruster_value * -1 + 1500);
      back_left.writeMicroseconds(thruster_value * -1 + 1500);
    }
    //claw open and close
    else if (dir == 5){
      claw.write(90);
      
      claw.write(claw_value * -1 + 90);
    }
   }   
 }
 
