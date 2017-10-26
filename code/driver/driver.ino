#include <Servo.h>

//stepper pins
#define stp 2
#define dir 3
#define MS1 4
#define MS2 5
#define MS3 6
#define EN  7
//driving pins
#define L_ARM_PIN 11
#define R_ARM_PIN 12
#define FORWARD_PIN 13
#define BACKWARD_PIN 14
//sensor pins
#define DRIVING_SWITCH 15
#define TILT_SWITCH 16
#define L_ARM_SWITCH 17
#define R_ARM_SWITCH 18

Servo l_arm;
Servo r_arm;

int location = 0;

void lift(int pour_time) {
  if (pour_time == 0) {
    return; 
  }
  
  //close the arms
  l_arm.write(0);
  r_arm.write(180);
  delay(1000);
  l_arm.write(90);
  r_arm.write(90);
  //lift the arm
  digitalWrite(dir, LOW);
  for (int x=0; x<1000; x++)  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
  delay(pour_time);
  //lower the arm
  digitalWrite(dir, HIGH);
  while (digitalRead(TILT_SWITCH == LOW))  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
  //open the arms
  l_arm.write(180);
  r_arm.write(0);
  while ((digitalRead(L_ARM_SWITCH == LOW)) or (digitalRead(R_ARM_SWITCH == LOW))) {
    if (digitalRead(L_ARM_SWITCH == HIGH)) {
      l_arm.write(90);
    }
    if (digitalRead(R_ARM_SWITCH == HIGH)) {
      r_arm.write(90);
    }
    delay(1);
  }
  l_arm.write(90);
  r_arm.write(90);
  //bump the bottle
  digitalWrite(dir, LOW);
  for (int x=0; x<50; x++)  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
  //lower the arm
  digitalWrite(dir, HIGH);
  while (digitalRead(TILT_SWITCH == LOW))  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
}

void goto_bottle(int target_location) {
  int vel = 1;
  int old_state = digitalRead(DRIVING_SWITCH);
  int new_state = digitalRead(DRIVING_SWITCH);
  
  while (location != target_location) {
    if (location > target_location) {
      vel = 1;
      digitalWrite(BACKWARD_PIN, LOW);
      digitalWrite(FORWARD_PIN, HIGH);
    }
    else {
      vel = -1;
      digitalWrite(FORWARD_PIN, LOW);
      digitalWrite(BACKWARD_PIN, HIGH);
    }
    old_state = new_state;
    new_state = digitalRead(DRIVING_SWITCH);
    if ((old_state == LOW) and (new_state == HIGH)) {
      location = location + vel; 
    }
  }
  digitalWrite(FORWARD_PIN, LOW);
  digitalWrite(BACKWARD_PIN, LOW);
}

void resetBEDPins()
{
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(MS3, LOW);
  digitalWrite(EN, HIGH);
}

void setup() {
  // setting up the stepper
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  pinMode(EN, OUTPUT);
  resetBEDPins();

  //setting up driving pins
  pinMode(FORWARD_PIN, OUTPUT);
  digitalWrite(FORWARD_PIN, LOW);
  pinMode(BACKWARD_PIN, OUTPUT);
  digitalWrite(BACKWARD_PIN, LOW);
  l_arm.attach(L_ARM_PIN);
  l_arm.write(90);
  r_arm.attach(R_ARM_PIN);
  r_arm.write(90);

  //setting up the sensors
  pinMode(DRIVING_SWITCH, INPUT);
  pinMode(TILT_SWITCH, INPUT);
  pinMode(L_ARM_SWITCH, INPUT);
  pinMode(R_ARM_SWITCH, INPUT);

  Serial.begin(9600);
}

void loop() {
  // wait and construct command list
  int commands[] = {0,0,0,0,0,0,0,0,0,0};
  int inc = 0;
  while (Serial.available() < 10) {
    delay(1);
  }
  if (Serial.available()>=10) {
    for (int i=0; i<10; i++) {
      inc = Serial.read();
      inc = inc-90;
      commands[i] = inc;
    }
  }
  int i = 0;
  while (i<10) {
    goto_bottle(commands[i]);
    i = i+1;
    lift(commands[i]*1000);
    i = i+1;
  }
}
