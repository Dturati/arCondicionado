#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SPI.h>
#include <DHT.h>
//#include <FloatToString.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include "DHT.h"
#include <EtherCard.h>


//confuguração de sensor de temperatura
//#define dht_dpin A1 //Pino DATA do Sensor ligado na porta Analogica A1
//dht DHT; //Inicializa o sensor
#define DHTPIN A1 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE);

//confugurações de internet 
static byte mymac[] = {  0x00,0x01,0x01,0x01,0x01,0x01  };
static byte myip []  = { 192,168,206,200 };
byte Ethernet::buffer[400];
BufferFiller bfill;
//botoes configuração internet
char* on = "LIGADO";
char* off = "DESLIGADO";
char* subir = "SUBIR";
char* descer = "DESCER";
char* statusLabel;
char* buttonLabel;
char* myTemp = "Temperatura teste";
char temp[10];
char hum[10];
int arStatus;
int subirTemp;
int descerTemp;


IRsend irsend;
const int tm = 198;
typedef const int16_t PROGMEM prog_int16_t;
//vetores liga e desliga
prog_int16_t desligar[198] = {6150,7350,550,1650,550,1650,550,1600,600,1650,550,1600,600,1600,600,1600,600,1600,550,550,550,600,500,650,550,550,600,550,550,550,600,550,550,550,600,1600,500,1700,600,1600,600,1600,600,1600,550,1650,550,1650,550,1600,550,600,500,650,500,600,600,550,550,550,600,550,550,550,600,550,550,1650,550,1650,550,1650,500,1700,550,1600,600,1600,600,1600,600,1600,600,550,550,550,550,600,550,550,600,550,500,600,500,650,550,550,600,1650,550,1600,600,550,550,1650,550,1650,550,1650,550,1600,600,1600,550,600,500,600,600,1600,600,550,550,550,600,550,550,600,550,550,550,1650,550,1650,550,1650,550,1650,500,1650,600,550,600,1600,500,1700,550,550,600,550,550,600,550,550,550,600,550,1600,600,550,550,600,550,550,600,1600,500,600,600,1600,600,550,600,1600,550,550,600,550,550,1650,550,600,550,1600,600,550,550,1650,550,550,600,1600,600,1600,550,7400,600,};
prog_int16_t ligar1[tm]  = {6050,7300,550,1600,600,1600,600,1550,600,1600,600,1600,550,1600,550,1650,550,1600,550,600,550,550,550,550,600,550,550,550,600,500,600,550,550,550,600,1600,550,1600,600,1600,550,1600,600,1600,550,1600,600,1600,550,1650,550,550,550,550,600,550,550,550,550,550,600,550,550,550,600,500,600,1600,600,1550,600,1600,600,1600,550,1600,600,1600,550,1600,600,1600,550,550,600,500,600,550,550,550,600,550,550,550,550,550,600,550,550,1600,600,550,550,1600,600,550,550,1600,550,600,550,1600,550,1650,550,550,550,1600,600,550,550,1600,600,550,550,1600,550,600,550,550,550,1650,550,1600,550,1650,550,1600,550,1650,550,550,550,1650,550,1600,550,600,550,550,550,550,600,550,550,550,600,1550,600,550,550,550,600,550,550,1600,600,550,550,1600,600,500,600,1600,600,500,600,550,550,1600,600,550,550,1600,600,500,600,1600,550,550,600,1600,550,1600,600,7300,600,};

