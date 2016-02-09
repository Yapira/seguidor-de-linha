#define SENSORES 8
int sensorPin[SENSORES] = {0, 1, 2, 3, 4, 5, 6, 7};

#define LEFT_WING_SENSOR 1
#define LEFT_FRONTAL_SENSOR 2
#define RIGHT_FRONTAL_SENSOR 5
#define LEFT_BACK_SENSOR 3
#define RIGHT_BACK_SENSOR 4
#define RIGHT_WING_SENSOR 6
#define FINISH_SEN_SENSOR 7

#define MOTORES 2
int motorPin[MOTORES] = {10, 11};

#define ledPin 13

#define SPEED_LIMIT 0.20

#define NORMAL_CURVE_ACCELERATION 0.5
#define ACCEN_CURVE_ACCELERATION 0.25
#define FUCKING_CURVE_ACCELERATION 0.00

#define SENSOR_ON_FAIXA LOW
bool isRobotWorking = true;

void setMotores(double left, double right) {
  analogWrite(motorPin[0], SPEED_LIMIT * left * 255);
  analogWrite(motorPin[1], SPEED_LIMIT * right * 255);
}

void setup() {
  // put your setup code here, to run once:
  for(int i = 0; i < SENSORES; i++) {
    pinMode(sensorPin[i], INPUT);
    if(i < MOTORES) pinMode(motorPin[i], OUTPUT);
  }
  pinMode(ledPin, OUTPUT);
  delay(5000);
}

void loop() {
  // put your main code here, to run repeatedly:
  int leituras[SENSORES];
  if(isRobotWorking) {
    isRobotWorking = false;
    for(int i = 0; i < SENSORES; i++) {
      leituras[i] = digitalRead(sensorPin[i]);
      if(leituras[i] == SENSOR_ON_FAIXA && i != 0 && i!= sensorPin[FINISH_SEN_SENSOR]) isRobotWorking = true;
    }
  }
  if(isRobotWorking) {
    if(leituras[sensorPin[LEFT_FRONTAL_SENSOR]] != SENSOR_ON_FAIXA && leituras[sensorPin[RIGHT_FRONTAL_SENSOR]] != SENSOR_ON_FAIXA) { // Dois sensores frontais fora da faixa, curva forte (?)
      if(leituras[sensorPin[LEFT_BACK_SENSOR]] == SENSOR_ON_FAIXA) { // Sensor esquerdo na laterna
        setMotores(FUCKING_CURVE_ACCELERATION, 1.00);
      } else if(leituras[sensorPin[LEFT_BACK_SENSOR]] == SENSOR_ON_FAIXA) {
        setMotores(1.00, FUCKING_CURVE_ACCELERATION);
      } else isRobotWorking = false; // Robo perdido
    } else if(leituras[sensorPin[LEFT_FRONTAL_SENSOR]] != SENSOR_ON_FAIXA) {
      if(leituras[sensorPin[LEFT_BACK_SENSOR]] != SENSOR_ON_FAIXA) { 
        setMotores(ACCEN_CURVE_ACCELERATION, 1.00);
      } else {
        setMotores(NORMAL_CURVE_ACCELERATION, 1.00);
      }
    } else if(leituras[sensorPin[RIGHT_FRONTAL_SENSOR]] != SENSOR_ON_FAIXA) {
      if(leituras[sensorPin[RIGHT_BACK_SENSOR]] != SENSOR_ON_FAIXA) { 
        setMotores(1.00, ACCEN_CURVE_ACCELERATION);
      } else {
        setMotores(1.00, NORMAL_CURVE_ACCELERATION);
      }
    } else if(leituras[sensorPin[LEFT_BACK_SENSOR]] != SENSOR_ON_FAIXA) {
      setMotores(1.00, NORMAL_CURVE_ACCELERATION);
    } else if(leituras[sensorPin[RIGHT_BACK_SENSOR]] != SENSOR_ON_FAIXA) {
      setMotores(NORMAL_CURVE_ACCELERATION, 1.00);
    } else setMotores(1.00, 1.00);
  }
}
