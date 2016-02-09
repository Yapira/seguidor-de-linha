int sensores[] = {0, 1, 2, 3, 4, 5, 6, 7};

void setMotores(double left, double right) {
  analogWrite(11, 255 * left);
  analogWrite(10, 255 * right);
}

void setup() {
  // put your setup code here, to run once:
  for(int i = 0; i < 8; i++) {
    pinMode(sensores[i], INPUT);
  }
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool acionou = false;
  for(int i = 0; i < 8; i++) {
    if(analogRead(sensores[i]) == LOW) {
      acionou = true;
      if(i % 2 == 0) setMotores(1.00, 0.00);
      else setMotores(0.00, 1.00);
    } 
  }
  if(!acionou) {
    setMotores(0.00, 0.00);
  }
}
