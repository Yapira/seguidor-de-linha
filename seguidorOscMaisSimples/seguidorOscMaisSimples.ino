#define SENSOR_CENTRAL 2
#define SENSOR_MARCA   7
#define SENSOR_ESQ     1
#define SENSOR_DIR     6

#define MOTOR_LEFT    11
#define MOTOR_RIGHT   10

#define SPEED_LIMIT 0.03110

#define NIVEL_NA_FAIXA LOW

#define DIR_ESQUERDA 1
#define DIR_DIREITA 2
int direcaoOsc = DIR_ESQUERDA;
bool isOnTrack = true;

void setMotores(double left, double right) {
  analogWrite(MOTOR_LEFT, left * SPEED_LIMIT * 255);
  analogWrite(MOTOR_RIGHT, right * SPEED_LIMIT * 255);
}

void setup() {
  pinMode(SENSOR_CENTRAL, INPUT);
  pinMode(SENSOR_MARCA, INPUT);
  pinMode(SENSOR_ESQ, INPUT);
  pinMode(SENSOR_DIR, INPUT);
  pinMode(MOTOR_LEFT, OUTPUT);
  pinMode(MOTOR_RIGHT, OUTPUT);
  setMotores(10.00, 0.10);
}

void loop() {
  bool leituraCentral = (digitalRead(SENSOR_CENTRAL) == NIVEL_NA_FAIXA);
  bool leituraMarca = (digitalRead(SENSOR_MARCA) == NIVEL_NA_FAIXA);
  // pendente:
  //   cruzamento
  //   calibracao (?) 
  //   parar na chegada!
  if (!leituraCentral && isOnTrack) {
    isOnTrack = false;
    switch (direcaoOsc) {
      case DIR_ESQUERDA:
        setMotores(0.1 , 10.00);
        direcaoOsc = DIR_DIREITA;
        break;
      case DIR_DIREITA:
        setMotores(10.00, 0.1);
//        setMotores(0.00, 0.00);
        direcaoOsc = DIR_ESQUERDA;
        break;
    }
  }
  if(leituraCentral && !isOnTrack) isOnTrack = true;
//  setMotores(1.00, 1.00);
}
