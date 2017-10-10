#include <Keypad.h>
#include <QTRSensors.h>
#include <Servo.h>

// Define Senzori linie
#define NUM_SENSORS             4  // number of sensors used
#define NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per sensor reading

QTRSensorsAnalog qtra((unsigned char[]) {0,1,2,3}, NUM_SENSORS, NUM_SAMPLES_PER_SENSOR);
unsigned int sensorVal[NUM_SENSORS];

// Define Motoare
const int motorIn1 = 22;
const int motorIn2 = 23;
const int motorIn3 = 24;
const int motorIn4 = 25;
const int speedMotorA = 2;
const int speedMotorB = 3;
const int vitezaMotoare = 140;

// Define sertar modules
const int sertarMotor1 = 34;
const int sertarMotor2 = 35;
const int speedSertarMotor = 8;
const int senzorSertar1 = 36;
const int senzorSertar2 = 37;

// Define servomotoare
Servo servoLift1, servoLift2, servoBrat1, servoBrat2;
int maximSuperiorLift = 90;
int maximSuperiorBrat = 90;

// Define tastatura matriceala
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {33, 32, 31, 30}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {29, 28, 27, 26}; //connect to the column pinouts of the keypad

Keypad tastatura = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

// Variabile

int raftCurrent = 0, raft=0, nivel=0, num = 0, valSenzorSertar1 = 0, valSenzorSertar2 = 0;
bool intoarcere = false, raftnivel = false, depunere = false;


void setup() {
  // setup pini senzori
  pinMode(senzorSertar1, INPUT);
  pinMode(senzorSertar2, INPUT);
  
  // setup pini motoare
  pinMode(motorIn1,     OUTPUT);
  pinMode(motorIn2,     OUTPUT);
  pinMode(speedMotorA,  OUTPUT);
  pinMode(motorIn3,     OUTPUT);
  pinMode(motorIn4,     OUTPUT);
  pinMode(speedMotorB,  OUTPUT);

  // initializare servomotare
  servoLift1.attach(4);
  servoLift2.attach(5);
  servoBrat1.attach(6);
  servoBrat2.attach(7);
  
  // initializare comunicare seriala spre Arduino IDE
  delay(500);
  Serial.begin(9600); // set the data rate in bits per second for serial data transmission
  delay(1000);
  
}

void moveF(){
  analogWrite(speedMotorA, vitezaMotoare);
  digitalWrite(motorIn1, HIGH);
  digitalWrite(motorIn2, LOW);
  analogWrite(speedMotorB, vitezaMotoare);
  digitalWrite(motorIn3, LOW);
  digitalWrite(motorIn4, HIGH);
  delay(10);
}

void moveB(){
  analogWrite(speedMotorA, vitezaMotoare);
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, HIGH);
  analogWrite(speedMotorB, vitezaMotoare);
  digitalWrite(motorIn3, HIGH);
  digitalWrite(motorIn4, LOW);
  delay(10);
}

void stopRobot(){
  analogWrite(speedMotorA, vitezaMotoare);
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, LOW);
  analogWrite(speedMotorB, vitezaMotoare);
  digitalWrite(motorIn3, LOW);
  digitalWrite(motorIn4, LOW);
  delay(10);
}

void moveL(){
  analogWrite(speedMotorA, vitezaMotoare);
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, HIGH);
  analogWrite(speedMotorB, vitezaMotoare);
  digitalWrite(motorIn3, LOW);
  digitalWrite(motorIn4, HIGH);
  delay(10);
}

void moveR(){
  analogWrite(speedMotorA, vitezaMotoare);
  digitalWrite(motorIn1, HIGH);
  digitalWrite(motorIn2, LOW);
  analogWrite(speedMotorB, vitezaMotoare);
  digitalWrite(motorIn3, HIGH);
  digitalWrite(motorIn4, LOW);
  delay(10);
}

void motorSertarOUT(){
  analogWrite(speedSertarMotor, vitezaMotoare);
  digitalWrite(sertarMotor1, HIGH);
  digitalWrite(sertarMotor2, LOW);
  delay(5);
}

void motorSertarIN(){
  analogWrite(speedSertarMotor, vitezaMotoare);
  digitalWrite(sertarMotor1, LOW);
  digitalWrite(sertarMotor2, HIGH);
  delay(5);
}

void motorSertarStop(){
  analogWrite(speedSertarMotor, vitezaMotoare);
  digitalWrite(sertarMotor1, LOW);
  digitalWrite(sertarMotor2, LOW);
  delay(5);
}

