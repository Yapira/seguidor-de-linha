/* "Pau Mandado" - Seguidor de Linha
 * Autor: Equipe Yapira
 * Versao: 0.1 (Mai/2015)
 */

/* Limite de velocidade */
#define MAX_SPEED     0.172110

/* Velocidade para corrigit alinhamento na reta */
#define STR_FIX_SPEED  0.0

/* Define pinos de entrada dos sensores.
 * O indice 0 do vetor e' sao sensor mais a esquerda, enquanto o indice (SENSORES_PINS - 1) e' o sensor mais a direita.
 * Disposicao *aproximada* dos sensores fisicamente (no hardware):
 * 0 1 2 5 6 7
 *     3 4
 */
#define SENSORES_PINS 8
int pinSensores[] = {0, 1, 2, 3, 4, 5, 6, 7};

//Fio cinza esquerdo
//Fio branco direita
#define ENGINE_PINS  2
int pinEngines[] = {11, 10};

// Define o nivel do sensor quando ele estiver em cima da faixa/marcacao.
#define SENSOR_NIVEL_FAIXA LOW

///* Define o pino de entrada do botao (se tiver) */
//#define buttonPin 13

/* Definicao dos modos operacionais
 * Modo SLeeP: Robo em stand-by, acionando o robo apos o delay inicial.
 * Modo STRaight: Robo andando em linha reta, tentara fazer apenas pequenas correcoes na trajetoria
 * Modo TuRN: Robo detectou uma marcacao de curva ou inicio/fim de percurso.
 * Mode UnKnoWn (Aceito sugestoes de nome): Modo pos-curva, para detectar se a curva continua em sentido inverso ou se e' uma reta.
 */
#define MODE_SLP 0
#define MODE_STR 1
#define MODE_TRN 2
#define MODE_UKW 3
int currentMode = MODE_SLP;

/* Variaveis utilizadas durante o programa */

bool didIgnoreFirstMark; // Utilizado para ignorar a marca da partida.
int direcaoCurva; // Sentido da curva. -1 = esquerda / 0 = indeterminado / 1 = direita
double turnRate; // Ajuste da potencia do motor no momento da curva (1.00 >= turnRate >= 0.00)
//long millisSinceCurveEnded; // Tempo desde o ultimo termino de curva.
long oldMillis, ledMillis = 0;
int ledStatus = LOW;

// TO-DO: Experimentar e brincar com os defines abaixo
// Tempo (em ms) maximo que o robo permanece no modo unknown
#define UNKNOWN_MAX_TIME 250
// Tempo (em ms) entre as checagens de alinhamento na curva.
#define TIME_BETWEEN_CURVE_CHECKS 1
// Tempo (em ms) em que o robo permanece em "sleep" apos ser ligado.
#define SLEEP_TIME 2500

/* setMotores(left, right);
 * Envia um sinal PWM para os drivers dos motores para aciona-los com a potencia desejada.
 * Left e Right sao dois "doubles" entre 1.00 e 0.00 que indicam a potencia
 */
void setMotores(double right, double left) {
  analogWrite(pinEngines[0], left * MAX_SPEED * 255);
  analogWrite(pinEngines[1], right * MAX_SPEED * 255);  
}

void acionaMotoresReta(int align) {
  switch(align) {
    case -1: //Esquerda
      setMotores(1.0, STR_FIX_SPEED);
      break;
    case 1: // Direita
      setMotores(STR_FIX_SPEED, 1.0);
      break;
    case 0:
      setMotores(1.0, 1.0);
      break;
    default:
//      Serial.write("Alinhamento desconhecido (MODE_STR)");
      break;
  } 
}

/* alinhamentoReta(leituras[])
 * Analisa o alinhamento do robo na reta (dando prioridade para os sensores frontais)
 * Com base nas leituras dos sensores (vetor "leituras"), retorna:
 * -1 se o robo esta' muito para a esquerda (sensores 2 e/ou 4 para fora da faixa)
 * 0 se o robo esta' devidamente alinhado com a linha (todos os 4 sensores centrais dentro da faixa).
 * 1 se o robo esta' muito para a direita (sensor 3 e/ou 5 para fora da faixa)
 */
