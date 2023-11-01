#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Servo.h>

#define DIR1 2
#define STEP1 3
#define DIR2 4
#define STEP2 5
#define SERVO 6

#define SWITCH1 7
#define SWITCH2 8

AccelStepper stepper1 = AccelStepper(1, STEP1, DIR1);
AccelStepper stepper2 = AccelStepper(1, STEP2, DIR2);
Servo servo;

int speed = 800;

void setup() {
  // put your setup code here, to run once:
  pinMode(SERVO, OUTPUT);
  pinMode(SWITCH1, INPUT_PULLUP);
  pinMode(SWITCH2, INPUT_PULLUP);

  servo.attach(SERVO);
  servo.write(90);
  
  stepper1.setMaxSpeed(1000);
  stepper2.setMaxSpeed(1000);
  
  Serial.begin(9600);
  reset();
  Serial.write(1);
}


void loop() {
  int command;
  while(!Serial.available()) {}
  command = Serial.read();
  if(command == 1) {
    long arr[2];
    while(!Serial.available()) {}
    int t1 = Serial.read();
    while(!Serial.available()) {}
    int t2 = Serial.read();
    while(!Serial.available()) {}
    int t3 = Serial.read();
    while(!Serial.available()) {}
    int t4 = Serial.read();

    arr[0] = -(t1 * 256 + t2);
    arr[1] = t3 * 256 + t4;

    stepper1.setSpeed(stepper1.currentPosition()>arr[0]?-speed:speed);
  
    while(stepper1.currentPosition() != arr[0]) {
      stepper1.runSpeed();
    }

    stepper2.setSpeed(stepper2.currentPosition()<arr[1]?speed:-speed);
  
    while(stepper2.currentPosition() != arr[1]) {
      stepper2.runSpeed();
    }
    
    Serial.write(1);
  } else if(command == 2) {
    while(!Serial.available()) {}
    bool is_up = Serial.read();
    if(!is_up) {
        for(int i=155; i>=90; i-=5) {
            servo.write(i);
            delay(75);
        }
    } else {
        for(int i=90; i<=155; i+=5) {
            servo.write(i);
            delay(75);
        }
    }
    delay(1000);
    Serial.write(1);
  } else if(command == 3) {
    reset();
    Serial.write(1);
  }
  delay(10);
}

void reset() {    

  stepper1.setSpeed(800);

  while(digitalRead(SWITCH1)) {
    stepper1.runSpeed();
  }
  
  stepper1.setCurrentPosition(0);
  stepper1.setSpeed(-800);
  
  while(stepper1.currentPosition() != -200) {
    stepper1.runSpeed();
  }
  stepper1.setCurrentPosition(0);
  
  stepper2.setSpeed(-800);

  while(digitalRead(SWITCH2)) {
    stepper2.runSpeed();
  }

  stepper2.setCurrentPosition(0);
  stepper2.setSpeed(800);
  while(stepper2.currentPosition() != 200) {
    stepper2.runSpeed();
  }
  stepper2.setCurrentPosition(0);

}
