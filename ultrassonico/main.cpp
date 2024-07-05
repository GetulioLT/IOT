// Inclui a biblioteca para o display LCD I2C
#include <LiquidCrystal_I2C.h>
// Inclui a biblioteca para o sensor ultrassônico
#include <NewPing.h>

// Define o endereço I2C do display LCD
#define address 0x27
// Define o número de linhas do LCD
#define row 2
// Define o número de colunas do LCD
#define colunm 16

// Cria uma instância do display LCD
LiquidCrystal_I2C lcd(address, colunm, row);

// Define os pinos para o sensor ultrassônico
#define TRIGGER_PIN 4
#define ECHO_PIN 5
// Define a distância máxima de medição em cm
#define MAX_DISTANCE 200

// Cria uma instância do sensor ultrassônico
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// Define o pino do LED
#define led 2

// Variáveis para armazenar as medições do sensor ultrassônico
int cm = 0; // Distância em centímetros
int us = 0; // Tempo de viagem do pulso ultrassônico em microssegundos
int in = 0; // Distância em polegadas

void setup(){
  // Inicia a comunicação Serial
  Serial.begin(115200);
  // Inicializa o LCD
  lcd.init();
  // Liga a luz de fundo do LCD
  lcd.backlight();
  // Posiciona o cursor no início do LCD
  lcd.home();

  // Configura o pino do LED como saída
  pinMode(led, OUTPUT);
}

// Função para realizar a medição com o sensor ultrassônico
void Sonar(){
  // Realiza uma medição e retorna o tempo em microssegundos
  us = sonar.ping();
  Serial.print("US: ");
  Serial.println(us);

  // Converte o tempo de viagem do pulso em distância em centímetros
  cm = sonar.ping_cm();
  Serial.print("CM: ");
  Serial.println(cm);

  // Converte o tempo de viagem do pulso em distância em polegadas
  in = sonar.ping_in();
  Serial.print("IN: ");
  Serial.println(in);
}

// Função para exibir a distância medida no LCD
void PrintLCD(){
  // Limpa o display LCD
  lcd.clear();

  // Exibe a distância medida em centímetros
  lcd.print("Distancia(cm):");
  lcd.setCursor(0, 1); // Move o cursor para a segunda linha
  lcd.print(cm);
}

// Função para controlar o LED com base na distância medida
void Ping(){
  // Se a distância medida for menor ou igual a 50 cm
  if (cm <= 50) {
    // Acende o LED
    digitalWrite(led, HIGH);
    // Espera um tempo proporcional à distância medida
    delay(cm * 10);
    // Apaga o LED
    digitalWrite(led, LOW);
    // Espera um tempo proporcional à distância medida
    delay(cm * 10);
  }
}

void loop(){
  // Realiza a medição com o sensor ultrassônico
  Sonar();
  // Exibe a distância medida no LCD
  PrintLCD();
  // Controla o LED com base na distância medida
  Ping();
  // Espera 100 milissegundos antes da próxima iteração
  delay(100);
}
