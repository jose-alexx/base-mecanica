#include <WiFi.h>

// Credenciais Wi-Fi do ponto de acesso do servidor
const char* ssid = "ESP32_AP";         
const char* password = "12345678";     

WiFiClient client;  
const int buttonPin4 = 4;   // Pino para ligar os motores
const int buttonPin15 = 15; // Pino para inverter a rotação

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin4, INPUT_PULLUP);   // Configura o pino 4
  pinMode(buttonPin15, INPUT_PULLUP);  // Configura o pino 15

  // Conectar-se ao Wi-Fi
  Serial.println("Conectando ao Wi-Fi...");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao Wi-Fi...");
  }
  Serial.print("Conectado! IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (digitalRead(buttonPin4) == LOW) {  // Botão no pino 4 pressionado
    Serial.println("Botão (pino 4) pressionado: Ligar motores!");
    if (client.connect("192.168.4.1", 80)) {
      client.println("GET /motor/on HTTP/1.1");
      client.println("Host: 192.168.4.1");
      client.println("Connection: close");
      client.println();
      delay(500);
      client.stop();
    } else {
      Serial.println("Falha ao conectar ao servidor");
    }
  }

  if (digitalRead(buttonPin15) == LOW) {  // Botão no pino 15 pressionado
    Serial.println("Botão (pino 15) pressionado: Inverter rotação!");
    if (client.connect("192.168.4.1", 80)) {
      client.println("GET /motor/invert HTTP/1.1");
      client.println("Host: 192.168.4.1");
      client.println("Connection: close");
      client.println();
      delay(500);
      client.stop();
    } else {
      Serial.println("Falha ao conectar ao servidor");
    }
  }

  delay(100);
}
