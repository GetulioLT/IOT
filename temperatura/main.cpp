// Inclui a biblioteca do sensor DHT
#include "DHT.h"
// Inclui a biblioteca para o display LCD I2C
#include <LiquidCrystal_I2C.h>

// Define o pino do sensor DHT11
#define DHTPIN 4
// Define o tipo do sensor DHT como DHT11
#define DHTTYPE DHT11

// Cria uma instância do sensor DHT
DHT dht(DHTPIN, DHTTYPE);

// Endereço I2C do display LCD
#define Adress 0x27
// Número de colunas do LCD
#define Col 16
// Número de linhas do LCD
#define Line 2

// Define os pinos para os botões de navegação
#define butAnt 15 // Botão Anterior
#define butDep 5  // Botão Próximo

// Variáveis para armazenar o estado atual dos botões
int butAntState;
int antButAntState = 0; // Estado anterior do botão Anterior

int butDepState;
int antButDepState = 0; // Estado anterior do botão Próximo

// Variável para controlar a tela atual no display
int tela = 0;

// Cria uma instância do display LCD
LiquidCrystal_I2C lcd(Adress, Col, Line);

// Caracter personalizado para o símbolo de grau
byte grau[8] = {
  B00000,
  B01110,
  B01010,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000
};

void setup() {
  // Inicializa o sensor DHT
  dht.begin();
  // Inicializa a comunicação Serial
  Serial.begin(115200);
  // Inicializa o LCD
  lcd.init();
  // Liga a luz de fundo do LCD
  lcd.backlight();
  // Cria o caractere personalizado no LCD
  lcd.createChar(0, grau);
  // Configura os pinos dos botões como entrada
  pinMode(butAnt, INPUT);
  pinMode(butDep, INPUT);
}

// Variáveis para armazenar os dados de temperatura e umidade
float h, c, f;
// Variáveis para armazenar os índices de calor
float hif, hic;

// Função para ler dados do sensor DHT
void ReadDHT() {
  h = dht.readHumidity(); // Lê a umidade
  c = dht.readTemperature(); // Lê a temperatura em Celsius
  f = dht.readTemperature(true); // Lê a temperatura em Fahrenheit

  // Calcula os índices de calor em Fahrenheit e Celsius
  hif = dht.computeHeatIndex(f, h, true);
  hic = dht.computeHeatIndex(c, h, false);
}

// Função para imprimir os dados no Serial Monitor
void PrintSerial() {
  Serial.print("Humidade: ");
  Serial.print(h);
  Serial.println("%");

  Serial.print("Temperatura: ");
  Serial.print(c);
  Serial.println("°C");

  Serial.print("Temperatura: ");
  Serial.print(f);
  Serial.println("°F");

  Serial.print("Indice de Calor: ");
  Serial.print(hic);
  Serial.println("°C");

  Serial.print("Indice de Calor: ");
  Serial.print(hif);
  Serial.println("°F");

  Serial.println();
}

// Função para mudar a tela com base na entrada dos botões
void MudarTela()
{
  // Lê o estado atual dos botões
  butAntState = digitalRead(butAnt); // Botão Anterior
  butDepState = digitalRead(butDep); // Botão Próximo

  // Verifica se o estado do botão Próximo mudou
  if(butDepState != antButDepState)
  {
    antButDepState = butDepState; // Atualiza o estado anterior
    if(butDepState == HIGH) // Se o botão Próximo foi pressionado
        tela++; // Avança para a próxima tela
  }
  // Verifica se o estado do botão Anterior mudou
  else if(butAntState != antButAntState)
  {
    antButAntState = butAntState; // Atualiza o estado anterior
    if(butAntState == HIGH) // Se o botão Anterior foi pressionado
        tela--; // Retorna para a tela anterior
  }

  // Mantém o número da tela dentro dos limites
  if(tela < 0) tela = 3; // Se menor que 0, vai para a última tela
  else if(tela > 3) tela = 0; // Se maior que 3, volta para a primeira tela
}

// Função para exibir informações no LCD com base na tela atual
void PrintLCD() {
  switch(tela) {
    case 0: // Tela inicial
      lcd.clear();
      lcd.print("Sensor DHT11");
      lcd.setCursor(0, 1);
      lcd.print("Treinamento");
      break;
    case 1: // Tela de umidade
      lcd.clear();
      lcd.print("Umidade: ");
      lcd.print(h);
      lcd.print("%");
      break;
    case 2: // Tela de temperatura
      lcd.clear();
      lcd.print("Temp: ");
      lcd.print(c);
      lcd.write(0); // Símbolo de grau
      lcd.print("C");
      lcd.setCursor(0, 1);
      lcd.print("Temp: ");
      lcd.print(f);
      lcd.write(0); // Símbolo de grau
      lcd.print("F");
      break;
    case 3: // Tela de índice de calor
      lcd.clear();
      lcd.print("Ind. C: ");
      lcd.print(hic);
      lcd.print("%");
      lcd.setCursor(0, 1);
      lcd.print("Ind. F: ");
      lcd.print(hif);
      lcd.print("%");
      break;
  }
  delay(250); // Pequena pausa para evitar flickering
}

void loop() {
  ReadDHT(); // Lê os dados do sensor
  // Verifica se os dados lidos são válidos
  if (isnan(h) || isnan(c) || isnan(f)) {
    Serial.println("Falha no sensor!!!");
    return;
  }
  //PrintSerial(); // Descomente para ativar a impressão no Serial Monitor
  MudarTela(); // Verifica a entrada dos botões para mudar a tela
  PrintLCD(); // Atualiza o display LCD com as informações
  //delay(2000); // Descomente para adicionar um delay no loop
}
