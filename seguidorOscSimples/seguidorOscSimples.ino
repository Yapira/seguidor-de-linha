#define SENSOR_ESQU 1
#define SENSOR_CENT 2
#define SENSOR_DIRE 6
#define SENSOR_MARC 7

#define MOTOR_LEFT  11
#define MOTOR_RIGHT 10

//     x x
// x x     x x
//     x x

#define SPEED_LIMIT 1.00

#define NIVEL_NA_FAIXA LOW

void setMotores(double left, double right) {
  analogWrite(MOTOR_LEFT, left * SPEED_LIMIT);
  analogWrite(MOTOR_RIGHT, right * SPEED_LIMIT);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(SENSOR_ESQU, INPUT);
  pinMode(SENSOR_CENT, INPUT);
  pinMode(SENSOR_DIRE, INPUT);
  pinMode(SENSOR_MARC, INPUT);
  pinMode(MOTOR_LEFT, OUTPUT);
  pinMode(MOTOR_RIGHT, OUTPUT);
  setMotores(1.00, 1.00);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool leituraEsquerda = (digitalRead(SENSOR_ESQU) == NIVEL_NA_FAIXA);
  bool leituraCentro = (digitalRead(SENSOR_CENT) == NIVEL_NA_FAIXA);
  bool leituraDireita = (digitalRead(SENSOR_DIRE) == NIVEL_NA_FAIXA);
  bool leituraMarca = (digitalRead(SENSOR_MARC) == NIVEL_NA_FAIXA);
//  if(!leituraCentro) {
//  } else if(leituraEsquerda) {
//    setMotores(2.00, 0.25);
//  } else if(leituraDireita) {
//    setMotores(0.25, 2.00);
//  }
}
