#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Endereço I2C do LCD, 16 colunas e 2 linhas

const int pinoLDR = A0; // Pino analógico para o LDR
const int ledVerde = 11; // Pino digital para o LED verde
const int ledAmarelo = 10; // Pino digital para o LED amarelo
const int ledVermelho = 9; // Pino digital para o LED vermelho
const int buzzer = 6; // Pino digital para o buzzer

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

    pinMode(ledVerde, OUTPUT); // Define o pino do LED verde como saída
    pinMode(ledAmarelo, OUTPUT); // Define o pino do LED amarelo como saída
    pinMode(ledVermelho, OUTPUT); // Define o pino do LED vermelho como saída
    pinMode(buzzer, OUTPUT); // Define o pino do buzzer como saída
}

void loop() {
    //Loop principal do sistema
}