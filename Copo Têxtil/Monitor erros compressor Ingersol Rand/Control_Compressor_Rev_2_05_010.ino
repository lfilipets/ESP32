// PROGRMA ESP32 - MONITOR ERROS COMPRESSOR INGERSOL RAND 160

// AUTOR: Filipe Teixeira
// EMAIL: lfilipets@gmail.com
// TLM: +351 962 197 960
// EMPRESA: GRUPO COPO - Copo Têxtil Portugal
// DEPARTAMENTO: Departamento de Manutenção
// DATA: 25/05/2025
// REV: 2.50.010
// DESCRIÇÃO: 
// Implementação de código para envio de mensagens WhatsApp via Library CallMeBot
// Inserção de funcionalidade da Data e Hora através de servidor NTP

//--------------------------------------------------------------------------------------

// Inclusão de Librarias

    #include <WiFi.h>                    // Libraria Wifi
    #include <Wire.h>                    // Libraria comunicação I2C
    #include <LiquidCrystal_I2C.h>       // Libraria para controle do LCD
    #include <Callmebot_ESP32.h>         // Libraria CallMeBot (envio de mensagens WhatsWapp)
    #include <NTPClient.h>               // Libraria NTP (servidor de hora mundial)
 
//--------------------------------------------------------------------------------------

// Configurações do LCD I2C 1602

   LiquidCrystal_I2C lcd(0x27, 16, 2); 

//--------------------------------------------------------------------------------------

// Configuração da rede WiFi ( para comutar entre redes ativar ou desativar as linhas correspondentes )

   const char *ssid= "LFTS-TPLINK";            // Define o SSID ( Placa 4G TP-Link)
   const char *password= "29011984";           // Define a PassWord ( Placa 4G TP-Link)
   //const char *ssid= "LFTS-HOME";            // Define o SSID ( Wifi Casa)
   //const char *password = "LFTS@2024";       // Define a PassWord ( Wifi Casa)

//--------------------------------------------------------------------------------------

// Configuração de Strings para o serviço CallMeBot

// Telemóveis:

   String phoneNumber01 = "+351962197960";
   String phoneNumber02 = "+351910574192";

// API Keys:

   String apiKey01 = "7979071";
   String apiKey02 = "1724402";

// Mensagens:

   String messsage01 = "SISTEMA LIGADO";
   String messsage02 = "COMPRESSOR OK";
   String messsage03 = "COMPRESSOR LIGADO COM SUCESSO";
   String messsage04 = "COMPRESSOR DESLIGADO OFF";
   String messsage05 = "COMPRESSOR EM ALARME - O compressor está em alarme por favor verificar e fazer RESET ao Sistema ";
   String messsage06 = "";

//-------------------------------------------------------------------------

// Configurações de hardware e variáveis

   int btnGPIO = 0;
   int btnState = false;
   int LED = 2;                    // Pino D2 LED Interno
   int ERRO_01 = 13;               // Entrada ERRO01 ( Compressor OFF )
   int ERRO_02 = 32;               // Entrada ERRO02 ( Compressor em alarme )
   int COMP_OK = 33;               // Entrada OK ( Compressor sem erros )
   unsigned long DelayPisca;       // Variável para o pisca do LED interno com a função MILLIS
   unsigned long DelayLCD;         // Variável para alternar entre Hora e "SISTEMA OK" no LCD
   int State_ERRO_01 = 0;
   int LastState_ERRO_01 = 0;
   int State_ERRO_02 = 0;
   int LastState_ERRO_02 = 0;
   int State_OK = 0;
   int LastState_OK = 0;

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

//************* SETUP ***************
void setup() {
  Serial.begin(9600);                       // Inicia comunicação Serial com o PC via USB
  lcd.init();                               // Inicia o display LCD 
  lcd.backlight();                          // Liga backlight do LCD
  lcd.clear();                               // Limpa o display do LCD
  delay(10);           

  pinMode(ERRO_01, INPUT_PULLUP);           // Entrada ERRO01 ( Compressor OFF )
  pinMode(ERRO_02, INPUT_PULLUP);           // Entrada ERRO02 ( Compressor em alarme )
  pinMode(COMP_OK, INPUT_PULLUP);           // Entrada OK ( Compressor sem erros )
  pinMode(btnGPIO, INPUT_PULLUP);           // Entrada botão interno pull-up
  pinMode(LED, OUTPUT);                     // Saida LED interno Azul

 // Exibe mensagem inicial no LCD
     lcd.setCursor(0, 0);
     lcd.print("Conectar WiFi");
     lcd.setCursor(0, 1);
     lcd.print(ssid); // [1][5]



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

// Ativação do LED Interno do ESP32
  digitalWrite(LED, HIGH);

// ERRO 01 ( Compressor Desligado )

   State_ERRO_01 = digitalRead(ERRO_01);

  if (State_ERRO_01 == HIGH && LastState_ERRO_01 == LOW){
  
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("    ERRO 01    ");
        lcd.setCursor(0, 1);
        lcd.print("Compressor OFF");

        Serial.println("Compressor Desligado OFF");
        Serial.println("Compressor Desligado verificar arranque do sistema");

        Callmebot.whatsappMessage(phoneNumber01, apiKey01, messsage04);
	      Serial.println(Callmebot.debug());
        delay(10);
  }  
        LastState_ERRO_01 = State_ERRO_01;
        delay(100);

// ERRO 02 ( compressor em alarme )

  State_ERRO_02 = digitalRead(ERRO_02);

  if (State_ERRO_02 == HIGH && LastState_ERRO_02 == LOW){
  
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("    ERRO 02    ");
        lcd.setCursor(0, 1);
        lcd.print("Compressor Alarm");

        Serial.println("Compressor em alarme");
        Serial.println("Compressor em alarme verificar erros e efetuar RESET ao sistema");

        Callmebot.whatsappMessage(phoneNumber01, apiKey01, messsage05);
	      Serial.println(Callmebot.debug());
        delay(10);
  }  
        LastState_ERRO_02 = State_ERRO_02;
        delay(100);

// ESTADO OK ( Compressor sem erros )

State_OK = digitalRead(COMP_OK);

if ((State_ERRO_01 == LOW && LastState_ERRO_01 == HIGH) && (State_ERRO_02 == LOW && LastState_ERRO_02 == HIGH)){
  
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("     SISTEMA    ");
        lcd.setCursor(0, 1);
        lcd.print("       OK       ");

        Serial.println("Sistema OK");
        Serial.println("Compressor em alarme verificar erros e efetuar RESET ao sistema");

        Callmebot.whatsappMessage(phoneNumber01, apiKey01, messsage02);
	      Serial.println(Callmebot.debug());
        delay(10);

  }  
        LastState_OK = State_OK;
        delay(100);

}

