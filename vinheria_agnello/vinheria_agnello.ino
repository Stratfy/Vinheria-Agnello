#include <LiquidCrystal.h> // Biblioteca para controle do display LCD
#include <string.h>        // Biblioteca padrão para manipulação de strings (strlen)

// Inicializa o display LCD nos pinos (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

// Pino analógico conectado ao sensor LDR (fotoresistor)
const short int pinoLDR = A0;

// LEDs de sinalização (verde = ideal, amarelo = alerta, vermelho = crítico)
const short int ledVermelho = 6;
const short int ledAmarelo  = 5;
const short int ledVerde    = 4;

// Buzzer que emite som de alerta
const short int pinoBuzzer = 3;

// Botão físico usado para calibração dos níveis de luz
const short int botaoCalibrar = 2;

// Limites calibrados durante o setup
int limiteLuzMinima = 1023;
int limiteLuzMaxima = 0;

// Tempo de espera entre leituras (em milissegundos)
const int tempoPrecisao = 100;

void setup() {
  // Configura os pinos como entrada ou saída
  pinMode(pinoLDR, INPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(pinoBuzzer, OUTPUT);
  pinMode(botaoCalibrar, INPUT_PULLUP); // Usa resistor interno de pull-up

  // Inicializa o display LCD com 16 colunas e 2 linhas
  lcd.begin(16, 2);

  // Mostra animação de boas-vindas com o nome do grupo
  char logo[] = "----Stratfy-----------------------";
  animacaoPrint(logo, 200);
  delay(3000);

  // Limpa a tela aos poucos (efeito visual de saída)
  for (int i = 16; i > 0; i--) {
    lcd.setCursor(i, 0);
    lcd.print(" ");
    lcd.setCursor(i, 1);
    lcd.print(" ");
    delay(50);
  }

  // Inicia processo de calibração do LDR
  setupLDR();
  delay(3000);
}

void loop() {
  float somaLeiturasLDR = 0;
  float mediaLeituraLDR = 0;
  float percentualLuz   = 0;

  // Faz 10 leituras do LDR para suavizar ruídos (média)
  for (int i = 0; i < 10; i++) {
    somaLeiturasLDR += analogRead(pinoLDR);
    delay(tempoPrecisao);
  }

  // Calcula a média das leituras
  mediaLeituraLDR = somaLeiturasLDR / 10;

  // Converte a leitura para porcentagem com base nos limites calibrados
  percentualLuz = mapParseFloat(mediaLeituraLDR, limiteLuzMinima, limiteLuzMaxima, 0, 100);

  lcd.clear();
  lcd.print("Luz Atual(LDR)");
  lcd.setCursor(0, 1);
  lcd.print(percentualLuz, 2); // Mostra duas casas decimais
  lcd.print('%');

  if (percentualLuz <= 40) {
    // Nível ideal de luz para vinhos: LED verde ligado
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, LOW);
    noTone(pinoBuzzer);
  } else if (percentualLuz <= 70) {
    // Nível intermediário: alerta amarelo
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, HIGH);
    digitalWrite(ledVermelho, LOW);
    noTone(pinoBuzzer);
  } else {
    // Luz excessiva: alerta vermelho + buzzer por 3 segundos
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, HIGH);
    tone(pinoBuzzer, 1000); // Emite som em 1000 Hz
    delay(3000);
    noTone(pinoBuzzer);
  }
}

// Função personalizada para mapear valores float (equivalente ao map() do Arduino)
float mapParseFloat(float valor, float inMin, float inMax, float outMin, float outMax) {
  return (valor - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

// Realiza 10 leituras do LDR e retorna a média, aguardando o botão ser solto
int calibrarLDR() {
  int media = 0;

  do {
    int soma = 0;

    for (int i = 0; i < 10; i++) {
      soma += analogRead(pinoLDR);
      delay(tempoPrecisao);
    }

    media = soma / 10;
  } while (digitalRead(botaoCalibrar) == LOW); // Espera soltar o botão

  return media;
}

// Processo de calibração do sensor LDR, guiado por mensagens no display
void setupLDR() {
  lcd.clear();
  lcd.setCursor(0, 0);

  // Mensagem de início do setup
  char mensagem[] = "SETUP Sensor LDR";
  animacaoPrint(mensagem, 150);
  lcd.setCursor(0, 1);
  lcd.print("Pressione");

  // Espera botão ser pressionado e solto para continuar
  while (digitalRead(botaoCalibrar) == HIGH);
  delay(100);
  while (digitalRead(botaoCalibrar) == LOW);

  // Mensagem para definir luz mínima
  char mensagemMin[] = "Luz Min. Pressione";
  animacaoPrint(mensagemMin, 100);

  // Espera botão e realiza calibração mínima
  while (digitalRead(botaoCalibrar) == HIGH);
  delay(100);
  while (digitalRead(botaoCalibrar) == LOW);
  limiteLuzMinima = calibrarLDR();

  // Mensagem para definir luz máxima
  char mensagemMax[] = "Luz Max. Pressione";
  animacaoPrint(mensagemMax, 100);

  while (digitalRead(botaoCalibrar) == HIGH);
  delay(100);
  while (digitalRead(botaoCalibrar) == LOW);
  limiteLuzMaxima = calibrarLDR();

  // Confirmação no display
  lcd.clear();
  lcd.print("Calibragem OK!");
  delay(1500);
}

// Função que imprime texto no LCD com efeito de digitação
// Automaticamente quebra linha após 15 caracteres
void animacaoPrint(char mensagem[], int tempo) {
  lcd.clear();

  for (int i = 0; i < strlen(mensagem); i++) {
    lcd.print(mensagem[i]);
    delay(tempo);

    if (i > 14) {
      lcd.setCursor(i - 15, 1);
    }
  }
}