//vetores temperatura
prog_int16_t temp17[tm]  = {6050,7300,550,1650,550,1600,550,1650,550,1600,550,1650,550,1600,600,1600,550,1600,550,600,550,550,550,550,600,550,550,550,550,600,550,550,550,550,600,1600,550,1650,550,1600,550,1650,550,1600,550,1600,600,1600,550,1600,600,550,550,550,550,600,550,550,550,600,550,550,550,550,600,550,550,1600,600,500,600,1600,550,1600,600,1600,600,1600,550,1600,600,1600,550,550,550,1650,550,550,550,550,600,550,550,550,550,550,600,550,600,1550,600,550,550,1600,550,600,600,1550,550,600,550,1600,550,1600,600,550,550,1600,600,550,550,1600,600,550,550,1600,550,600,550,550,550,600,550,1600,550,1650,550,1600,550,1650,550,550,550,1650,550,1600,550,1650,550,550,600,550,550,550,550,550,550,1650,550,550,550,550,600,550,550,1600,600,550,550,1600,550,550,600,1600,550,550,600,550,550,1600,600,550,550,1600,600,550,550,1600,550,600,550,1600,550,1650,550,7350,550,};
prog_int16_t temp18[tm]  = {6050,7300,600,1550,550,1650,600,1550,600,1600,600,1600,600,1550,600,1600,600,1550,600,550,550,550,500,600,600,550,500,600,600,500,600,550,550,550,600,1600,550,1600,600,1600,550,1600,600,1600,550,1600,600,1600,600,1550,600,550,550,550,600,500,600,550,600,500,500,650,600,500,500,650,600,1550,550,600,600,1550,500,1700,600,1550,550,1650,600,1550,500,1700,600,500,600,1600,600,500,600,500,600,550,600,500,600,500,600,550,550,1600,600,550,550,1600,600,500,600,550,600,500,600,1600,600,1550,600,550,600,1550,600,550,600,1550,600,1600,600,1550,600,550,600,500,550,1650,600,500,550,1650,550,1600,550,1650,600,500,550,1650,550,1600,500,600,600,1600,550,550,600,550,500,600,600,1550,600,550,600,500,600,550,600,1550,600,550,600,1550,600,550,600,1550,600,500,650,500,600,1550,600,550,600,1550,600,550,600,1550,600,500,650,1550,550,1650,600,7250,600,};
prog_int16_t temp19[tm]  = {6000,7350,500,1700,600,1550,550,1650,600,1550,550,1650,600,1600,550,1600,600,1600,550,550,600,500,600,550,550,550,600,500,600,550,550,550,600,550,550,1600,550,1650,550,1600,600,1600,550,1600,600,1600,550,1600,600,1600,550,550,600,550,550,550,550,600,550,550,500,600,600,500,550,600,600,1550,600,550,550,1600,550,1650,600,1550,550,1650,600,1550,550,1650,600,500,600,1600,550,550,500,650,550,550,500,650,600,500,600,500,600,1600,500,600,600,1600,600,500,600,500,600,550,550,1600,600,1600,550,550,600,1600,550,550,600,1600,550,1600,600,1600,550,550,500,650,550,550,600,500,600,1600,600,1550,600,1600,600,500,600,1600,550,1600,600,1600,500,1650,600,550,500,600,600,500,550,1650,600,500,600,550,550,550,600,1600,550,550,550,1650,550,550,600,1600,550,550,550,550,600,1600,550,550,600,1600,500,600,600,1550,650,500,600,1550,550,1650,600,7300,600,};
prog_int16_t temp20[tm]  = {6050,7350,600,1550,600,1600,600,1550,600,1600,600,1550,600,1600,600,1550,600,1600,600,500,600,550,600,500,500,650,600,500,500,600,600,500,600,550,600,1550,600,1600,600,1550,600,1600,600,1550,550,1650,600,1600,600,1550,600,500,600,550,600,500,600,550,600,500,600,500,600,550,600,500,600,1600,600,500,550,1650,600,1550,600,1600,600,1550,600,1600,600,1550,600,550,600,1550,600,550,600,500,550,550,600,550,550,550,600,500,550,1650,600,500,550,1650,600,500,550,600,600,500,550,1650,600,1550,550,600,550,1600,550,550,600,1600,550,1600,600,1600,550,550,600,550,550,1600,600,1600,550,550,600,1600,550,1600,600,500,600,1600,600,1550,600,550,600,500,600,1600,600,500,550,600,600,1550,600,550,600,500,550,550,600,1600,550,550,600,1600,550,550,600,1600,550,550,600,500,550,1650,600,500,550,1650,600,500,550,1650,600,500,600,1600,600,1550,600,7300,600,};

