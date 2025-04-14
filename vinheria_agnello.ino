#include <LiquidCrystal.h>

// Define os pinos do LCD: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// Define os pinos dos componentes conectados ao Arduino
const int pinoLDR = A3;         // Pino analógico que recebe o sinal do LDR
const int ledVerde = 11;        // LED verde indica luz adequada
const int ledAmarelo = 10;      // LED amarelo indica nível de alerta
const int ledVermelho = 9;      // LED vermelho indica luz excessiva
const int buzzer = 13;          // Buzzer para alarme sonoro

void setup() {
  // Inicializa o LCD com 16 colunas e 2 linhas
  lcd.begin(16, 2);

  // Exibe mensagem de boas-vindas no LCD
  lcd.setCursor(0, 0);                 // Posição: coluna 0, linha 0
  lcd.print("Vinheria Agnello");       // Primeira linha com o nome fictício

  lcd.setCursor(0, 1);                 // Posição: coluna 0, linha 1
  lcd.print("Bem-vindo!");             // Segunda linha com saudação
  delay(3000);                         // Aguarda 3 segundos para leitura
  lcd.clear();                         // Limpa o display após a mensagem

  // Define os pinos dos LEDs e buzzer como saídas
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  int soma = 0; // Variável para acumular as leituras

  // Realiza 10 leituras do sensor LDR para obter média (mais estável)
  for (int i = 0; i < 10; i++) {
    soma += analogRead(pinoLDR); // Lê valor do sensor de luz
    delay(100);                  // Pausa entre leituras (100ms)
  }

  int leituraMedia = soma / 10; // Calcula a média das leituras

  // Converte o valor analógico (0–1023) para porcentagem (0–100%)
  int porcentagemLuz = map(leituraMedia, 0, 1023, 0, 100);

  // Mostra o valor de luz atual no display LCD
  lcd.setCursor(0, 0);                  // Primeira linha
  lcd.print("Luz: ");                   // Texto fixo
  lcd.print(porcentagemLuz);           // Valor da luz
  lcd.print("%   ");                   // Limpa lixo visual com espaços

  // Controle de LEDs e buzzer com base na intensidade da luz
  if (porcentagemLuz <= 40) {
    // Luz adequada: LED verde aceso
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, LOW);
    noTone(buzzer); // Buzzer desligado
  } else if (porcentagemLuz <= 70) {
    // Luz em nível de alerta: LED amarelo aceso
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, HIGH);
    digitalWrite(ledVermelho, LOW);
    noTone(buzzer); // Buzzer desligado
  } else {
    // Luz excessiva: LED vermelho aceso + buzzer por 3s
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, HIGH);
    tone(buzzer, 1000); // Buzzer toca em 1kHz
    delay(3000);        // Dura 3 segundos
    noTone(buzzer);     // Desliga o buzzer após o tempo
  }

  delay(1000); // Aguarda 1 segundo antes de reiniciar o loop
}
