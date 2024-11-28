#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class Motor {
  private:
    int in1, in2, in3, in4;

  public:
    Motor(int pin1, int pin2, int pin3, int pin4) {
      in1 = pin1;
      in2 = pin2;
      in3 = pin3;
      in4 = pin4;
      pinMode(in1, OUTPUT);
      pinMode(in2, OUTPUT);
      pinMode(in3, OUTPUT);
      pinMode(in4, OUTPUT);
    }

    void ligarParaFrente() {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
    }

    void ligarParaTras() {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    }

    void desligar() {
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
    }
};

class Buzzer {
  private:
    int buzzerPin;
    int ledPin;
     int ledAmareloPin;

  public:
    Buzzer(int pin, int led, int ledAmarelo) {
      buzzerPin = pin;
      ledPin = led;
      ledAmareloPin = ledAmarelo;
      pinMode(buzzerPin, OUTPUT);
      pinMode(ledPin, OUTPUT);
      pinMode(ledAmareloPin, OUTPUT);
    }

     void emitirSirene(int duracao) {
      digitalWrite(ledAmareloPin, LOW);
      unsigned long startTime = millis();
      while (millis() - startTime < duracao) {
        for (int i = 1000; i <= 4000; i += 20) {
          tone(buzzerPin, i);
          digitalWrite(ledPin, HIGH);
          delay(2);
          digitalWrite(ledPin, LOW);
        }
        for (int i = 4000; i >= 1000; i -= 20) {
          tone(buzzerPin, i);
          digitalWrite(ledPin, HIGH);
          delay(2);
          digitalWrite(ledPin, LOW);
        }
      }
      noTone(buzzerPin);
      digitalWrite(ledAmareloPin, HIGH);
    }

    void emitirSom(int duracao) {
      digitalWrite(ledAmareloPin, LOW);
      tone(buzzerPin, 1000);
      digitalWrite(ledPin, HIGH); // Liga o LED
      delay(duracao);
      noTone(buzzerPin); // Para o som
      digitalWrite(ledPin, LOW); // Desliga o LED
      digitalWrite(ledAmareloPin, HIGH); // Liga o LED amarelo
    }

    void emitirDoisToquesRapidos() {
      for (int i = 0; i < 2; i++) {
        emitirSom(200); // Emite som por 200 milissegundos
        delay(100); // Espera 100 milissegundos entre os toques
      }
    }
};

class Contar {
  private:
    Buzzer& buzzer;

  public:
    Contar(Buzzer& b) : buzzer(b) {}

    void contarAteTres() {
      for (int i = 1; i <= 3; i++) {
        buzzer.emitirSom(500); // Emite som por 500 milissegundos
        delay(2000); // Espera 2 segundos entre os sons
      }
    }
};

// Definindo os pinos dos motores A e B
int IN1 = 27;
int IN2 = 19;
int IN3 = 18;
int IN4 = 5;

int botaoFrentePin = 15;
int botaoTrasPin = 4;
int resetPin = 13;
int ledPin = 14; // Pino do LED
int ledAmareloPin = 12; // Pino do LED amarelo
int buzzerPin = 2;
int ledSempreLigadoPin = 23; // Pino do LED que sempre estará ligado

// Criando instâncias das classes
Motor motor(IN1, IN2, IN3, IN4);
Buzzer buzzer(buzzerPin, ledPin, ledAmareloPin);
Contar contador(buzzer);

bool frentePressionado = false; // Variável de controle para saber se o botão do pino 15 já foi pressionado

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  pinMode(botaoFrentePin, INPUT_PULLUP);
  pinMode(botaoTrasPin, INPUT_PULLUP);
  pinMode(resetPin, INPUT_PULLUP);
  pinMode(ledAmareloPin, OUTPUT);
  pinMode(ledSempreLigadoPin, OUTPUT);

  digitalWrite(ledSempreLigadoPin, HIGH); // Liga o LED sempre ligado

  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);

      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);

  lcd.init();
  lcd.backlight();
  lcd.print("Preparar para");
  lcd.setCursor(0, 1);
  lcd.print("o lancamento");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Aperte lancar");
}

void loop() {
  int botaoFrente = digitalRead(botaoFrentePin);
  int botaoTras = digitalRead(botaoTrasPin);
    int reset = digitalRead(resetPin);

  if (reset == LOW) {
    lcd.clear();
    lcd.setDelay(0, 0);
    lcd.print("Resetando");
    lcd.setDelay(0, 1);
    lcd.print("atividades");
    frentePressionado = false; // Reseta a variável de controle
    buzzer.emitirDoisToquesRapidos(); // Emite dois toques rápidos antes de ligar para trás
    delay(500);
    motor.desligar(); // Desliga os motores
    digitalWrite(ledAmareloPin, HIGH); // Liga o LED amarelo
    delay(1000); // Espera um segundo para evitar detecções falsas
    lcd.clear();
    lcd.setDelay(0, 0);
    lcd.print("Aperte lancar");
  }

  if (botaoFrente == LOW) {
    // contador.contarAteTres(); // Inicia a contagem até três com som
    lcd.clear();
    lcd.setDelay(0, 0);
    lcd.print("Contagem:");

    for(int i=1; i<= 3; i++) {
      lcd.setDelay(9, 0);
      lcd.print(i);
      buzzer.emitirSom(500);
      delay(2000);
    }
    lcd.clear();
    lcd.setDelay(0, 0);
    lcd.print("Lancar!");
    buzzer.emitirSirene(5000); // Emite sirene por 5 segundos
    motor.ligarParaFrente(); // Liga os motores para frente
    frentePressionado = true; //atualiza
    delay(1000);
  } else if (botaoTras == LOW && frentePressionado) {
    lcd.clear();
    lcd.setDelay(0, 0);
    lcd.print("Voltando");
    lcd.setDelay(0, 1);
    lcd.print("motores");
    buzzer.emitirDoisToquesRapidos(); // Emite dois toques rápidos antes de ligar para trás
    delay (500);
    motor.ligarParaTras(); // Liga os motores para trás
    digitalWrite(ledAmareloPin, LOW); // Liga o LED amarelo
    delay(1000);
  } else {
    motor.desligar(); // Desliga os motores
    digitalWrite(ledAmareloPin, HIGH); // Liga o LED amarelo
  }
  digitalWrite(ledSempreLigadoPin, HIGH); 
}

