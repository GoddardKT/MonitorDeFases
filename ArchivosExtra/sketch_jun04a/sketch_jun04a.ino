#include <Wire.h>  
#include <RTClib.h>

#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include <Filters.h>

float testFrequency = 60;                     // Frecuencia (Hz)
float windowLength = 40.0 / testFrequency;   // promedio de la señal
 
int Sensor = 0;
float intercept = -0.01; // to be adjusted based on calibration testing
float slope = 0.0235; // to be adjusted based on calibration testing
float volts; // Voltage

unsigned long periodo = 500;
unsigned long tiempoAnterior = 0;

String Guardar = "";
char dataBuffer[100]; // Buffer para almacenar los datos

RTC_DS3231 rtc;

void writeFile(fs::FS &fs, const char * path, String message){
  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("Fallo al abrir archivo para escritura.");
    return;
  }
  if(file.println(message)){
    Serial.println("Archivo escrito");
    digitalWrite(15,0);
  } else {
    Serial.println("Fallo de escritura");
  }
  file.close();
}

void setup() {
  pinMode(14,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(15,OUTPUT);
  Serial.begin(115200);
  if (! rtc.begin()) {       // si falla la inicializacion del modulo
    Serial.println("Modulo RTC no encontrado!");  // muestra mensaje de error
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
  if ((unsigned long)(millis() - tiempoAnterior) >= periodo) {
    float V1 = leerVoltaje(34,14);
    float V2 = leerVoltaje(35,12);
    float V3 = leerVoltaje(32,13);
    String HoraFecha = Reloj();
    
    snprintf(dataBuffer, sizeof(dataBuffer), "%s,%f,%f,%f", HoraFecha.c_str(), V1, V2, V3);
    Guardar += dataBuffer;

    tiempoAnterior = millis();
  }

  if (Guardar.length() > 0) {
    Serial.println(Guardar);
    digitalWrite(15,1);
    delay(100);
    writeFile(SD, "/Val.csv",Guardar);
    Guardar = "";
  }
}

String Reloj(){
  DateTime fecha = rtc.now();
  char HoraFecha[20];
  sprintf(HoraFecha, "%02d:%02d:%02d,%02d/%02d/%02d", fecha.hour(), fecha.minute(), fecha.second(), fecha.day(), fecha.month(), fecha.year());
  String Tiempo = String(HoraFecha);
  return Tiempo;
}



float leerVoltaje(int pin, int led) {
  digitalWrite(led,1);
  RunningStatistics inputStats;
  inputStats.setWindowSecs(windowLength);
  int i = 0;

  while (true) {
    Sensor = analogRead(pin);  // Leer pin analógico
    inputStats.input(Sensor);
    if ((unsigned long)(millis() - tiempoAnterior) >= periodo) {
      volts = intercept + slope * inputStats.sigma(); // Offset y amplitud
      volts = volts * (40.3231);                      // Calibración
      tiempoAnterior = millis();
         i++;
     if (i == 10) {
        digitalWrite(led,0);
        break;
      }
  }
  return volts;
}
}
