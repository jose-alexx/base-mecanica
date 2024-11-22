#include <esp_now.h>
#include <WiFi.h>

// Estrutura de comando
typedef struct {
  char command[10];
} CommandData;
CommandData command;

void onReceive(const uint8_t *mac_addr, const uint8_t *data, int len) {
  memcpy(&command, data, sizeof(command));

  if (strcmp(command.command, "ligar") == 0) {
    Serial.println("Motor Ligado!");
  } else if (strcmp(command.command, "desligar") == 0) {
    Serial.println("Motor Desligado!");
  } else if (strcmp(command.command, "status") == 0) {
    Serial.println("Enviando status...");
    // Aqui você pode enviar de volta informações para o ESP32-A
  }
}

void setup() {
  Serial.begin(115200);

  // Configura o Wi-Fi no modo STA
  WiFi.mode(WIFI_STA);

  // Inicializa ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao iniciar ESP-NOW!");
    return;
  }

  // Registra callback para receber dados
  esp_now_register_recv_cb(onReceive);
}

void loop() {
  // Processamento contínuo
}