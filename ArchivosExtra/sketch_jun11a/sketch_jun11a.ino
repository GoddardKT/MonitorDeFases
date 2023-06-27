#include <Wire.h>
#include <RTClib.h>

#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include <Filters.h>

float testFrequency = 60;                     // Frecuencia (Hz)
float windowLength = 40.0 / testFrequency;    // promedio de la señal

int Sensor = 0; //A0
float intercept = -0.04; // to be adjusted based on calibration testing
float slope = 0.0105; // to be adjusted based on calibration testing
float volts; // Voltage

unsigned long periodo = 100;
unsigned long tiempoAnterior = 0;

RTC_DS3231 rtc;

const char* filePath = "/Val.csv"; // Ruta del archivo .csv

File dataFile; // Objeto File para escribir en el archivo

void writeFile(fs::FS &fs, const char *path, const String &message) {
  Serial.printf("Escribiendo archivo: %s\n", path);
  Serial.println("Escribiendo.....");
  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Fallo al abrir archivo para escritura.");
    return;
  }
  if (file.println(message)) {
    Serial.println("Archivo escrito");
  } else {
    Serial.println("Fallo de escritura");
  }
  file.close();
}

void setup() {
  delay(5000);
  Serial.begin(115200);
  
  if (!rtc.begin()) {
    Serial.println("Modulo RTC no encontrado!");
  }

  if (!SD.begin(SS)) {
    Serial.println("Fallo en montar tarjeta");
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("SD no encontrada");
    return;
  }
  
  // Abrir el archivo en modo de escritura (se sobrescribirá si ya existe)
  dataFile = SD.open(filePath, FILE_WRITE);
  if (!dataFile) {
    Serial.println("Fallo al abrir el archivo");
    return;
  }
  
  // Escribir los encabezados de las columnas en el archivo .csv
  dataFile.println("Fecha,Hora,Voltaje1,Voltaje2,Voltaje3");
  
  // Cerrar el archivo
  dataFile.close();
}

void loop() {
  float V1 = Voltaje1();
  float V2 = Voltaje2();
  float V3 = Voltaje3();
  
  String HoraFecha = Reloj();
  
  // Crear una cadena con los valores de fecha, hora y voltajes separados por comas
  String dataLine = HoraFecha + "," + String(V1) + "," + String(V2) + "," + String(V3);
  
  // Abrir el archivo en modo de escritura, pero en modo de agregar al final del archivo
  dataFile = SD.open(filePath, FILE_WRITE | FILE_APPEND);
  if (dataFile) {
    // Escribir la línea de datos en el archivo
    dataFile.println(dataLine);
    dataFile.close();
    Serial.println("Datos escritos en el archivo");
  } else {
    Serial.println("Fallo al abrir el archivo");
  }
  
  delay(1000);
}


String Reloj(){
  DateTime fecha = rtc.now();
  char HoraFecha[20];  
  sprintf(HoraFecha,"%02d:%02d:%02d,%02d/%02d/%02d",fecha.hour(), fecha.minute(), fecha.second(),fecha.day(), fecha.month(), fecha.year());  
  String Tiempo=String(HoraFecha);
  return Tiempo;
}

float Voltaje1(){
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


float Voltaje2(){
  RunningStatistics inputStats;
  inputStats.setWindowSecs(windowLength);
  int i=0;

  while (true) {
    Sensor = analogRead(35);  //Leer pin Analógico
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

float Voltaje3(){
  RunningStatistics inputStats;
  inputStats.setWindowSecs(windowLength);
  int i=0;

  while (true) {
    Sensor = analogRead(32);  //Leer pin Analógico
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
