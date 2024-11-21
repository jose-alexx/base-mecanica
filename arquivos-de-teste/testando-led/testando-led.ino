// Definição dos pinos
const int led1Pin = 19;
const int led2Pin = 33;
const int led3Pin = 32;
const int buzzerPin = 25;

void setup() {
  // Definindo os pinos como saída
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  // Acende o LED1 e ativa o buzzer
  digitalWrite(led1Pin, HIGH);
  digitalWrite(buzzerPin, HIGH);
  delay(500);  // Espera 500 ms

  // Desliga o LED1 e desativa o buzzer
  digitalWrite(led1Pin, LOW);
  digitalWrite(buzzerPin, LOW);
  delay(500);  // Espera 500 ms

  // Acende o LED2 e ativa o buzzer
  digitalWrite(led2Pin, HIGH);
  digitalWrite(buzzerPin, HIGH);
  delay(500);  // Espera 500 ms

  // Desliga o LED2 e desativa o buzzer
  digitalWrite(led2Pin, LOW);
  digitalWrite(buzzerPin, LOW);
  delay(500);  // Espera 500 ms

  // Acende o LED3 e ativa o buzzer
  digitalWrite(led3Pin, HIGH);
  digitalWrite(buzzerPin, HIGH);
  delay(500);  // Espera 500 ms

  // Desliga o LED3 e desativa o buzzer
  digitalWrite(led3Pin, LOW);
  digitalWrite(buzzerPin, LOW);
  delay(500);  // Espera 500 ms
}