//vetores desce temperatura
prog_int16_t temp16[tm]  =  {6000,7350,550,1650,450,1700,600,1600,500,1650,550,1650,500,1650,600,1600,500,1700,550,550,500,600,500,650,550,550,500,600,550,600,500,600,550,600,450,1700,550,1650,450,1700,550,1650,550,1600,500,1700,550,1600,550,1650,550,550,500,650,550,550,500,600,550,600,450,650,550,600,450,650,550,550,500,650,450,1700,550,1650,450,1700,550,1650,450,1700,550,1650,450,1750,550,1600,500,600,550,600,450,650,550,550,500,650,550,550,500,1700,550,550,550,1600,500,650,550,1600,600,550,550,1600,500,1700,550,550,500,1700,550,550,550,1650,550,550,550,1600,600,550,550,550,600,1600,550,1600,600,1600,550,1600,600,1600,550,550,600,1600,550,1600,600,550,550,550,500,650,550,550,550,600,500,1650,550,550,550,600,550,550,550,1650,550,550,550,1650,550,550,550,1600,550,600,550,550,550,1650,550,550,550,1600,600,550,550,1600,500,650,550,1600,500,1700,550,7350,550,};

//ledo ar ligado
int ligado = 5;

//botoes pinos
int botao = 2;
int botaoSobe = 4;
int botaoDesce = 7;
int botaoTime = 9;

//estado de botoes
int estadoBotao = 0;
int estadoBotaoSobeTemp = 0;
int estadoBotaoDesceTemp = 0;
int estadoBotaoTime = 0;
//variáveis contadoras
int arLigado = 0;

//temperatura inicial
int temperaturaAr = 20;

long previousMillis = 0;        // Variável de controle do tempo
long redLedInterval = 6000;     // Tempo em ms do intervalo a ser executado


void setup() {
  Serial.begin(9600);
  pinMode(ligado,OUTPUT);
  pinMode(botao,INPUT);
  pinMode(botaoSobe,INPUT); 
  dht.begin();
  
  //configurações de internet
  if (ether.begin(sizeof Ethernet::buffer, mymac, 8) == 0){
    Serial.println("Nao eh possivel inicializar ethernet.");
}else{
    Serial.println("Ethernet inicializado.");
    ether.staticSetup(myip);
    ether.printIp("Meu IP: ", ether.myip);
    ether.printIp("GW IP: ", ether.gwip);
    arStatus = false;
}
 
}


void loop() {     
    acender();
    char teste[10];
    float flt = temperatura();
    int temp = 0;
    int fat = 1;
    char p;
    
    word pos = ether.packetLoop(ether.packetReceive());
    if(pos) {
        for(int c = pos; Ethernet::buffer[c]; c++){
          if( (char)Ethernet::buffer[c] == 't' ){
              p = c+2;
              while((char)Ethernet::buffer[p] != ' '){
                 //Serial.println((char)Ethernet::buffer[p]);
                 if((char)Ethernet::buffer[p] - '0' == 0){
                   temp = temp * fat;
                 }
                 temp += ((char)Ethernet::buffer[p] - '0') * fat;
                 
                 fat = fat*10;
                 p++; 
              }
              
              //temp = temp*1000;
              
              break;
           }
        }
        //Serial.print( (char *)Ethernet::buffer + pos );
        Serial.println(temp);
        word n;
        switch(Ethernet::buffer[pos+5]){
        case 'A': n=TemperaturaAmbiente(temperatura(),humidade());
        break;
        case 'B': n = funcaoTimer(temp);
        break;
        default:n=homePage(pos);
        
 
      }
      ether.httpServerReply(n);
   }

}

//sensor de humidade e temperatura
float temperatura(){
                float h = dht.readHumidity();
                float t = dht.readTemperature();
                // testa se retorno é valido, caso contrário algo está errado.
                if (isnan(t) || isnan(h)) 
                {
                    //Serial.println("Failed to read from DHT");
                }else{
                  //Serial.println("Temperatura");
                  //return Serial.println(t); 
                  return t;
                }
}

//sensor de humidade e temperatura
float humidade(){
                float h = dht.readHumidity();
                float t = dht.readTemperature();
                // testa se retorno é valido, caso contrário algo está errado.
                if (isnan(t) || isnan(h)) 
                {
                    //Serial.println("Failed to read from DHT");
                }else{
                  //Serial.println("Temperatura");
                  //return Serial.println(t); 
                  return h;
                }
}

