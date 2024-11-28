#include <WiFi.h>

// Credenciais Wi-Fi do ponto de acesso do servidor
const char* ssid = "ESP32_AP";         // SSID do ponto de acesso do servidor
const char* password = "12345678";     // Senha do Wi-Fi

WiFiClient client;  // Cliente WiFi

const int buttonPin = 4;  // Pino do botão que controla os motores
const int invertButtonPin = 15;  // Pino do botão que inverte a rotação
const int buzzerPin = 2;  // Pino do buzzer

void setup() {
  Serial.begin(115200);

  pinMode(buttonPin, INPUT_PULLUP);  // Configura o pino do botão de ligar/desligar os motores
  pinMode(invertButtonPin, INPUT_PULLUP);  // Configura o pino do botão de inversão de rotação
  pinMode(buzzerPin, OUTPUT);  // Configura o pino do buzzer

  // Conectar-se ao Wi-Fi (do ponto de acesso do servidor)
  Serial.println("Conectando ao Wi-Fi...");
  WiFi.begin(ssid, password);
  
  // Aguarda a conexão com o Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao Wi-Fi...");
  }
  Serial.print("Conectado! IP: ");
  Serial.println(WiFi.localIP());  // Exibe o IP local do cliente
}

void loop() {
  // Verifica se o botão de ligar/desligar os motores foi pressionado
  if (digitalRead(buttonPin) == LOW) {  
    Serial.println("Botão pressionado! Enviando comando para ligar/desligar motores.");

    // Tenta se conectar ao servidor
    if (client.connect("192.168.4.1", 80)) {  // IP do servidor (alterar conforme necessário)
      Serial.println("Conectado ao servidor!");

      // Envia o comando para ligar os motores
      client.println("GET /motor/on HTTP/1.1");
      client.println("Host: 192.168.4.1");
      client.println("Connection: close");
      client.println();  // Linha em branco para finalizar a requisição

      delay(500);  // Delay para garantir que o comando seja enviado
      client.stop();  // Desconecta do servidor
    } else {
      Serial.println("Falha ao conectar ao servidor");
    }
  }

  // Verifica se o botão de inverter a rotação foi pressionado
  if (digitalRead(invertButtonPin) == LOW) {  
    Serial.println("Botão de inverter rotação pressionado! Enviando comando para inverter a rotação.");

    // Tenta se conectar ao servidor
    if (client.connect("192.168.4.1", 80)) {  // IP do servidor (alterar conforme necessário)
      Serial.println("Conectado ao servidor!");

      // Envia o comando para inverter a rotação
      client.println("GET /motor/invert HTTP/1.1");
      client.println("Host: 192.168.4.1");
      client.println("Connection: close");
      client.println();  // Linha em branco para finalizar a requisição

      delay(500);  // Delay para garantir que o comando seja enviado
      client.stop();  // Desconecta do servidor

      // Emite um som no buzzer para indicar que a rotação foi invertida
      tone(buzzerPin, 1000, 500);  // Toca o buzzer a 1000 Hz por 500 ms
    } else {
      Serial.println("Falha ao conectar ao servidor");
    }
  }

  delay(100);  // Pequeno atraso para evitar leituras rápidas demais
}