int alinhamentoReta(int *leituras) {
  if(leituras[2] != SENSOR_NIVEL_FAIXA) return -1;
  if(leituras[3] != SENSOR_NIVEL_FAIXA) return 1;
  if(leituras[4] != SENSOR_NIVEL_FAIXA) return -1;
  if(leituras[5] != SENSOR_NIVEL_FAIXA) return 1;
  return 0;
}

/* alinhamentoCurva
 * Analisa o alinhamento do robo na curva.
 * Retorna:
 * -1 se o robo esta' saindo para a esquerda na curva.
 * 0 se o robo esta' "alinhado" com a curva.
 * 1 se o robo esta' saindo para a direita na curva.
 */
int alinhamentoCurva(int *leituras) {
  // VERIFICAR SE N EH MELHOR USAR OS SENSORES CENTRAIS!!!!!!!!!!!!!!!!!
  if(leituras[1] == SENSOR_NIVEL_FAIXA) return -1;
  if(leituras[6] == SENSOR_NIVEL_FAIXA) return 1;
  return 0;
  // TO-DO: Testar o return abaixo na pratica:
  // return alinhamentoReta(leituras);
}

#define MARK_CURVA   1
#define MARK_INIFIM  2
bool didFoundMark = false; // Utilizado para guardar o encontro de uma marca, possibilitando o return apos o fim da marca
bool isCruzamento;
int markOnSensor = 0; // Sensor onde a marca foi encontrada
/* buscaMarcas(leituras[])
 * Retorna MARK_CURVA caso o sensor detecte uma marca de curva (marca na esquerda do robo!)
 * Retorna MARK_INIFIM caso o sensor detecte uma marca de inicio/fim de percurso (marca na direita do robo!)
 * Retorna 0 caso contra'rio.
 * Detalhe importante: A funcao so retorna apos parar de detectar a marca!
 */
int buscaMarcas(int *leituras) {
  if(didFoundMark) {
    // Caso ja tenha encontrado uma marca anteriormente, procura pelo fim dela
    if(!isCruzamento && leituras[markOnSensor - 1] != SENSOR_NIVEL_FAIXA) {
      didFoundMark = false;
      return (markOnSensor == 1) ? (MARK_CURVA) : (MARK_INIFIM);
    }
    if(leituras[SENSORES_PINS - markOnSensor] == SENSOR_NIVEL_FAIXA) isCruzamento = true;  
    if(isCruzamento && leituras[0] != SENSOR_NIVEL_FAIXA && leituras[7] != SENSOR_NIVEL_FAIXA) didFoundMark = false;
  } else {
    didFoundMark = true;
    isCruzamento = false;
    if(leituras[0] == SENSOR_NIVEL_FAIXA) markOnSensor = 1;
    else if(leituras[7] == SENSOR_NIVEL_FAIXA) markOnSensor = 8;
    else didFoundMark = false;
  }
  return 0;
}

int isForaDaLinha(int *leituras) {
  for(int i = 0; i < SENSORES_PINS; i++) {
    if(leituras[i] == SENSOR_NIVEL_FAIXA) return 0;
  }
  return 1;
}

void setup() {
  //  pinMode(buttonPin, INPUT);
  for(int i = 0; i < SENSORES_PINS; i++) {
    pinMode(pinSensores[i], INPUT);
    if(i < ENGINE_PINS) pinMode(pinEngines[i], OUTPUT);
  }
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
}