//Ligar pelos botoes na protoboard
void acender(){
  estadoBotao = digitalRead(botao);
  estadoBotaoTime = digitalRead(botaoTime);
  
  //condição que faz com que o ar ligue
  if(estadoBotao == HIGH){
    delay(250);
    	digitalWrite(ligado,HIGH);
    	
    	if(estadoBotao == HIGH && arLigado == 0){
          digitalWrite(ligado,HIGH);
          arLigado = 1;
          estadoBotao = LOW;
          ligaAr();
        }
        
        
  }
  
    //condição que faz com que o ar desloga
  if(estadoBotao == HIGH){
    
    	digitalWrite(ligado,HIGH); 
    	
    	if(estadoBotao == HIGH && arLigado == 1){
          digitalWrite(ligado,LOW);
          arLigado = 0;
          estadoBotao = LOW;
          desligaAr();
        }
  }       
        desceTemperatura();
        sobeTemperatura();
   
   
   //desliga com o timer   
  if(estadoBotaoTime == HIGH){
    timerDesliga();
  }
        
  }
  
  //Ar liga na temperatura 20 graus
  void ligaAr(){
    unsigned int rawCode1[tm];
       for(int i=0; i<tm; i++){
            rawCode1[i]= (pgm_read_word(&temp20[i]));
        }
     arStatus = true;
     irsend.sendRaw(rawCode1,tm,38);
     
  }
  
  void desligaAr(){
    unsigned int rawCode[tm];
       for(int i=0; i<tm; i++){
            rawCode[i]= (pgm_read_word(&desligar[i]));   
        }
     arStatus = false;
     irsend.sendRaw(rawCode,198,38);
  }
  
 void sobeTemperatura(){
   unsigned int rawCode[tm];
   estadoBotaoSobeTemp = digitalRead(botaoSobe);
  
   if(estadoBotaoSobeTemp == HIGH)
   if(estadoBotaoSobeTemp == HIGH && temperaturaAr == 16 ){
     for(int i=0; i<tm; i++){
            rawCode[i]= (pgm_read_word(&temp17[i]));
     }  
     temperaturaAr = 17;
     irsend.sendRaw(rawCode,tm,38);
     estadoBotaoSobeTemp = LOW;
     delay(250);
   }
   
   
    if(estadoBotaoSobeTemp == HIGH && temperaturaAr == 17 ){
     for(int i=0; i<tm; i++){
            rawCode[i]= (pgm_read_word(&temp18[i]));
     }  
     temperaturaAr = 18;
     irsend.sendRaw(rawCode,tm,38);
     estadoBotaoSobeTemp = LOW;
     delay(250);
   }
   
    if(estadoBotaoSobeTemp == HIGH && temperaturaAr == 18 ){
     for(int i=0; i<tm; i++){
            rawCode[i]= (pgm_read_word(&temp19[i]));
     }  
     temperaturaAr = 19;
     irsend.sendRaw(rawCode,tm,38);
     delay(250);
     estadoBotaoSobeTemp = LOW;
     
   }
   
    if(estadoBotaoSobeTemp == HIGH && temperaturaAr == 19 ){
     for(int i=0; i<tm; i++){
            rawCode[i]= (pgm_read_word(&temp20[i]));
     }  
     temperaturaAr = 20;
     irsend.sendRaw(rawCode,tm,38);
     delay(250);
     estadoBotaoSobeTemp = LOW;
   }
   
   
 }
 
void desceTemperatura(){
  unsigned int rawCode[tm];
  estadoBotaoDesceTemp = digitalRead(botaoDesce);
  
   if(estadoBotaoDesceTemp == HIGH &temperaturaAr == 20){
    for(int i=0; i<tm; i++){
            rawCode[i]= (pgm_read_word(&temp19[i]));
     }  
     irsend.sendRaw(rawCode,tm,38);
     delay(250);
     temperaturaAr = 19;
     estadoBotaoDesceTemp = LOW;
  }
  
  
   if(estadoBotaoDesceTemp == HIGH && temperaturaAr == 19){
    for(int i=0; i<tm; i++){
            rawCode[i]= (pgm_read_word(&temp18[i]));
     }  
     irsend.sendRaw(rawCode,tm,38);
     delay(250);
     temperaturaAr = 18;
     estadoBotaoDesceTemp = LOW;
  }
  
  if(estadoBotaoDesceTemp == HIGH && temperaturaAr == 18){
    for(int i=0; i<tm; i++){
            rawCode[i]= (pgm_read_word(&temp17[i]));
     }  
     irsend.sendRaw(rawCode,tm,38);
     delay(250);
     temperaturaAr = 17;
     estadoBotaoDesceTemp = LOW;
  }
  
  if(estadoBotaoDesceTemp == HIGH && temperaturaAr == 17){
    for(int i=0; i<tm; i++){
            rawCode[i]= (pgm_read_word(&temp16[i]));
     }  
     irsend.sendRaw(rawCode,tm,38);
     delay(250);
     temperaturaAr = 16;
     estadoBotaoDesceTemp = LOW;
  }
 
  
}



