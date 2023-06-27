/*  Datalogger_SD_1ch_b.ino   Programa modificado del código de ejemplo.

 Se requieren las siguientes conexiones
 CS -> D10  Valor por omisión
MOSI -> D11
MISO -> D12
CLK -> D13
VCC -> 5 V
GND -> GND

Debe haber un sensor analógico en el canal A0  */

#include <SPI.h>
#include <SD.h>

const int analogPin = 34;
int sensor = 0;
float temperatura = 0;

void setup() {
  // Inicializa la comunicación serial
  Serial.begin(115200);
  while (!Serial) {
    ; // Espera a que se inicie el puerto serial
  }
  Serial.print("Inicializando tarjeta SD ...");

  // Verifica que la tarjeta se inicialice
  if (!SD.begin()) {
    Serial.println("Fallo en tarjeta, o no hay tarjeta");
    return;  // No hace nada más:
  }
  Serial.println("Tarjeta lista");
  SD.remove("Val.txt");  //Borra el archivo previo
  SD.mkdir("arc.txt");

}

void loop() {
 String dataString = "";  // Cadena vacía
 sensor = analogRead(analogPin);  // Lee el sensor
 temperatura=500*sensor/1024.0;  //Temperatura en Celsius
 dataString += String(temperatura);   //Almacena en la cadena
  // Abre el archivo en la memoria SD
  File dataFile = SD.open("arc.txt", FILE_WRITE);
  // Revisa si el archivo está disponible para escribir:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // Envía la cadena también por el puerto serial
    Serial.println(dataString);
  }
  // Si el archivo no se encuentra abierto, manda un error:
  else {
    Serial.println("Error al abrir el archivo arc.txt");
  }
  
  delay(984);   // Toma un dato cada segundo
  //delay(59984);   // Toma un dato cada minuto
  //delay(299984);  // Toma un dato cada 5 minutos
  //delay(599984);  // Toma un dato cada 10 minutos 
}
