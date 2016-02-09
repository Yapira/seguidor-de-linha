// "Pau mandado"
// Autor: Karine Nacano, Bernardo "Pedra" Mendonca, Golden Boy (Desconheco) e Equipe Yapira.
#define SENSORES 8
int sensorPins[] = {0, 1, 2, 3, 4, 5, 6, 7};

#define MOTORES 2
int motorPins[] = {10, 11};

#define MAX_SPEED 0.20

#define OSC_ESQUERDA 1
#define OSC_DIREITA 2
int direcaoOscilacao = OSC_ESQUERDA;

#define NIVEL_NA_FAIXA LOW

#define SPEED_AT_RETA    0.25
#define SPEED_AT_CURVE  0.125
#define SPEED_AT_S_CURVE 0.05

long mediaOsc, duracaoOscAtual, oscCount; // Duracao em ms da primeira oscilacao
bool isFirstOsc = true;

void setMotores(double left, double right) {
  analogWrite(motorPins[0], right * MAX_SPEED * 255);
  analogWrite(motorPins[1], left * MAX_SPEED * 255);
}

#define DIR_S_ESQUERDA  -2
#define DIR_ESQUERDA    -1
#define DIR_CENTRO       0
#define DIR_DIREITA      1
#define DIR_S_DIREITA    2
int getAlign(int *leituras) {
  //  if (leituras[sensorPins[1]] == NIVEL_NA_FAIXA) return DIR_DIREITA;
  //  if (leituras[sensorPins[6]] == NIVEL_NA_FAIXA) return DIR_ESQUERDA;
  //  if (leituras[sensorPins[2]] != NIVEL_NA_FAIXA) return DIR_S_ESQUERDA; // Sensor esquerdo fora da faixa
  //  if (leituras[sensorPins[5]] != NIVEL_NA_FAIXA) return DIR_S_DIREITA;
  if (leituras[sensorPins[2]] != NIVEL_NA_FAIXA) return DIR_ESQUERDA; // Sensor esquerdo fora da faixa
  if (leituras[sensorPins[5]] != NIVEL_NA_FAIXA) return DIR_DIREITA;
  return DIR_CENTRO;
}

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < SENSORES; i++) {
    pinMode(sensorPins[i], INPUT);
    if (i < MOTORES) pinMode(motorPins[i], OUTPUT);
  }
  //pinMode(13, OUTPUT); // Led catodo
  //pinMode(12, OUTPUT); // Led anodo (gnd)
  //digitalWrite(12, LOW);
  duracaoOscAtual = micros(); // Pedra Senpai wa hidoi desu S2
  mediaOsc = oscCount = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  int leituras[SENSORES];
  bool canUpdateMotor = true; // Ainda pode escrever no motor nesse ciclo de loop? (para evitar 2 comandos no mesmo loop())
  for (int i = 0; i < SENSORES; i++) {
    leituras[i] = digitalRead(sensorPins[i]);
  }
  int align;
  switch (direcaoOscilacao) {
    case OSC_ESQUERDA:
      //      if (mediaOsc != 0 && (align != DIR_S_ESQUERDA || align != DIR_ESQUERDA) && micros() - duracaoOscAtual > mediaOsc) {
      //        setMotores(1.00, 2.00);
      //      } else {
      switch (align) {
        case DIR_S_DIREITA:
          setMotores(SPEED_AT_CURVE, 1.0);
          break;
        case DIR_DIREITA:
          setMotores(SPEED_AT_S_CURVE, 0.75);
          break;
        case DIR_CENTRO:
          setMotores(SPEED_AT_RETA, 0.5);
          break;
        case DIR_ESQUERDA:
        case DIR_S_ESQUERDA:
          direcaoOscilacao = OSC_DIREITA;
          //          duracaoOscAtual -= micros(); // duracaoOscAtual = duracaoOscAtual - micros();
          //          if (isFirstOsc) {
          //            duracaoOscAtual *= 2;
          //            isFirstOsc = false;
          //          }
          //          oscCount++;
          //          mediaOsc = (oscCount - 1) * mediaOsc / oscCount + duracaoOscAtual / oscCount;
          //          duracaoOscAtual = micros();
          break;
      }
      //      }
      break;
    case OSC_DIREITA:
      //b      if (mediaOsc != 0 && (align != DIR_S_ESQUERDA || align != DIR_ESQUERDA) && micros() - duracaoOscAtual > mediaOsc) {
      //        setMotores(1.00, 2.00);
      //      } else {
      switch (align) {
        //          if(micros() - duracaoOscAtual > mediaOsc) {
        //            setMotores(1.00, 2.00);
        //            canUpdateMotor = false;
        //          }
        case DIR_S_ESQUERDA:
          setMotores(0.5, SPEED_AT_CURVE);
          break;
        case DIR_ESQUERDA:
          setMotores(0.75, SPEED_AT_S_CURVE);
          break;
        case DIR_CENTRO:
          setMotores(1.0, SPEED_AT_RETA);
          break;
        case DIR_DIREITA:
        case DIR_S_DIREITA:
          direcaoOscilacao = OSC_ESQUERDA;
          //            duracaoOscAtual -= micros(); // duracaoOscAtual = duracaoOscAtual - micros();
          //            if (isFirstOsc) {
          //              duracaoOscAtual *= 2; // duracaoOscAtual = duracaoOscAtual * 2;
          //              isFirstOsc = false;
          //            }
          //            oscCount++;
          //            mediaOsc = (oscCount - 1) * mediaOsc / oscCount + duracaoOscAtual / oscCount;
          //            duracaoOscAtual = micros();
          break;
      }
      break;
    default:
      break;
      //        }
  }
}
