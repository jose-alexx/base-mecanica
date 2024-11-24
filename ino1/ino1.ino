#include <WiFi.h>
#include <Stepper.h>

// Configuração Wi-Fi
const char* ssid = "ESP32_AP";  // Nome do SSID
const char* password = "12345678";  // Senha do ponto de acesso

WiFiServer server(80);  // Servidor na porta 80

// Motor de Passo
const int stepsPerRevolution = 2048;  // número de passos por revolução
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

bool motorStatus = false; // Status dos motores (ligado/desligado)

void setup() {
  Serial.begin(115200);

  // Configura o ponto de acesso Wi-Fi
  WiFi.softAP(ssid, password);
  Serial.println("Ponto de acesso criado");
  Serial.print("IP do servidor: ");
  Serial.println(WiFi.softAPIP());

  server.begin();  // Inicia o servidor
  myStepper1.setSpeed(5);  // Define a velocidade do motor
  myStepper2.setSpeed(5);  // Define a velocidade do motor
}

void loop() {
  // Processa solicitações do cliente (página web ou ESP32 Cliente)
  WiFiClient client = server.available();

  if (client) {
    Serial.println("Novo cliente conectado");
    String currentLine = "";  // Para armazenar os dados recebidos

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);  // Imprime no monitor serial os dados recebidos

        if (c == '\n') {
          // Verifica o comando recebido e ativa os motores
          if (currentLine.indexOf("GET /motor/on") >= 0) {
            motorControl(true); // Liga os motores
          } else if (currentLine.indexOf("GET /motor/off") >= 0) {
            motorControl(false); // Desliga os motores
          }

          // Envia a página web ao cliente
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // Página HTML com botões para controle
            client.println("<!DOCTYPE html>");
            client.println("<html>");
            client.println("<head>");
            client.println("<title>Controle de Motores</title>");
            client.println("<style>");
            client.println("body { font-family: Arial, sans-serif; text-align: center; background-color: #f4f4f4; }");
            client.println("button { padding: 15px 30px; margin: 20px; font-size: 18px; background-color: #4CAF50; color: white; border: none; border-radius: 5px; cursor: pointer; }");
            client.println("button:hover { background-color: #45a049; }");
            client.println("</style>");
            client.println("</head>");
            client.println("<body>");
            client.println("<h1>Controle de Motores</h1>");
            // Botões para ligar/desligar os motores
            client.println("<button onclick=\"location.href='/motor/on'\">Ligar Motores</button>");
            client.println("<button onclick=\"location.href='/motor/off'\">Desligar Motores</button>");
            // Exibe o status dos motores
            client.print("<h2>Status dos Motores: ");
            client.print(motorStatus ? "Ligados" : "Desligados");
            client.println("</h2>");
            client.println("</body>");
            client.println("</html>");
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
}

void motorControl(bool status) {
  motorStatus = status;
  if (status) {
    Serial.println("Motores ligados");

    // Controla os motores de passo
    for (int i = 0; i < 100; i++) {
      myStepper1.step(1);  // Gira no sentido horário
      myStepper2.step(-1); // Gira no sentido anti-horário
      delay(10);
    }
  } else {
    Serial.println("Motores desligados");
  }
}
