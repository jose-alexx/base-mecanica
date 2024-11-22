#include <Stepper.h>

const int stepsPerRevolution = 2048;  // número de passos por revolução

// ULN2003 Motor Driver Pins para Motor 1
#define IN1_MOTOR1 18
#define IN2_MOTOR1 5
#define IN3_MOTOR1 17
#define IN4_MOTOR1 16

// ULN2003 Motor Driver Pins para Motor 2
#define IN1_MOTOR2 4
#define IN2_MOTOR2 0
#define IN3_MOTOR2 2
#define IN4_MOTOR2 15

// inicializar a biblioteca Stepper para os dois motores
Stepper myStepper1(stepsPerRevolution, IN1_MOTOR1, IN3_MOTOR1, IN2_MOTOR1, IN4_MOTOR1);
Stepper myStepper2(stepsPerRevolution, IN1_MOTOR2, IN3_MOTOR2, IN2_MOTOR2, IN4_MOTOR2);

unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
const long interval = 1;  // intervalo entre passos de 1 milissegundo

unsigned long previousDirectionChangeMillis = 0;  // tempo do último cambio de direção
const long directionChangeInterval = 5000;  // mudar direção a cada 5 segundos

// Variáveis para controle de direção
int direction1 = 1;  // 1 para horário, -1 para anti-horário
int direction2 = -1;  // 1 para horário, -1 para anti-horário

void setup() {
  // definir a velocidade dos motores (em rotações por minuto)
  myStepper1.setSpeed(5);  // motor 1
  myStepper2.setSpeed(5);  // motor 2

  // inicializar a porta serial
  Serial.begin(115200);
}

void loop() {
  unsigned long currentMillis = millis();  // pega o tempo atual

  // Verificar se é hora de mudar a direção
  if (currentMillis - previousDirectionChangeMillis >= directionChangeInterval) {
    previousDirectionChangeMillis = currentMillis;
    
    // Inverter a direção dos motores
    direction1 = -direction1;
    direction2 = -direction2;

    Serial.println("Direção invertida!");
  }

  // Controlar o Motor 1 com direção dinâmica
  if (currentMillis - previousMillis1 >= interval) {
    previousMillis1 = currentMillis;
    myStepper1.step(direction1);  // Motor 1 vai na direção controlada pela variável
  }

  // Controlar o Motor 2 com direção dinâmica
  if (currentMillis - previousMillis2 >= interval) {
    previousMillis2 = currentMillis;
    myStepper2.step(direction2);  // Motor 2 vai na direção controlada pela variável
  }

  // Ambos os motores agora estão girando simultaneamente e alternando a direção
}

