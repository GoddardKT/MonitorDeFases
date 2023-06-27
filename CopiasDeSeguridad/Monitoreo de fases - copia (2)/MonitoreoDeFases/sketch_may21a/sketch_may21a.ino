#include <Wire.h>  
#include <RTClib.h>

#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include <Filters.h>

float testFrequency = 60;                     // Frecuencia (Hz)
float windowLength = 40.0 / testFrequency;   // promedio de la señal
 
int Sensor = 0; //A0
float intercept = -0.04; // to be adjusted based on calibration testing
float slope = 0.0105; // to be adjusted based on calibration testing
float volts; // Voltage

unsigned long periodo = 100;
unsigned long tiempoAnterior = 0;


String Reloj();
float Voltaje();
String Guardar="";

RTC_DS3231 rtc;


void writeFile(fs::FS &fs, const char * path, String message){
  Serial.printf("Escirbiendo archivo: %s\n", path);
  Serial.println("Escribiendo.....");
  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Fallo para abrir archivo para estritura.");
    return;
  }
  if(file.println(message)){
    Serial.println("Archivo Escrito");
  } else {
    Serial.println("Fallo de escritura");
  }
  file.close();
}


void setup() {
  Serial.begin(115200);
  if (! rtc.begin()) {       // si falla la inicializacion del modulo
  Serial.println("Modulo RTC no encontrado !");  // muestra mensaje de error
  }

  if(!SD.begin(SS)){
    Serial.println("Fallo en montar tarjeta");
    return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("SD no encontrada");
    return;
  }
}



void loop() {
  float V=Voltaje();
  String Volt=String(volts);
  String HoraFecha=Reloj();
  //Serial.println(HoraFecha);
  
  //Serial.println(Volt);
  String Contatenar=HoraFecha + "," + Volt + "\n";
  Guardar+=Contatenar;

  Serial.println(Guardar);
  writeFile(SD, "/Val.csv",Guardar);
  delay(1000);
 //Voltaje();
}

String Reloj(){
  DateTime fecha = rtc.now();
  char HoraFecha[20];
  //char Hora[10];
  //char Fecha[10];  
  sprintf(HoraFecha,"%02d:%02d:%02d,%02d/%02d/%02d",fecha.hour(), fecha.minute(), fecha.second(),fecha.day(), fecha.month(), fecha.year());  
  String Tiempo=String(HoraFecha);
  return Tiempo;
}

float Voltaje(){
  RunningStatistics inputStats;
  inputStats.setWindowSecs(windowLength);
  int i=0;

  while (true) {
    Sensor = analogRead(34);  //Leer pin Analógico
    inputStats.input(Sensor);
    if ((unsigned long)(millis() - tiempoAnterior) >= periodo) {
      volts = intercept + slope * inputStats.sigma(); //offset y amplitud
      volts = volts * (40.3231);                      //calibración
      tiempoAnterior = millis();
      
      i++;
      if (i==100){
        break;
      } 
    }
  }

  return volts;
}
