#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Inicializa o LCD
hd44780_I2Cexp lcd;

// Inicializa o MPU6050
Adafruit_MPU6050 mpu;

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

  // Exibe mensagem inicial no LCD
  lcd.setCursor(0, 0);
  lcd.print("MPU6050 OK!");
  delay(2000); // Aguarde 2 segundos antes de começar as leituras
  lcd.clear();
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

  delay(500); // Aguarde meio segundo entre as atualizações
}

