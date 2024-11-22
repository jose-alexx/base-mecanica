#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <esp_now.h>

// Configurações do Access Point
const char* ssid = "ESP32-AP";
const char* password = "12345678";

// Variável para status
String statusMotor = "Desligado";

// Estrutura de comando
typedef struct {
  char command[10];
} CommandData;
CommandData command;

// Endereço MAC do ESP32-B
uint8_t broadcastAddress[] = {0x24, 0x0A, 0xC4, 0xB1, 0x02, 0xC8};

// Cria o servidor web
AsyncWebServer server(80);

void onSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Comando enviado!" : "Erro ao enviar comando!");
}

void setup() {
  Serial.begin(115200);

  // Configura como Access Point
  WiFi.softAP(ssid, password);
  Serial.print("IP do AP: ");
  Serial.println(WiFi.softAPIP());

  // Inicializa ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao iniciar ESP-NOW!");
    return;
  }

  // Registra callback
  esp_now_register_send_cb(onSent);

  // Adiciona o ESP32-B como peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Erro ao adicionar peer!");
    return;
  }

  // Configura o servidor web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", "<html>Use /command?cmd=ligar|desligar|status</html>");
  });

  server.on("/command", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("cmd")) {
      String cmd = request->getParam("cmd")->value();
      cmd.toCharArray(command.command, sizeof(command.command));

      // Envia o comando ao ESP32-B via ESP-NOW
      esp_now_send(broadcastAddress, (uint8_t*)&command, sizeof(command));

      if (cmd == "ligar") {
        statusMotor = "Ligado";
      } else if (cmd == "desligar") {
        statusMotor = "Desligado";
      }
      request->send(200, "text/plain", "Comando enviado: " + cmd);
    } else {
      request->send(400, "text/plain", "Comando inválido");
    }
  });

  server.begin();
}

void loop() {
  // Nada a fazer no loop principal
}

