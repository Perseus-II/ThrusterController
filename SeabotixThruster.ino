#include <Servo.h>

// #define DEBUG 1

#define SURGE_PORT_THRUSTER (11)
#define SURGE_STARBOARD_THRUSTER (3)
#define HEAVE_A_THRUSTER (9)
#define HEAVE_B_THRUSTER (10)

/* Current states for thrusters */
float sp = 90.0;
float ss = 90.0;
float ha = 90.0;
float hb = 90.0;

/* Desired states for thrusters */
float sp_d = 90.0; 
float ss_d = 90.0;
float ha_d = 90.0;
float hb_d = 90.0;

/* Error states */
float sp_e = 0.0;
float ss_e = 0.0;
float ha_e = 0.0;
float hb_e = 0.0;

Servo sp_servo;
Servo ss_servo;
Servo ha_servo;
Servo hb_servo;

float K_p = 0.2; /* proportional gain */
float E_thresh = .3; /* error threshold */

void setup() {
  Serial.begin(115200); 
  Serial.setTimeout(20);

  sp_servo.attach(SURGE_PORT_THRUSTER);
  ss_servo.attach(SURGE_STARBOARD_THRUSTER);
  ha_servo.attach(HEAVE_A_THRUSTER);
  hb_servo.attach(HEAVE_B_THRUSTER);

  delay(100);

}

void loop() {
  if(Serial.available() != 0) {
  
  /* Read in the comma-separated serial data */
  sp_d             =   Serial.parseInt();
  ss_d             =   Serial.parseInt();
  ha_d             =   Serial.parseInt();
  hb_d             =   Serial.parseInt();
  

  sp_d             =   float(map(sp_d, -100, 100, 0, 180));
  ss_d             =   float(map(ss_d, -100, 100, 0, 180));
  ha_d             =   float(map(ha_d, -100, 100, 0, 180));
  hb_d             =   float(map(hb_d, -100, 100, 0, 180));
  
  }

  sp_e = sp_d - sp;
  ss_e = ss_d - ss;  
  ha_e = ha_d - ha;
  hb_e = hb_d - hb;
  
  if(abs(sp_e) > E_thresh)
    sp = sp + K_p * sp_e;
  if(abs(ss_e) > E_thresh)
    ss = ss + K_p * ss_e;
  if(abs(ha_e) > E_thresh)
    ha = ha + K_p * ha_e;
  if(abs(hb_e) > E_thresh)
    hb = hb + K_p * hb_e;
 
  
  /* update thrusters to new position */
  sp_servo.write(int(sp));
  ss_servo.write(int(ss));
  ha_servo.write(int(ha));
  hb_servo.write(int(hb));
  
  
  #ifdef DEBUG
  Serial.print(sp);
  Serial.print("->");
  Serial.print(sp_d);
  Serial.print("(");
  Serial.print(sp_e);
  Serial.print(")");

  Serial.print(" | ");
  
  Serial.print(ss);
  Serial.print("->");
  Serial.print(ss_d);
  Serial.print("(");
  Serial.print(ss_e);
  Serial.print(")");
  
  Serial.print(" | ");
  
  Serial.print(ha);
  Serial.print("->");
  Serial.print(ha_d);
  Serial.print("(");
  Serial.print(ha_e);
  Serial.print(")");
  
  Serial.print(" | ");
  
  Serial.print(hb);
  Serial.print("->");
  Serial.print(hb_d);
  Serial.print("(");
  Serial.print(hb_e);
  Serial.print(")");
  
  Serial.println();
  
  #endif
  
  delay(40);  
  
}

