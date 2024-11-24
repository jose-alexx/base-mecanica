#include <Stepper.h>

// Configuração para o primeiro motor de passo
const int stepsPerRevolution = 2048; // Passos por revolução
#define IN1 19
#define IN2 18
#define IN3 5
#define IN4 17
Stepper stepper1(stepsPerRevolution, IN1, IN3, IN2, IN4);

// Configuração para o segundo motor de passo
#define IN5 23
#define IN6 22
#define IN7 21
#define IN8 20
Stepper stepper2(stepsPerRevolution, IN5, IN7, IN6, IN8);

void setup() {
  stepper1.setSpeed(15); // Velocidade do primeiro motor em RPM
  stepper2.setSpeed(15); // Velocidade do segundo motor em RPM
}

void loop() {
  // Controlando o primeiro motor
  stepper1.step(stepsPerRevolution); // Uma volta no sentido horário
  delay(1000);
  stepper1.step(-stepsPerRevolution); // Uma volta no sentido anti-horário
  delay(1000);

  // Controlando o segundo motor
  stepper2.step(stepsPerRevolution); // Uma volta no sentido horário
  delay(1000);
  stepper2.step(-stepsPerRevolution); // Uma volta no sentido anti-horário
  delay(1000);
}

