#include <Wire.h>   
#include <RTClib.h>


void Reloj();

RTC_DS3231 rtc;
void setup() {
  Serial.begin(115200);
  if (! rtc.begin()) {       // si falla la inicializacion del modulo
 Serial.println("Modulo RTC no encontrado !");  // muestra mensaje de error
 while (1);         // bucle infinito que detiene ejecucion del programa
  }
}



void loop() {
  Reloj();
  delay(1000);
}

void Reloj(){
  DateTime fecha = rtc.now();  
  Serial.println(fecha.timestamp());
}
