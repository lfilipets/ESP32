// PROGRMA ESP32 - MONITOR ERROS COMPRESSOR INGERSOL RAND 160
// AUTOR: Filipe Teixeira
// EMAIL: lfilipets@gmail.com
// TLM: +351 962 197 960
// EMPRESA: GRUPO COPO - Copo Têxtil Portugal
// DEPARTAMENTO: Departamento de Manutenção
// DATA: 16/05/2025
// REV: 1.04
// DESCRIÇÃO: 
// Implementação de código para envio de mensagens WhatsApp via Library CallMeBot
// Inserção de funcionalidade da Data e Hora através de servidor NTP

//------------------------------------------------------------------------

// Inclusão de Librarias

    #include <WiFi.h>              // Libraria Wifi
    #include <HTTPClient.h>
    #include <UrlEncode.h>
    #include <Wire.h>              // Libraria comunicação I2C
    #include <LiquidCrystal_I2C.h> // Libraria para controle do LCD
    #include <Callmebot_ESP32.h>   // Libraria CallMeBot (envio de mensagens WhatsWapp)
    #include <NTPClient.h>         // Libraria NTP (servidor de hora mundial)
    #include <TimeLib.h>


//-------------------------------------------------------------------------

// Configurações do LCD I2C 1602

    LiquidCrystal_I2C lcd(0x27, 16, 2); 

//-------------------------------------------------------------------------

// Configuração da rede WiFi

   const char *ssid = "HOME-LFTS";     // Define o SSID (nome da rede)
   const char *password = "LFTS@2024"; // Define a PassWord

//-------------------------------------------------------------------------

// Configuração de Strings para o serviço CallMeBot

// Telemóveis:

String phoneNumber01 = "+351962197960";

// API Keys:

String apiKey01 = "7979071";

// Mensagens:

String messsage01 = "SISTEMA LIGADO";
String messsage02 = "COMPRESSOR OK";
String messsage03 = "COMPRESSOR LIGADO COM SUCESSO";
String messsage04 = "COMPRESSOR DESLIGADO COM SUCESSO";
String messsage05 = "COMPRESSOR EM ERRO";
String messsage06 = "";

//-------------------------------------------------------------------------

// Configurações de hardware

int btnGPIO = 0;
int btnState = false;
int LED = 2;          // Pino D2 LED Interno

//------------------------------------------------------------------------

// Rotina para piscar texto "VERIFICAR ERROS"

void piscaVerificarErros(LiquidCrystal_I2C &lcd, int vezes, int intervalo) {
  for (int i = 0; i < vezes; i++) {
    lcd.setCursor(0, 1);
    lcd.print("VERIFICAR  ERROS"); // Mostra o texto
    delay(intervalo);
    lcd.setCursor(0, 1);
    lcd.print("                "); // Apaga o texto (16 espaços)
    delay(intervalo);
  }
}

//------------------------------------------------------------------------

// Rotina para piscar texto "SISTEMA OK"

void piscaSistemaOK(LiquidCrystal_I2C &lcd, int vezes, int intervalo) {
  for (int i = 0; i < vezes; i++) {
    lcd.setCursor(0, 1);
    lcd.print("   SISTEMA OK   "); // Mostra o texto
    delay(intervalo);
    lcd.setCursor(0, 1);
    lcd.print("                "); // Apaga o texto (16 espaços)
    delay(intervalo);
  }
  // No fim, garante que o texto fica visível
    lcd.setCursor(0, 1);
    lcd.print("   SISTEMA OK   ");
}

//------------------------------------------------------------------------

// Configuração servidor NTP para sincronização da hora

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pt.pool.ntp.org", 3600, 60000);
 
char Time[ ] = "TIME:00:00:00";
char Date[ ] = "DATE:00/00/2000";
byte last_second, second_, minute_, hour_, day_, month_;
int year_;

//------------------------------------------------------------------------