void ridicaLift(){
  for(int pozLift = 0; pozLift <= maximSuperiorLift; pozLift += 1)
  {
    servoLift1.write(pozLift);
    servoLift2.write(maximSuperiorLift - pozLift);
    delay(100);
  }
}

void coboaraLift(){
  for(int pozLift = 0; pozLift <= maximSuperiorLift; pozLift += 1)
  {
    servoLift1.write(maximSuperiorLift - pozLift);
    servoLift2.write(pozLift);
    delay(100);
  }
}

void ridicaBrate(){
  for(int pozBrat = 0; pozBrat <= maximSuperiorBrat; pozBrat += 1)
  {
    servoBrat1.write(pozBrat);
    servoBrat2.write(maximSuperiorBrat - pozBrat);
    delay(100);
  }
}

void coboaraBrate(){
  for(int pozBrat = 0; pozBrat <= maximSuperiorBrat; pozBrat += 1)
  {
    servoBrat1.write(maximSuperiorBrat - pozBrat);
    servoBrat2.write(pozBrat);
    delay(100);
  }
}

void impingeSertar(){
  valSenzorSertar1 = digitalRead(senzorSertar1);
  while(valSenzorSertar1 == HIGH){
    motorSertarOUT();
  }
  motorSertarStop();
}
void retrageSertar(){
  valSenzorSertar2 = digitalRead(senzorSertar2);
  while(valSenzorSertar2 == LOW){
    motorSertarIN();
  }
  motorSertarStop();
}

void verificaSenzoriCentrali(){
  if ((sensorVal[0] > 300)&&(sensorVal[1] < 300)){
    moveL();
    delay(10);
  }
  if ((sensorVal[0] < 300)&&(sensorVal[1] > 300)){
    moveR();
    delay(10);
  }
  if ((sensorVal[0] < 300)&&(sensorVal[1] < 300)){
    moveF();
    delay(10);
  }
}

void intoarceSpreRaft(){
    for(int i=0;i<=200;i+=1){
      if ((sensorVal[1] > 300)){
        moveR();
        delay(10);
      }
      else {
        moveF();
        delay(10);
      }
    }
}

void executaActiuneDeclarata(){
  if(depunere == false){
    if(nivel == 0){
      ridicaBrate();
      impingeSertar();
      coboaraBrate();
      retrageSertar();
    }
    else {
      ridicaLift();
      ridicaBrate();
      impingeSertar();
      coboaraBrate();
      retrageSertar();
      coboaraLift();
    }
  }
  else {
    if (nivel == 0){
      impingeSertar();
      ridicaBrate();
      retrageSertar();
      coboaraBrate();
    }
    else {
      ridicaLift();
      impingeSertar();
      ridicaBrate();
      retrageSertar();
      coboaraBrate();
      coboaraLift();
    }
  }
}

void pregatireRobot(){
  if(raftnivel == false){
    char key = tastatura.getKey();
      switch(key)
      {
        case NO_KEY:
          break;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        num = key - '0';
        break;

        case 'A':
        raft = num;
        break;
        
        case 'B':
        nivel = num;
        break;

        case 'C':
        depunere = true;

        case 'D':
        depunere = false;
      
        case '#':
        raftnivel = true;
      }
      key = tastatura.getKey();
  }
}

void verificaSenzoriExterni(){
  if(raftCurrent < raft){
    verificaSenzoriCentrali();
    if(sensorVal[2] > 300){
      delay(1500);
      raftCurrent +=1;
    }
  }
  else if(raftCurrent == raft && intoarcere == false){
    intoarceSpreRaft();
    intoarcere = true;
  }
  else {
    verificaSenzoriCentrali();
  }
  
  if(sensorVal[3] > 300 && sensorVal[2] > 300 & intoarcere == true)
  {
    stopRobot();
    executaActiuneDeclarata();
    intoarceSpreRaft();
    intoarcereSprePlatformaStart();
  }
}

void intoarcereSprePlatformaStart(){
    if(sensorVal[3] > 300 && sensorVal[2] > 300){
      stopRobot();
      resetare();
    }
}

void resetare(){
  intoarcere = false;
  raftnivel = false;
  raft = 0;
  raftCurrent = 0;
  nivel = 0;
  depunere = false;
  
  
}

void loop() {
  if(raftnivel == false){
    pregatireRobot();
  }
  qtra.read(sensorVal);
  verificaSenzoriExterni();


}
