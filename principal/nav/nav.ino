#include <Wire.h>
#include <Stepper.h>
#include <MPU6050.h>  // Agora deve funcionar após a instalação da biblioteca MPU6050

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

// Inicializar o MPU6050
MPU6050 mpu;

// Botão no pino 14
#define BUTTON_PIN 14

// inicializar a biblioteca Stepper para os dois motores
Stepper myStepper1(stepsPerRevolution, IN1_MOTOR1, IN3_MOTOR1, IN2_MOTOR1, IN4_MOTOR1);
Stepper myStepper2(stepsPerRevolution, IN1_MOTOR2, IN3_MOTOR2, IN2_MOTOR2, IN4_MOTOR2);

// Variáveis para controle de direção
int direction1 = 1;  // 1 para horário, -1 para anti-horário
int direction2 = -1;  // 1 para horário, -1 para anti-horário
bool motorsActive = true;  // Flag para controlar o estado dos motores

// Variáveis para controle de tempo do botão
unsigned long lastButtonPressTime = 0;
const unsigned long debounceDelay = 200;  // debounce para evitar múltiplos acionamentos

void setup() {
  // Definir a velocidade dos motores
  myStepper1.setSpeed(10);  // motor 1
  myStepper2.setSpeed(10);  // motor 2

  // Inicializar a porta serial
  Serial.begin(115200);

  // Inicializar o sensor MPU6050
  Wire.begin();
  mpu.initialize();

  // Verificar se o MPU6050 está funcionando
  if (!mpu.testConnection()) {
    Serial.println("Falha na conexão com o MPU6050!");
    while (1);
  }
  Serial.println("MPU6050 inicializado com sucesso!");

  // Configurar o pino do botão
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // Leitura do botão (com debounce)
  if (digitalRead(BUTTON_PIN) == LOW && (millis() - lastButtonPressTime > debounceDelay)) {
    lastButtonPressTime = millis();  // Atualizar o tempo da última pressão

    // Alternar o estado dos motores
    motorsActive = !motorsActive;

    if (motorsActive) {
      Serial.println("Motores ativados");
    } else {
      Serial.println("Motores desativados");
    }

    // Aguarda para evitar múltiplos acionamentos do botão
    delay(200);
  }

  // Ler a inclinação do eixo Z do MPU6050
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // Calcular a posição em graus para o eixo Z (utilizando a aceleração no eixo Z)
  float gZ = az / 16384.0;  // Escala do acelerômetro para +/-2g (valor padrão do MPU6050)
  float angleZ = asin(gZ) * 180.0 / PI;  // Converter para graus

  // Exibir a posição no monitor serial
  Serial.print("Posição Z: ");
  Serial.print(angleZ);
  Serial.println(" graus");

  // Se os motores estão ativos, controlar o movimento
  if (motorsActive) {
    // Controlar o Motor 1 com direção dinâmica
    myStepper1.step(direction1);  // Motor 1 vai na direção controlada pela variável

    // Controlar o Motor 2 com direção dinâmica
    myStepper2.step(direction2);  // Motor 2 vai na direção controlada pela variável
  } else {
    // Se os motores não estiverem ativos, apenas enviar uma mensagem
    Serial.println("Motores parados");
  }

  delay(100);  // Pequeno atraso para estabilidade na leitura do sensor
}
