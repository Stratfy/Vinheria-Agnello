#include <LiquidCrystal.h>

LiquidCrystal lcd(12,11,10,9,8,7); //Declara os pinos usados no header LiquidCrystal

//DECLARAÇÃO DOS PINOS COMO VÁRIAVEIS
/*Uso de const short tipo - Como são váriaveis que não vão mudar seu valor
se usa constantes, também, como são números pequenos, se usa short para economizar memória*/
const short int LDR = A0;
const short int ledRed = 6;
const short int ledYellow = 5;
const short int ledGreen = 4;
const short int buzzer = 3;
float mapParseFloat(float var,float inMin, float inMax, float outMin, float outMax);


void setup() {
  pinMode(LDR, INPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(buzzer, OUTPUT);
 
  //lcd setup
  lcd.begin(16,2);
  lcd.print("Stratfy");
}

void loop() {
//Leitura e média do LDR

float leituraLDR;
float somaMedia = 0;
for(int i = 0; i < 10; i++){
  somaMedia += analogRead(LDR);
  delay(100);
}
leituraLDR = somaMedia/10;

lcd.clear();
lcd.print(mapParseFloat(leituraLDR,1023,0,0,100));



}

float mapParseFloat(float var,float inMin, float inMax, float outMin, float outMax){
  return(var-inMin)*(outMax-outMin)/(inMax-inMin)+outMin;
}