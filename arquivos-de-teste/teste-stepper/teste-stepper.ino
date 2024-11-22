#include <Stepper.h>

const int stepsPerRevolution = 2048;  // número de passos por revolução

// ULN2003 Motor Driver Pins para Motor 1
#define IN1_MOTOR1 19
#define IN2_MOTOR1 18
#define IN3_MOTOR1 4
#define IN4_MOTOR1 2

// ULN2003 Motor Driver Pins para Motor 2
#define IN1_MOTOR2 23
#define IN2_MOTOR2 22
#define IN3_MOTOR2 21
#define IN4_MOTOR2 15

// inicializar a biblioteca Stepper para os dois motores
Stepper myStepper1(stepsPerRevolution, IN1_MOTOR1, IN3_MOTOR1, IN2_MOTOR1, IN4_MOTOR1);
Stepper myStepper2(stepsPerRevolution, IN1_MOTOR2, IN3_MOTOR2, IN2_MOTOR2, IN4_MOTOR2);

void setup() {
  // definir a velocidade dos motores (em rotações por minuto)
  myStepper1.setSpeed(5);  // motor 1
  myStepper2.setSpeed(5);  // motor 2

  // inicializar a porta serial
  Serial.begin(115200);
}

void loop() {
  // girar ambos os motores no sentido horário (mesma direção)
  Serial.println("Ambos os motores girando no sentido horário.");
  myStepper1.step(stepsPerRevolution);
  myStepper2.step(stepsPerRevolution);
  delay(1000);

  // girar ambos os motores no sentido anti-horário (mesma direção)
  Serial.println("Ambos os motores girando no sentido anti-horário.");
  myStepper1.step(-stepsPerRevolution);
  myStepper2.step(-stepsPerRevolution);
  delay(1000);
}