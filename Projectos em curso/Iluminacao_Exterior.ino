// Prgrama ESP32 Iluminação focos exteriores do lado do muro
// 18/08/2025 Rev: 1.00
// Horário de funcionamento das 20h30 às 01h00 
// Horário de Verão modo: ON

#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>  // Biblioteca para LCD I2C

const char* ssid     = "LFTS-TPLINK";
const char* password = "29011984";

WiFiUDP ntpUDP;
// UTC+0 para Portugal + ajuste manual horário de verão
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000);

const int relePin = 2; // Pino do relé

// Ajuste manual do horário de verão (1 para ativar, 0 para desativar)
const int horarioVerao = 1;

// Variáveis para controle dos impulsos diários
bool impulso_ligar = false;
bool impulso_desligar = false;

// Endereço padrão do LCD I2C 1602 é 0x27, tamanho 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  pinMode(relePin, OUTPUT);
  digitalWrite(relePin, HIGH);  // RELÉ desligado (assumindo LOW ativa)

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  timeClient.begin();

  lcd.init();                    
  lcd.backlight();     
  lcd.setCursor(0, 1);
  lcd.print("Luzes: OFF  ");          // Liga a luz de fundo do LCD e escreve a mensagem " Luzes: OFF"
}

void loop() {
  timeClient.update();
  int hora = timeClient.getHours();
  int minuto = timeClient.getMinutes();

  if (horarioVerao) {
    hora = (hora + 1) % 24;
  }

  // Zera os flags de impulso à meia-noite (00:00)
  //if (hora == 0 && minuto == 0) {
  //  impulso_ligar = false;
  //  impulso_desligar = false;
  //}

  // Impulso às 18h18
  if (!impulso_ligar && hora == 20 && minuto == 30) {
    digitalWrite(relePin, LOW);  // Liga relé
    delay(500);
    digitalWrite(relePin, HIGH); // Desliga relé
    impulso_ligar = true;
    Serial.println("Impulso Ligar");
    lcd.setCursor(0, 1);
    lcd.print("Luzes: ON  ");
  }

  // Impulso às 18h20
  if (!impulso_desligar && hora == 01 && minuto == 00) {
    digitalWrite(relePin, LOW);  // Liga relé
    delay(500);
    digitalWrite(relePin, HIGH); // Desliga relé
    impulso_desligar = true;
    Serial.println("Impulso Desligar");
    lcd.setCursor(0, 1);
    lcd.print("Luzes: OFF  ");
  }

  // Exibe no LCD a hora atual na primeira linha
  lcd.setCursor(0, 0);
  lcd.print("Hora: ");
  if (hora < 10) lcd.print('0');
  lcd.print(hora);
  lcd.print(":");
  if (minuto < 10) lcd.print('0');
  lcd.print(minuto);
  lcd.print("     "); // Espaços para limpar caracteres se necessário

  // Exibe o estado do relé na segunda linha
 // lcd.setCursor(0, 1);
  //if (digitalRead(relePin) == LOW) {
    //lcd.print("Luzes: ON  ");
 // } else {
  //  lcd.print("Luzes: OFF");
  //}

  delay(1000);
}
