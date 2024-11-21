#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

hd44780_I2Cexp lcd;

void setup() {
  int status = lcd.begin(20, 4);
  if (status) {
    Serial.print("Erro LCD: ");
    Serial.println(status);
    while (1);

  }
  
   lcd.clear();
   lcd.noBacklight();
   lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("Teste Linha 1");
  lcd.setCursor(0, 1);
  lcd.print("Teste Linha 2");
  lcd.setCursor(0, 2);
  lcd.print("Teste Linha 3");
  lcd.setCursor(0, 3);
  lcd.print("Teste Linha 4");
}

void loop() {
}
