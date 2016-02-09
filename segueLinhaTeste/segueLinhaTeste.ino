#define SENSORES 8
int sensores[] = {0, 1, 2, 3, 4, 5, 6, 7};
#define MOTORES 2
int motores[] = {11,10};

#define MAX_SPEED 1.00

void setup() {
  // put your setup code here, to run once:
  for(int i = 0; i < SENSORES; i++) {
    pinMode(sensores[i], INPUT);
    if(i < MOTORES) pinMode(motores[i], OUTPUT);
  }
}

void setMotores(double left, double right) {
  //
  analogWrite(motores[0], 255 * MAX_SPEED * left);
  analogWrite(motores[1], 255 * MAX_SPEED * right);
}

void loop() {
  // put your main code here, to run repeatedly:
//  int leituras[SENSORES];
//  for(int i = 0; i < SENSORES; i++) {
//    leituras[i] = analogRead(sensores[i]);
//  }
  int nr = random() % 3;
  switch(nr) {
    case 0:
      setMotores(1.00, 1.00); // fremte
    case 1:
      setMotores(0.00, 1.00); // esquerda
    case 2:
      setMotores(1.00, 0.00); // direita
    case 3:
      setMotores(0.00, 0.00);
  }
  delay(5000);
}
