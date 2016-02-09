// Teste dos Sensores

#define SENSORES 8
int sensorPin[] = {0, 1, 2, 3, 4, 5, 6, 7};

#define MOTORES 2
int motorPin[] = {11, 10};

#define SENSORINDEX 7

void setMotores(double left, double right) {
  analogWrite(motorPin[0], left * 255);
  analogWrite(motorPin[1], right * 255);
}

void setup() {
  // put your setup code here, to run once:
  for(int i = 0; i < SENSORES; i++) {
    pinMode(sensorPin[i], INPUT);
    if(i < MOTORES) {
      pinMode(motorPin[i], OUTPUT);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(sensorPin[SENSORINDEX]) == LOW) {
    setMotores(1.00, 1.00);
  } else setMotores(0.00, 0.00);
}