void loop() {
  int leituras[SENSORES_PINS], align, marca;
  if(currentMode != MODE_SLP) {
    for(int i = 0; i < SENSORES_PINS; i++) {
      leituras[i] = digitalRead(pinSensores[i]);
    }
    marca = buscaMarcas(leituras);
  }
//  if(isForaDaLinha(leituras)) {
//    setMotores(0.0, 0.0);
//    return;
//  }
  switch(currentMode) {
    case MODE_SLP:
      {
        // Aguarda pressionamento do botao para ir para o modo str.
        didIgnoreFirstMark = false;
//        delay(SLEEP_TIME);
        currentMode = MODE_STR;
      }
      break;                          
    case MODE_STR:
      {
        // Leves correcoes de trajetoria; Detectar marcas no chao e acionar os motores de acordo.
        digitalWrite(13, LOW);
        align = alinhamentoReta(leituras);
        acionaMotoresReta(align);
        
        if(marca == MARK_INIFIM && !didIgnoreFirstMark) didIgnoreFirstMark = true;
        else if(marca == MARK_INIFIM) {
          // Fim de percurso. Mantem motores ativos por 1 segundo e depois para completamente o robo.
          setMotores(1.0, 1.0);
          delay(1000);
          currentMode = MODE_SLP;
          setMotores(0, 0);
        } else if(marca == MARK_CURVA) {
          currentMode = MODE_TRN;
          direcaoCurva = 0;
          turnRate = 1;
          oldMillis = 0;
        }
      }
      break;
    case MODE_TRN:
      /* Buscar inicio da curva, determinando se eh uma curva pra esquerda ou pra direita.
       * Procurar marcas durante a curva e identificar se elas sao marcas de fim de curva ou de mudanca de sentido de curva.
       * Ao detectar uma marca de fim de curva ou inversao de sentido, leva ao modo "unknown".
       */
      digitalWrite(13, HIGH);
      if(millis() > oldMillis + TIME_BETWEEN_CURVE_CHECKS) {
        oldMillis = millis();
        align = alinhamentoCurva(leituras);
        switch(direcaoCurva) {
          case 0: 
            if (align != 0) direcaoCurva = align;
            setMotores(1.0, 1.0);
            break;
          default:
            if(align == -direcaoCurva) turnRate -= 3 * turnRate / 4;
            else if(align == direcaoCurva) turnRate += 3 * (1 - turnRate) / 4;
            if(direcaoCurva == 1) setMotores(1.0, turnRate);
            else setMotores(turnRate, 1.0);
        }
      }
      if(marca == MARK_CURVA) {
        currentMode = MODE_UKW;
        oldMillis = millis();
      }
      break;
    case MODE_UKW: 
      /* Tenta determinar se a ultima marca encontrada foi uma marca de fim de curva ou inversao de sentido.
       * Caso detecte algo que seja condizente com uma curva, leva novamente ao modo turn.
       * Caso contrario, apos UKNOWN_MAX_TIME ms, leva ao modo straight.
       */
      {
        // Busca marca de inicio de nova curva, levando ao... modo curva.
        if(marca == MARK_CURVA) {
          direcaoCurva = 0;
          currentMode = MODE_TRN;
          turnRate = 1;
          oldMillis = 1;
        }
        // Inverte o led feliz a cada 250ms
        if(millis() > ledMillis + 250) {
          if(ledStatus == HIGH) {
            digitalWrite(13, LOW);
            ledStatus = LOW;
          } else {
            digitalWrite(13, HIGH);
            ledStatus = HIGH;
          }
          ledMillis = millis();
        }
        // Se estorou o tempo limite do modo uknown, leva ao modo reta.
        if(millis() > oldMillis + UNKNOWN_MAX_TIME) {
          currentMode = MODE_STR;
        } else {
          // Checa se detectou uma curva no sentido inverso e leva ao modo curva com sentido inverso
          align = alinhamentoCurva(leituras);
          if(align == -direcaoCurva) {
            direcaoCurva = -direcaoCurva;
            currentMode = MODE_TRN;
            turnRate = 1;
            oldMillis = 0;
          } else acionaMotoresReta(alinhamentoReta(leituras));
        }
      }
      break;
    default:
//      Serial.write("DEBUG: Modo de operacao desconhecido (em main())");
      break;
  }
}