void timerDesliga(){
  unsigned int rawCode[tm];
  
   for(int i=0; i<tm; i++){
            rawCode[i]= (pgm_read_word(&desligar[i]));
     } 
     estadoBotaoTime = LOW;
     delay(1000);
     digitalWrite(ligado,LOW);
     temperaturaAr = 20;
     arStatus =0;
     estadoBotaoSobeTemp = LOW;
     irsend.sendRaw(rawCode,tm,38);
}


//apartir daqui comela a parte de web
static word homePage(int pos){ 
    int sensor;
    //arStatus = -1; //ar está neutro
    unsigned int rawCode[tm];  
    bfill = ether.tcpOffset();
    
    
    
    if(strstr((char *)Ethernet::buffer + pos, "GET /?status=SUBIR") != 0) {
      //Serial.println("SUBIR");
      arStatus = 2; 
      
    }
    
     if(strstr((char *)Ethernet::buffer + pos, "GET /?status=DESCER") != 0) {
      //Serial.println("DESCER");
      arStatus = 3; 
    }
      
      if(strstr((char *)Ethernet::buffer + pos, "GET /?status=LIGADO") != 0) {
        //Serial.println("LIGADO");
        arStatus = 1;
        digitalWrite(ligado,HIGH);
      }
    
    
    if(strstr((char *)Ethernet::buffer + pos, "GET /?status=DESLIGADO") != 0) {
      Serial.println("DESLIGADO");
      arStatus = 0;
      temperaturaAr = 16;
      digitalWrite(ligado,LOW);
    }
       
    
    //subir temperatura
    //se o ar estiver no status de subir temperatura
    if(arStatus == 2){
       Serial.println("Subindo");
       if(temperaturaAr == 16 && arStatus == 2 ){
         for(int i=0; i<tm; i++){
            rawCode[i]= (pgm_read_word(&temp17[i]));
         }  
           temperaturaAr = 17;
           irsend.sendRaw(rawCode,tm,38);
           arStatus = -1; // manda ar para estado -1 que é neutro
           //delay(250);
        }
        
       
       if(temperaturaAr == 17 && arStatus == 2){
         for(int i=0; i<tm; i++){
            rawCode[i]= (pgm_read_word(&temp18[i]));
         }  
           temperaturaAr = 18;
           irsend.sendRaw(rawCode,tm,38);
           arStatus = -1;
           //delay(250);
       }
   
      if(temperaturaAr == 18 && arStatus == 2){
       for(int i=0; i<tm; i++){
            rawCode[i]= (pgm_read_word(&temp19[i]));
       }  
         temperaturaAr = 19;
         irsend.sendRaw(rawCode,tm,38);
         arStatus = -1;
         //delay(250);
       }
   
      if(temperaturaAr == 19 && arStatus == 2){
        for(int i=0; i<tm; i++){
            rawCode[i]= (pgm_read_word(&temp20[i]));
        }  
         temperaturaAr = 20;
         irsend.sendRaw(rawCode,tm,38);
         arStatus = -1;
         //delay(250);
       }
       
    }
    
    //descer temperatura
    //se o ar estiver no estutus 3 de descer temperatura
    if(arStatus == 3){
      if(temperaturaAr == 20 && arStatus == 3){
        for(int i=0; i<tm; i++){
            rawCode[i]= (pgm_read_word(&temp19[i]));
         }  
         irsend.sendRaw(rawCode,tm,38);
         //delay(250);
         temperaturaAr = 19;
         arStatus = -1; //manda o ar no estatus neutro
    }
  
  
   if(temperaturaAr == 19 && arStatus == 3){
    for(int i=0; i<tm; i++){
            rawCode[i]= (pgm_read_word(&temp18[i]));
     }  
     irsend.sendRaw(rawCode,tm,38);
     //delay(250);
     temperaturaAr = 18;
     arStatus = -1;
    }
  
    if(temperaturaAr == 18 && arStatus == 3){
      for(int i=0; i<tm; i++){
            rawCode[i]= (pgm_read_word(&temp17[i]));
      }  
     irsend.sendRaw(rawCode,tm,38);
     //delay(250);
     temperaturaAr = 17;
     arStatus = -1;
  }
  
    if(temperaturaAr == 17 && arStatus == 3){
      for(int i=0; i<tm; i++){
            rawCode[i]= (pgm_read_word(&temp16[i]));
     }  
       irsend.sendRaw(rawCode,tm,38);
       //delay(250);
       temperaturaAr = 16;
       arStatus = -1;
     }
      
    }
    
    if(arStatus == 1) {
      statusLabel = on;
      buttonLabel = off;
      for(int i=0; i<tm; i++){
            rawCode[i]=(pgm_read_word(&temp20[i]));
        }
      irsend.sendRaw(rawCode,tm,38);
    }
    
    if(arStatus == 0){
      statusLabel = off;
      buttonLabel = on;
      for(int i=0; i<tm; i++){
            rawCode[i]=(pgm_read_word(&desligar[i]));
        }
        irsend.sendRaw(rawCode,tm,38);
        arStatus = 0;
        temperaturaAr = 20;
    }
   
   
    bfill = ether.tcpOffset();
    bfill.emit_p(PSTR(
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/html\r\nPragma: no-cache\r\n\r\n"
      "<html>"
      "<head>"
      "<title>Ar condicionado lab hardware</title>"
      "</head>"
      "<body>"
      "<p> Status do AR: $S </p>"
      "<a href=\"/?status=$S\"><input type=\"button\" value=\"$S\"></a>"
      "<br>"
      "<a href=\"/?status=SUBIR\"><input type=\"button\" value=\"SUBIR\"> </a>"
      "<a href=\"/?status=DESCER\"><input type=\"button\" value=\"DESCER\"> </a>"
      "<br>"
      "<a href='/A' target='_blank'>Temperatura</a>"
      "<br>"
      "<a href='/B' target='_blank'>Programar tempo</a>"
      "<br>"
      "</body>"
      "</html>"
      ), statusLabel, buttonLabel, buttonLabel);
      
}

