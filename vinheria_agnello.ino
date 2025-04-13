#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Endereço I2C do LCD, 16 colunas e 2 linhas

void setup() {
    lcd.init(); // Inicializa o LCD
    lcd.backlight(); // Liga a luz de fundo do LCD
    lcd.clear(); // Limpa o LCD

    lcd.setCursor(0, 0); // Define o cursor na primeira linha
    lcd.print("Vinheria Agnello"); // Exibe o nome da vinheria

    lcd.setCursor(0, 1); // Define o cursor na segunda linha
    lcd.print("Bem-vindo!"); // Exibe a mensagem de boas-vindas

    delay(3000); // Exibe por 3 segundos
    lcd.clear(); // Limpa o LCD
}

void loop() {
    //Loop principal do sistema
}