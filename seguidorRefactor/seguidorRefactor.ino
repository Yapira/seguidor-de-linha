/* "Pau Mandado" - Seguidor de Linha
 * Autor: Equipe Yapira
 * Versao: 0.1 (Mai/2015)
 */

/* Define pinos de entrada dos sensores. */
#define SENSORES_PINS 8
int pinSensores[] = {1, 2, 3, 4, 5, 6, 7 , 8};

/* Define os pinos de saida para os motores. Procurar portas analogicas. */
#define LEFT_ENGINE  0
#define RIGHT_ENGINE 2
#define ENGINE_PINS  4
int pinEngines[] = {9, 10, 11, 12};

/* Define o pino de entrada do botao (se tiver). */
#define buttonPin 13

/* Definicao dos modos operacionais 
 * Modo SLeeP: Robo em stand-by, aguardando pressionamento do botao
 * Modo STRaight: Robo andando em linha reta, tentara fazer apenas pequenas correcoes na trajetoria
 * Modo TuRN: Robo detectou uma marcacao de curva ou inicio/fim de percurso.
 */
#define MODE_SLP 0
#define MODE_STR 1
#define MODE_TRN 2
int currentMode = MODE_SLP;

bool didIgnoreFirstMark;

#define DIR_LEFT    1
#define DIR_SLEFT   2
#define DIR_FORWARD 3
#define DIR_SRIGHT  4
#define DIR_RIGHT   5

void setMotores(int direction, double speed) {
  // TO-DO: Preencher os cases
  switch(direction) {
    case DIR_LEFT:
    
      break;
    case DIR_SLEFT:
    
      break;
    case DIR_FORWARD:
    
      break;
    case DIR_SRIGHT:
    
      break;
    case DIR_RIGHT:
    
      break;
    default:
      Serial.write("DEBUG: Direcao desconhecida (em setMotores())");
  }
}

void setup() {
  pinMode(buttonPin, INPUT);
  for(int i = 0; i < SENSORES_PINS; i++) {
    pinMode(pinSensores[i], INPUT); 
    if(i < ENGINE_PINS) pinMode(pinEngines[i], OUTPUT);
  }
}

void loop() {
  int leituras[SENSORES_PINS];
  if(currentMode != MODE_SLP) {
    for(int i = 0; i < SENSORES_PINS; i++) {
      leituras[i] = digitalRead(pinSensores[i]);
    }
  }
  switch(currentMode) {
    case MODE_SLP:
      // Aguarda pressionamento do botao para ir para o modo str.
      if(digitalRead(buttonPin) == LOW) {
        currentMode = MODE_STR;
        didIgnoreFirstMark = false;
      }
      break;
    case MODE_STR:
      setMotores(DIR_FORWARD, 1);
      // TO-DO: Leves correcoes de trajetoria, detectar marcas no chao, cuidar dos cruzamentos!
      break;
    case MODE_TRN:
      /* TO-DO
       * Buscar pelo inicio curva (ou FIM DE PERCURSO!)
       * Caso seja curva:
       *   - Determinar direcao
       *   - Setar os motores de acordo
       *   - Tentar acertar a trajetoria
       *   - Busca continua por marcas no chao (fim da curva!)
       * Caso nao seja uma curva (ou seja, fim do percurso):
       *   - Parar o robo (ou talvez andar mais um pouco para garantir que ele esta dentro da area. Testar!).
       *   - Setar modo sleep
       *   - :D
       */
      break;
    default:
      Serial.write("DEBUG: Modo de operacao desconhecido (em main())");
  }
}