//************* SETUP ***************
void setup() {
  Serial.begin(9600);  // Inicia comunicação Serial com o PC via USB
  lcd.init();          // Inicia o display LCD 
  lcd.backlight();     // Liga backlight do LCD
  lcd.clear();         // Limpa o display do LCD
  delay(10);           

 // Exibe mensagem inicial no LCD
     lcd.setCursor(0, 0);
     lcd.print("Conectar WiFi");
     lcd.setCursor(0, 1);
     lcd.print(ssid); // [1][5]

  pinMode(btnGPIO, INPUT_PULLUP);  // Adicionado pull-up interno
  pinMode(LED, OUTPUT);

    Serial.println("\n[WiFi] Conectar a ");
    Serial.println(ssid);

   WiFi.begin(ssid, password);
   delay(5000);
   Callmebot.whatsappMessage(phoneNumber01, apiKey01, messsage01);
	 Serial.println(Callmebot.debug());
   while (true) {
    switch (WiFi.status()) {
      case WL_CONNECTED:
       Serial.println("[WiFi] Conectado!");
       Serial.print(ssid);
        delay(1000);
       Serial.print("IP: ");
       Serial.println(WiFi.localIP());
        
        // Atualiza LCD com sucesso
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("WiFi Conectado!");
        lcd.setCursor(0, 1);
        lcd.print(ssid); // [1][5]
          delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("WiFi Conectado!");
        lcd.setCursor(0, 1);
        lcd.print("IP: ");
        lcd.print(WiFi.localIP()); // [1][5]
          delay(5000);  
        lcd.clear();

        lcd.setCursor(0, 0);
        lcd.print("Filipe Teixeira");
        delay(1500);
        lcd.setCursor(0, 1);
        lcd.print("Copo Textil S.A");
          delay(2000);
        lcd.clear();

        lcd.setCursor(0, 0);
        lcd.print("*COMPRESSOR 160*");
          delay(1500);
        lcd.setCursor(0, 1);
        lcd.print("A ligar sistema");
        lcd.clear();
         
        lcd.setCursor(0, 0);
        lcd.print("*COMPRESSOR 160*");
          delay(1500);
        piscaVerificarErros(lcd, 10, 500); // Pisca 10 vezes, 500ms cada
          delay(1000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("*COMPRESSOR 160*");
          delay(100);
        piscaSistemaOK(lcd, 2, 1000); // Pisca 2 vezes, 1000ms cada
                 delay(5000);
    Callmebot.whatsappMessage(phoneNumber01, apiKey01, messsage02);
	 Serial.println(Callmebot.debug());
        return;


      
      // Casos de erro (exemplo para um caso)
      case WL_NO_SSID_AVAIL:
        Serial.println("[WiFi] SSID não encontrado");
        lcd.clear();
        lcd.print("Erro WiFi:");
        lcd.setCursor(0, 1);
        lcd.print("SSID Invalido");
        break; 
    }
  }



}

//************* LOOP ***************
void loop() {
  // Piscar LED (mantido original)
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);


  // Verificar botão
  btnState = digitalRead(btnGPIO);
  
  if (btnState == LOW) {
    Serial.println("[WiFi] Desconectando...");
    
    // Atualiza LCD
    lcd.clear();
    lcd.print("Desconectando");
    lcd.setCursor(0, 1);
    lcd.print("WiFi...");
    
    if (WiFi.disconnect(true, false)) {
      Serial.println("[WiFi] Desconectado!");
      lcd.clear();
      lcd.print("WiFi");
      lcd.setCursor(0, 1);
      lcd.print("Desconectado!"); // [1][5]
    }
    delay(1000);
  }
lcd.clear();
timeClient.begin();

timeClient.update();
  unsigned long unix_epoch = timeClient.getEpochTime();    // Get Unix epoch time from the NTP server
 
  second_ = second(unix_epoch);
  if (last_second != second_) {
 
 
    minute_ = minute(unix_epoch);
    hour_   = hour(unix_epoch);
    day_    = day(unix_epoch);
    month_  = month(unix_epoch);
    year_   = year(unix_epoch);
 
 
 
    Time[12] = second_ % 10 + 48;
    Time[11] = second_ / 10 + 48;
    Time[9]  = minute_ % 10 + 48;
    Time[8]  = minute_ / 10 + 48;
    Time[6]  = hour_   % 10 + 48;
    Time[5]  = hour_   / 10 + 48;
 
 
 
    Date[5]  = day_   / 10 + 48;
    Date[6]  = day_   % 10 + 48;
    Date[8]  = month_  / 10 + 48;
    Date[9]  = month_  % 10 + 48;
    Date[13] = (year_   / 10) % 10 + 48;
    Date[14] = year_   % 10 % 10 + 48;
 
    Serial.println(Time);
    Serial.println(Date);
 
    lcd.setCursor(0, 0);
    lcd.print(Time);
    lcd.setCursor(0, 1);
    lcd.print(Date);
    last_second = second_;
 
  }
  delay(5000);
  lcd.clear();
  piscaSistemaOK(lcd, 5, 1000); // Pisca 5 vezes, 1000ms cada


}
