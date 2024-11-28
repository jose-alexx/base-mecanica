#include <WiFi.h>
#include <Stepper.h>
#include <Wire.h>
#include <MPU6050.h>

// Configuração Wi-Fi
const char* ssid = "ESP32_AP";  
const char* password = "12345678";  

WiFiServer server(80);  

// Motor de Passo
const int stepsPerRevolution = 2048;
#define IN1_MOTOR1 19
#define IN2_MOTOR1 18
#define IN3_MOTOR1 5
#define IN4_MOTOR1 17

#define IN1_MOTOR2 16
#define IN2_MOTOR2 4
#define IN3_MOTOR2 0
#define IN4_MOTOR2 2

Stepper myStepper1(stepsPerRevolution, IN1_MOTOR1, IN3_MOTOR1, IN2_MOTOR1, IN4_MOTOR1);
Stepper myStepper2(stepsPerRevolution, IN1_MOTOR2, IN3_MOTOR2, IN2_MOTOR2, IN4_MOTOR2);

MPU6050 mpu;  
bool motorStatus = false;
bool invertDirection = false;  // Flag para inverter direção

const int buzzerPin = 2;  // Pino do buzzer
const int ledPin = 12;    // Pino do LED verde
const int ledRedPin = 14; // Pino do LED vermelho

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  Serial.println("Ponto de acesso criado");
  Serial.print("IP do servidor: ");
  Serial.println(WiFi.softAPIP());

  server.begin();
  myStepper1.setSpeed(5);
  myStepper2.setSpeed(5);

  pinMode(buzzerPin, OUTPUT);  // Configura o pino do buzzer
  pinMode(ledPin, OUTPUT);     // Configura o pino do LED verde
  pinMode(ledRedPin, OUTPUT);  // Configura o pino do LED vermelho

  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("Falha na conexão com o MPU6050");
    while (1);
  } else {
    Serial.println("MPU6050 conectado!");
  }
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Novo cliente conectado");
    String currentLine = "";  

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);  

        if (c == '\n') {
          // Comandos recebidos
          if (currentLine.indexOf("GET /motor/on") >= 0) {
            Serial.println("Comando: Ligar motores");
            motorControl(true);  // Liga os motores
          } else if (currentLine.indexOf("GET /motor/off") >= 0) {
            Serial.println("Comando: Desligar motores");
            motorControl(false); // Desliga os motores
          } else if (currentLine.indexOf("GET /motor/invert") >= 0) {
            Serial.println("Comando: Inverter rotação");
            invertDirection = !invertDirection;  // Inverte a direção
            motorControl(true); // Religa os motores após inverter
            soundBuzzer();      // Emite o som do buzzer
          }

          // Envia a página HTML ao cliente
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("<!DOCTYPE html><html><head><title>Controle de Motores</title></head><body>");
            client.println("<button onclick=\"location.href='/motor/on'\">Ligar Motores</button>");
            client.println("<button onclick=\"location.href='/motor/off'\">Desligar Motores</button>");
            client.println("<button onclick=\"location.href='/motor/invert'\">Inverter Rotação</button>");
            client.println("<h2>Status dos Motores: " + String(motorStatus ? "Ligados" : "Desligados") + "</h2>");
            client.println("</body></html>");
            break;
          }
          currentLine = "";
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println("Cliente desconectado");
  }
  delay(100);
}

void motorControl(bool status) {
  motorStatus = status;
  if (status) {
    Serial.println("Motores ligados");
    digitalWrite(ledPin, HIGH);  // Acende o LED verde
    for (int i = 0; i < 100; i++) {
      // Controla a direção com base na variável invertDirection
      myStepper1.step(invertDirection ? -1 : 1);  
      myStepper2.step(invertDirection ? 1 : -1);   
      delay(10);
    }
  } else {
    Serial.println("Motores desligados");
    digitalWrite(ledPin, LOW);   // Apaga o LED verde
  }
}

void soundBuzzer() {
  tone(buzzerPin, 1000, 500);  // Toca o buzzer a 1000 Hz por 500 ms

  // Faz o LED vermelho piscar enquanto o buzzer toca
  for (int i = 0; i < 5; i++) {  // Pisca 5 vezes
    digitalWrite(ledRedPin, HIGH);  // Acende o LED vermelho
    delay(100);                     // Aguarda 100ms
    digitalWrite(ledRedPin, LOW);   // Apaga o LED vermelho
    delay(100);                     // Aguarda 100ms
  }
}

