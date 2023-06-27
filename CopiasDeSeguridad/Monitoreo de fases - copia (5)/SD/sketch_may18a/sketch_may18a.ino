#include "FS.h"
#include "SD.h"
#include "SPI.h"

const int chipSelect = SS;//10;

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\n", path);
  Serial.println("WRITEFILE.....");
  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}


void setup(){
  Serial.begin(115200);

  delay(2000);
  if(!SD.begin(SS)){
 // if(!SD.begin(5)){
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }

  writeFile(SD, "/hello.txt", "Hello ");
  delay(2000);

}

void loop(){
  for(int i = 0; i<=100; i++){
  char str[8];
  itoa( i, str, 10 );

  writeFile(SD, "/hello.txt", str);
  
  delay(5000);
  Serial.println(str);
  }
}