static word TemperaturaAmbiente(float flt, float hm){
      int inteiro = (int) flt;
      int inteiro2 = (int) hm;
      snprintf(temp,10, "%d", inteiro);
      snprintf(hum,10, "%d", inteiro2);
    
     bfill = ether.tcpOffset();
     bfill.emit_p(PSTR(
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/html\r\nPragma: no-cache\r\n\r\n"
      "<html>"
      "<head>"
      "<title>Ar condicionado lab hardware</title>"
      "<meta http-equiv=\"refresh\" content=\"10\">"
      "</head>"
      "<p> Temperatura <p>"
      "<p>  $S </p>"
      "<p> Humidade <p>"
      "<p>  $S </p>"
      "</body>"
      "</html>"
      ),temp,hum);
     
      return bfill.position();   
}

void timerWeb(int tempo){
  unsigned int rawCode[tm];
  
   for(int i=0; i<tm; i++){
            rawCode[i]= (pgm_read_word(&desligar[i]));
     } 
     estadoBotaoTime = LOW;
     delay(tempo);
     digitalWrite(ligado,LOW);
     
     //Marca temperatura inicial como 16
     temperaturaAr = 16;
     estadoBotaoSobeTemp = LOW;
     irsend.sendRaw(rawCode,tm,38);
     
     //Define status do ar como desligado
     arStatus = 0;
     
}

//Função para desligar o ar com um tempo definido
static word funcaoTimer(int tempo){
 
    if( tempo != 0){
        timerWeb(tempo*1000);
        //Serial.print(tempo);
    }
     
     bfill = ether.tcpOffset();
     bfill.emit_p(PSTR(
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/html\r\nPragma: no-cache\r\n\r\n"
      "<html>"
      "<head>"
      "<title>Programar tempo desligar </title>"
      "</head>"
      "<form method=get>"
      "Tempo: <input type=text name=t>"
      "<input type=submit value=Enviar>"
      "</form>"
      "</body>"
      "</html>"
      ));
      return bfill.position();   
}

