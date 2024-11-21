#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Stepper.h>

// Inicializa o LCD
hd44780_I2Cexp lcd;

// Inicializa o MPU6050
Adafruit_MPU6050 mpu;

// Define o número de passos por revolução do motor
#define STEPS_PER_REV 2048

// Define os pinos de controle dos motores de passo
#define motor1Pin1  4  // IN1 do motor 1
#define motor1Pin2  0  // IN2 do motor 1
#define motor1Pin3  2  // IN3 do motor 1
#define motor1Pin4  15  // IN4 do motor 1

#define motor2Pin1  16  // IN1 do motor 2
#define motor2Pin2  17  // IN2 do motor 2
#define motor2Pin3  18  // IN3 do motor 2
#define motor2Pin4  5  // IN4 do motor 2

// Inicializa os motores de passo
Stepper motor1(STEPS_PER_REV, motor1Pin1, motor1Pin2, motor1Pin3, motor1Pin4);
Stepper motor2(STEPS_PER_REV, motor2Pin1, motor2Pin2, motor2Pin3, motor2Pin4);

void setup() {
  // Inicializa o monitor serial
  Serial.begin(115200);

  // Inicializa o LCD
  int status = lcd.begin(20, 4); // Configura o LCD 20x4
  if (status) {
    Serial.print("Erro LCD: ");
    Serial.println(status);
    while (1);
  }

  lcd.backlight();  // Liga a luz de fundo do LCD
  lcd.clear();      // Limpa o display

  // Inicializa o MPU6050
  if (!mpu.begin()) {
    Serial.println("MPU6050 nao encontrado!");
    lcd.setCursor(0, 0);
    lcd.print("Erro: MPU6050!");
    while (1);
  }

  lcd.setCursor(0, 0);
  lcd.print("MPU6050 OK!");
  delay(2000); // Aguarde 2 segundos antes de começar as leituras
  lcd.clear();

  // Define a velocidade dos motores
  motor1.setSpeed(15);  // Ajuste a velocidade (em RPM) para o motor 1
  motor2.setSpeed(15);  // Ajuste a velocidade (em RPM) para o motor 2
}

void loop() {
  // Obtém dados do MPU6050
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Exibe os dados no LCD
  lcd.setCursor(0, 0);
  lcd.print("Accel X: ");
  lcd.print(a.acceleration.x, 1); // Uma casa decimal
  lcd.setCursor(0, 1);
  lcd.print("Accel Y: ");
  lcd.print(a.acceleration.y, 1); // Uma casa decimal
  lcd.setCursor(0, 2);
  lcd.print("Gyro X: ");
  lcd.print(g.gyro.x, 1); // Uma casa decimal
  lcd.setCursor(0, 3);
  lcd.print("Temp: ");
  lcd.print(temp.temperature, 1); // Uma casa decimal
  lcd.print(" C");

  // Movimento dos motores
  motor1.step(100);  // Motor 1 gira no sentido horário (100 passos)
  motor2.step(-100); // Motor 2 gira no sentido anti-horário (100 passos negativos)

  delay(1000); // Aguarde 1 segundo entre as atualizações
}

