#include <WiFi.h>

// Credenciais Wi-Fi do ponto de acesso do servidor
const char* ssid = "ESP32_AP";         // SSID do ponto de acesso do servidor
const char* password = "12345678";     // Senha do Wi-Fi

WiFiClient client;  // Cliente WiFi

const int buttonPin = 4;  // Pino do botão

void setup() {
  Serial.begin(115200);

  pinMode(buttonPin, INPUT_PULLUP);  // Configura o pino do botão

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
  if (digitalRead(buttonPin) == LOW) {  // Verifica se o botão foi pressionado
    Serial.println("Botão pressionado!");

    // Tenta se conectar ao servidor
    if (client.connect("192.168.4.1", 80)) {  // IP do servidor (alterar conforme necessário)
      Serial.println("Conectado ao servidor!");

      // Envia o comando como uma requisição HTTP GET
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
  delay(100);  // Pequeno atraso para evitar leituras rápidas demais
}
