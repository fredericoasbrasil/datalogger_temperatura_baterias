#include <Wire.h>
#include <RTClib.h>

#include <SD.h>
#include<SPI.h> 

#include "DHT.h"

//Define portas analógicas nas quais os DHTs estão ligados, no caso as analógicas 1 e 2
#define dhtA A1
#define dhtB A2
#define DHTTYPE DHT11 // Tipo do sensor: DHT 11

DHT sensorA(dhtA, DHTTYPE);
DHT sensorB(dhtB, DHTTYPE);

RTC_DS1307 RTC;
int chipSelect = 10; //pino chipSelect (CS) ligado na porta 10
File cartaoSD;

//Portas dos sensores de temperatura

String dadosSensores = "";

String getDataHora(){
  
    DateTime agora = RTC.now();//Recuperando a data e hora atual
    String dataHora = "";
    dataHora += agora.day();//Imprimindo o dia
    dataHora += "/";
    dataHora += agora.month();//Recuperando o mes
    dataHora += "/";
    dataHora += agora.year();//Recuperando o ano
    dataHora += ";";
    dataHora += agora.hour();//Recuperando a hora
    dataHora += ":";
    dataHora += agora.minute();//Reci[erando os minutos
    dataHora += ":";
    dataHora += agora.second();//Recuperando os segundos
        
    return dataHora;
}


String getTemperatura(DHT sensor){
  float t = sensor.readTemperature();
  if (isnan(t)) 
  {
    return String("ERRO NA LEITURA DE TEMP - VERIFIQUE SENSOR");
  } else{ 
  return String(t);
  }
}

void gravaDados(File cartao, String dados ){
  cartao = SD.open("datalog.csv", FILE_WRITE);
  if(cartao){ //True se conseguir abrir o cartao e o arquivo, False caso contrário
    cartao.println(dados);
    cartao.close();
    Serial.println("Dados gravados no cartao SD");
} else { //Falha no arquivo ou cartao SD
  Serial.println("ERRO NO ARQUIVO OU CARTAO SD - DADOS NAO FORAM GRAVADOS");
}

}

void setup () {
    
    Serial.begin(9600);
    Wire.begin();
    RTC.begin();
    sensorA.begin();
    sensorB.begin();
    //Ajusta a data/hora do Clock com a data/hora em que o codigo foi compilado
    RTC.adjust(DateTime(__DATE__, __TIME__));
    
  //Verifica se o modulo RTC esta funcionando
  if (! RTC.isrunning()) {
    Serial.println("Módulo RTC não identificado");
    
    
  }
  
  SD.begin(chipSelect);
  dadosSensores = "######## INICIO DA GRAVACAO DE DADOS ###########;;;";
  //Grava um banner para identificar o inicio da sessão de dados
  gravaDados(cartaoSD, dadosSensores);
  
  
}  

void loop () {
    dadosSensores = getDataHora();
    dadosSensores += ";";
    dadosSensores += getTemperatura(sensorA);
    dadosSensores += ";";
    dadosSensores += getTemperatura(sensorB);
    Serial.println(dadosSensores);
    gravaDados(cartaoSD, dadosSensores);
    delay(500);//A leitura sera repetida cada 1 segundo
    dadosSensores = "";
}
