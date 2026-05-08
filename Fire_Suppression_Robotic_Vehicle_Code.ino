#include <SoftwareSerial.h>
#include <Servo.h>

// ---------------------- Pin Definitions ----------------------
SoftwareSerial BT(10, 11); // TX, RX

// Motor driver pins
const int ENA = 5;
const int ENB = 6;
const int IN1 = 7;
const int IN2 = 8;
const int IN3 = 9;
const int IN4 = 4;

// Flame Sensors and Relay
const int FLAME_LEFT = A3;
const int FLAME_CENTER = A4;
const int FLAME_RIGHT = A5;
const int RELAY_PIN = 2;
const int FIRE_THRESHOLD = 800; 

// Servo setup
const int SERVO_PIN = 3;
Servo waterServo;

// ---------------------- Global Variables ----------------------
int Speed = 180;          

// Fire/Servo variables
bool isFightingFire = false;
unsigned long lastServoMoveTime = 0;
int servoPos = 90;
int servoDirection = 2; // Smaller step (2 degrees) for smoother movement

// ---------------------- Setup ----------------------
void setup() {
  Serial.begin(9600);
  BT.begin(9600);
  
  waterServo.attach(SERVO_PIN);
  waterServo.write(90);
  
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Pump off by default
  
  pinMode(FLAME_LEFT, INPUT);
  pinMode(FLAME_CENTER, INPUT);
  pinMode(FLAME_RIGHT, INPUT);

  stopCar();
}

// ---------------------- Main Loop ----------------------
void loop() {
  checkFire();          // Check for emergencies, steer towards fire, and pump
  checkBluetooth();     // Process BT commands (works when there is no fire)
  handleServoLine();    // Sweep servo if pumping
}

// ---------------------- Core Functions ----------------------

void checkBluetooth() {
  // Only process manual commands if the robot isn't busy fighting a fire
  if (!isFightingFire) {
    while (BT.available()) {
      char cmd = BT.read();
      executeCommand(cmd);
    }
  } else {
    // Clear the Bluetooth buffer so commands don't queue up while fighting fire
    while (BT.available()) {
      BT.read();
    }
  }
}

void checkFire() {
  int fLeft = analogRead(FLAME_LEFT);
  int fCenter = analogRead(FLAME_CENTER);
  int fRight = analogRead(FLAME_RIGHT);

  // Check if ANY fire sensor detects fire
  if (fCenter < FIRE_THRESHOLD || fLeft < FIRE_THRESHOLD || fRight < FIRE_THRESHOLD) {
    isFightingFire = true;
    digitalWrite(RELAY_PIN, LOW); // Turn ON pump

    // Autonomous movement to track the fire
    if (fCenter < FIRE_THRESHOLD) {
      // Fire is directly in front: Stop moving and focus on spraying
      stopCar();
    } else if (fLeft < FIRE_THRESHOLD) {
      // Fire is to the left: Turn left to face it
      left();
    } else if (fRight < FIRE_THRESHOLD) {
      // Fire is to the right: Turn right to face it
      right();
    }
    
  } else {
    if (isFightingFire) {
      // Fire is extinguished or lost, turn off pump and center the nozzle
      digitalWrite(RELAY_PIN, HIGH); // Turn OFF pump
      waterServo.write(90);          
      stopCar();                     // Stop autonomous movement
      isFightingFire = false;
    }
  }
}

void handleServoLine() {
  if (isFightingFire) {
    // Move servo slightly every 15 milliseconds for a very smooth sweep
    if (millis() - lastServoMoveTime > 15) { 
      lastServoMoveTime = millis();
      servoPos += servoDirection;
      
      // Reverse direction at the edges (sweeps between 60 and 120 degrees)
      if (servoPos >= 120) servoDirection = -2; 
      else if (servoPos <= 60) servoDirection = 2;  
      
      waterServo.write(servoPos);
    }
  }
}

// ---------------------- Command Execution ----------------------
void executeCommand(char cmd) {
  switch (cmd) {
    case 'F': forward(); break;
    case 'B': backward(); break;
    case 'L': left(); break;
    case 'R': right(); break;
    case 'S': stopCar(); break;

    case '0': Speed = 100; break;
    case '1': Speed = 140; break;
    case '2': Speed = 153; break;
    case '3': Speed = 165; break;
    case '4': Speed = 178; break;
    case '5': Speed = 191; break;
    case '6': Speed = 204; break;
    case '7': Speed = 216; break;
    case '8': Speed = 229; break;
    case '9': Speed = 242; break;
    case 'q': Speed = 255; break;
  }
}

// ---------------------- Motor Controls ----------------------
void forward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, Speed); analogWrite(ENB, Speed);
}

void backward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, Speed); analogWrite(ENB, Speed);
}

void left() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, Speed); analogWrite(ENB, Speed);
}

void right() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, Speed); analogWrite(ENB, Speed);
}

void stopCar() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0); analogWrite(ENB, 0);
}