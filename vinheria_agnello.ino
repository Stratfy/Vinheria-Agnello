#include <LiquidCrystal.h>
#include <string.h>

LiquidCrystal lcd(12,11,10,9,8,7); //Declara os pinos usados no header LiquidCrystal

//DECLARAÇÃO DOS PINOS COMO VÁRIAVEIS
/*Uso de const short tipo - Como são váriaveis que não vão mudar seu valor
se usa constantes, também, como são números pequenos, se usa short para economizar memória*/
const short int LDR = A0;
const short int ledRed = 6;
const short int ledYellow = 5;
const short int ledGreen = 4;
const short int buzzer = 3;
const short int button = 2;
int luzMinima = 1023,luzMaxima = 0;
const int precisao = 100; // milisegundos
float velocidade = 15;

byte customChars[][8]={{
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
},{
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000
},{//sol 1 #2
  B00000,
  B00011,
  B01111,
  B01111,
  B01111,
  B01111,
  B00011,
  B00000
},{//sol 1 #3
  B00000,
  B11000,
  B11110,
  B11110,
  B11110,
  B11110,
  B11000,
  B00000
},{//sol 2 #4
  B00000,
  B00011,
  B01100,
  B01000,
  B01000,
  B01100,
  B00011,
  B00000
},{//sol 2 #5
  B00000,
  B11000,
  B00110,
  B00010,
  B00010,
  B00110,
  B11000,
  B00000
},{//sol 3 #6
  B00101,
  B10011,
  B01100,
  B01000,
  B01000,
  B01100,
  B10011,
  B00101
},{//sol3 #7
  B10100,
  B11001,
  B00110,
  B00010,
  B00010,
  B00110,
  B11001,
  B10100
}};



void setup() {
  //Setup dos Pinos
  pinMode(LDR, INPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  lcd.begin(16,2);
  
  
  //Animação de entrada
  char logo[] = "----Stratfy-----------------------";
  animacaoPrint(logo,200);
  delay(3000);
  transicao();
  
  //Chama a função que faz o setup do LDR
  setupLDR();
  delay(3000);

}

void loop() {
  float leituraLDR, somaMedia = 0, leituraLDRporCem;
  
  //Faz a média das leituras do LDR
  for(int i = 0; i < 10; i++){
    somaMedia += analogRead(LDR);
    delay(precisao);
  }
  leituraLDR = somaMedia/10;

  //Transforma a média em porcentagem
  leituraLDRporCem = mapParseFloat(leituraLDR,luzMinima,luzMaxima,0,100);


  //Display da porcentagem de luz
  lcd.clear();
  lcd.print("Luz Atual(LDR)");
  lcd.setCursor(0,1);
  lcd.print(leituraLDRporCem,2);
  lcd.print('%');


   if (leituraLDRporCem <= 40) {
    // Luz adequada: LED verde aceso
    lcd.createChar(4,customChars[2]);
    lcd.setCursor(13,1);
    lcd.write(byte(4));
    lcd.createChar(5,customChars[3]);
    lcd.setCursor(14,1);
    lcd.write(byte(5));
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, LOW);
    noTone(buzzer); // Buzzer desligado
  } else if (leituraLDRporCem <= 70) {
    // Luz em nível de alerta: LED amarelo aceso
    lcd.createChar(4,customChars[4]);
    lcd.setCursor(13,1);
    lcd.write(byte(4));
    lcd.createChar(5,customChars[5]);
    lcd.setCursor(14,1);
    lcd.write(byte(5));
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, HIGH);
    digitalWrite(ledRed, LOW);
    noTone(buzzer); // Buzzer desligado
  } else {
    // Luz excessiva: LED vermelho aceso + buzzer por 3s
    lcd.createChar(4,customChars[6]);
    lcd.setCursor(13,1);
    lcd.write(byte(4));
    lcd.createChar(5,customChars[7]);
    lcd.setCursor(14,1);
    lcd.write(byte(5));
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, HIGH);
    tone(buzzer, 1000); // Buzzer toca em 1kHz
    delay(3000);        // Dura 3 segundos
    noTone(buzzer);     // Desliga o buzzer após o tempo
  }
  
}


//Funções

//Função Map só que com números de ponto flutuante
float mapParseFloat(float var,float inMin, float inMax, float outMin, float outMax){
  return(var-inMin)*(outMax-outMin)/(inMax-inMin)+outMin;
}

/*Função que faz a calibragem, retorna o a média de 10 leituras do LDR, feita para evitar a repetição
desse bloco de código na função setup*/
int calibragem(){
    int luz;
    do{
    int somaMedia = 0;
    for(int i = 0; i < 10; i++){
      somaMedia += analogRead(LDR);
      delay(precisao);
      }
    luz = somaMedia/10;
  }while(digitalRead(button) == LOW);
  return luz;
}


//Função com a parte visual e setup dos valores minimos e máximos do LDR
void setupLDR() {
  lcd.clear();
  lcd.setCursor(0, 0);
  char mensagem[] = "SETUP Sensor LDR";
  animacaoPrint(mensagem, 150);
  lcd.setCursor(0,1);
  lcd.print("Pressione");
  
  
  while(digitalRead(button) == HIGH);
  delay(100);
  while(digitalRead(button) == LOW);
  

  // Etapa: instrução para luz mínima
  transicao();
  char mensagem2[] = "Luz Min. Pressione";
  animacaoPrint(mensagem2, 100);
  
  // Espera o botão ser pressionado
  while (digitalRead(button) == HIGH);
  delay(100); // debounce
  while (digitalRead(button) == LOW);  // espera soltar

  luzMinima = calibragem();

  // Etapa: instrução para luz máxima
  transicao();
  char mensagem3[] = "Luz Max. Pressione";
  animacaoPrint(mensagem3, 100);

  while (digitalRead(button) == HIGH);
  delay(100); // debounce
  while (digitalRead(button) == LOW);  // espera soltar

  luzMaxima = calibragem();

  transicao();
  lcd.clear();
  lcd.print("Calibragem OK!");
  delay(1500);
}


//Função que imprime as mensagens de forma mais bonita, imprime letra a letra e quebra linha
//parâmetros formais: mensagem e tempo em milisegundos do delay entre as letras
void animacaoPrint(char mensagem[],int tempo){
  lcd.clear();
  for(int i = 0; i< strlen(mensagem);i++){
    lcd.print(mensagem[i]);
    delay(tempo);
    if(i>14)lcd.setCursor(i-15,1);
  }
}

/*Função que cria a barra e faz a animação dela em um espaço usando operadores
bit a bit, ou seja portabilizada e com o menor uso de memória*/

void barra(int cursor){
  lcd.write(byte(0));
  delay(velocidade);
  for(int i = 0; i<5; i++){
    lcd.createChar(1,customChars[1]);
    lcd.setCursor(cursor,0);
    lcd.write(byte(1));
    lcd.setCursor(cursor,1);
    lcd.write(byte(1));
    for(int j = 0; j<8; j++)customChars[1][j] = customChars[1][j] >> 1;
    delay(velocidade);
  }
  for(int j = 0; j<8; j++)customChars[1][j] = B10000;
}

/*função que junta e justifica a animação de transição de abas, repete a 
função barra por toda a tela*/
void transicao(void){
   for(int i = 0; i<16; i++){
    lcd.setCursor(i-1,0);
    lcd.write(byte(0));
    lcd.setCursor(i-1,1);
    lcd.write(byte(0));
    barra(i);
  }
  lcd.clear();
}










