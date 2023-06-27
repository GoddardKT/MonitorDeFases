#include <Filters.h>
float testFrequency = 60;                     // Frecuencia (Hz)
float windowLength = 40.0 / testFrequency;   // promedio de la señal

#include <Wire.h>
int Sensor = 0; //A0

float intercept = -0.04; // to be adjusted based on calibration testing
float slope = 0.0105; // to be adjusted based on calibration testing
float volts; // Voltage

unsigned long periodo = 1000;
unsigned long tiempoAnterior = 0;


void setup() {
  Serial.begin(115200);
}

void loop() {

  RunningStatistics inputStats;
  inputStats.setWindowSecs(windowLength);

  while (true) {
    Sensor = analogRead(34);  //Leer pin Analógico
    inputStats.input(Sensor);
    if ((unsigned long)(millis() - tiempoAnterior) >= periodo) {
      volts = intercept + slope * inputStats.sigma(); //offset y amplitud
      volts = volts * (40.3231);                      //calibración
      Serial.print("\tVoltage: ");
      Serial.println(volts);

      Serial.print("Voltaje RMS:");
      Serial.print(volts);
      tiempoAnterior = millis();
    }
  }

}
