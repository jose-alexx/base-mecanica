#include <WiFi.h>
#include <Stepper.h>

// Configuração Wi-Fi
const char* ssid = "ESP32_AP";  // Nome do SSID
const char* password = "12345678";  // Senha do ponto de acesso

WiFiServer server(80);  // Servidor na porta 80

// Motor de Passo
const int stepsPerRevolution = 2048;  // número de passos por revolução
#define IN1_MOTOR1 18
#define IN2_MOTOR1 5
#define IN3_MOTOR1 17
#define IN4_MOTOR1 16

#define IN1_MOTOR2 4
#define IN2_MOTOR2 0
#define IN3_MOTOR2 2
#define IN4_MOTOR2 15

Stepper myStepper1(stepsPerRevolution, IN1_MOTOR1, IN3_MOTOR1, IN2_MOTOR1, IN4_MOTOR1);
Stepper myStepper2(stepsPerRevolution, IN1_MOTOR2, IN3_MOTOR2, IN2_MOTOR2, IN4_MOTOR2);

int direction1 = 1;  // 1 para horário, -1 para anti-horário
int direction2 = -1; // 1 para horário, -1 para anti-horário

void setup() {
  Serial.begin(115200);

  // Conectar ao Wi-Fi
  WiFi.softAP(ssid, password);
  Serial.println("Ponto de acesso criado");
  Serial.print("IP do servidor: ");
  Serial.println(WiFi.softAPIP());

  server.begin();  // Inicia o servidor
  myStepper1.setSpeed(5);  // Define a velocidade do motor
  myStepper2.setSpeed(5);  // Define a velocidade do motor
}

void loop() {
  WiFiClient client = server.available();  // Verifica se há um cliente conectado

  if (client) {
    Serial.println("Novo cliente conectado");
    String currentLine = "";  // Para armazenar os dados recebidos

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);  // Imprime no monitor serial os dados recebidos

        // Verifica o fim da linha HTTP (requisição do cliente)
        if (c == '\n') {
          if (currentLine.length() == 0) {
            // Envia a resposta ao cliente (página HTML)
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.println("<html>");
            client.println("<head>");
            client.println("<title>Controle de Motores</title>");
            client.println("<style>");
            client.println("body { font-family: Arial, sans-serif; margin: 0; padding: 0; background-color: #f4f4f4; color: #333; }");
            client.println("h1 { color: #0078d4; }");
            client.println("button { padding: 10px 20px; font-size: 16px; margin: 10px; cursor: pointer; background-color: #4CAF50; color: white; border: none; border-radius: 5px; }");
            client.println("button:hover { background-color: #45a049; }");
            client.println(".container { display: flex; justify-content: center; align-items: center; flex-direction: column; margin-top: 50px; }");
            client.println("</style>");
            client.println("</head>");
            client.println("<body>");
            client.println("<div class='container'>");
            client.println("<h1>Controle de Motores</h1>");
            client.println("<form action='/motor' method='GET'>");
            client.println("<button type='submit'>Ativar Motores</button>");
            client.println("</form>");
            client.println("<form action='/stop' method='GET'>");
            client.println("<button type='submit'>Parar Motores</button>");
            client.println("</form>");
            client.println("</div>");
            client.println("</body>");
            client.println("</html>");

            break;
          } else {
            // Verifica os comandos enviados
            if (currentLine.endsWith("MOTOR_ON")) {
              Serial.println("Ativando motores...");
              motorControl(true);  // Ativa os motores
            }
            if (currentLine.endsWith("STOP_MOTORS")) {
              Serial.println("Parando motores...");
              motorControl(false);  // Para os motores
            }
            currentLine = "";
          }
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
  if (status) {
    Serial.println("Motores ligados");

    // Controla os motores de passo
    for (int i = 0; i < 100; i++) {
      myStepper1.step(direction1);
      myStepper2.step(direction2);
    }
  } else {
    Serial.println("Motores desligados");
    // Desligar motores
  }
}
