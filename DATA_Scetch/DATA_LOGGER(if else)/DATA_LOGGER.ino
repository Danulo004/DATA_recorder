#include <SD.h>
#include <SPI.h>
#include <DS3231.h>
#include <EEPROM.h>

#define delay_time 200
File myFile;
DS3231 rtc(SDA, SCL);

#define pinCS 10        
int val = 0;
int val_adress = 0;
int del_adress = 1;
int n_times = 0;
unsigned long timing = 30*1000;
unsigned long delating_time = 30*1000;
unsigned long last_time = 0;
unsigned long last_2_time = 0;

String fileString = "";
String delString = "";

void setup() {
  pinMode(pinCS, OUTPUT);
  pinMode(A0, INPUT);     
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(2, INPUT_PULLUP);   //EEPROM reset buttom
  pinMode(3, OUTPUT);         //green led - new log
  pinMode(4, OUTPUT);         //red led --- creating a new file
  pinMode(5, OUTPUT);         //blue led --- delating
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
   
  //EEPROM starting
  int vall = EEPROM.read(val_adress);
  rtc.begin();
  
    fileString = "";
    fileString += vall;
    fileString += ".txt";
}
void loop() {
  if(SD.begin(pinCS)){
    if( digitalRead(2) != 1){
        for (int i = 0 ; i < EEPROM.length() ; i++) {
          EEPROM.write(i, 0);
        }
        for (int j = 0 ; j <= 255 ; j++) {
          int vall = EEPROM.read(val_adress);
          fileString = "";
          fileString += j;
          fileString += ".txt";
          SD.remove( fileString );
        }
        val = 0;
        n_times = 0;
      }
    int vall = EEPROM.read(val_adress);
    fileString = "";
    fileString += vall;
    fileString += ".txt";
      
    //Create a new file
    if( millis()-last_time > timing){
        last_time = millis();
          int vall = EEPROM.read(val_adress);
          vall++;
          EEPROM.update(val_adress, vall);
        fileString = "";
        fileString += vall;
        fileString += ".txt";
        digitalWrite(4, HIGH);
      }
    //Delate an old file
    if( millis() - last_2_time > delating_time){
        last_2_time = millis();
        int vall = EEPROM.read(val_adress);
        int n_times = EEPROM.read(del_adress);
         if( vall-5 > n_times){
            n_times++;
              delString = "";
              delString += n_times;
              delString += ".TXT";
            SD.remove( delString );
            EEPROM.update(del_adress, n_times);
            digitalWrite(5, HIGH);
          }
      }
    //LOGING
    myFile = SD.open( fileString, FILE_WRITE);
    if (myFile) {  
      myFile.print(rtc.getDateStr());
      myFile.print(" ");
      myFile.print(rtc.getTimeStr()); //int(rtc.getTemp());
      myFile.print(",");
      if(!digitalRead(6)){
          myFile.print(analogRead(0));
          myFile.print(",");
        }
      if(!digitalRead(7)){
          myFile.print(analogRead(1));
          myFile.print(",");
        }
      if(!digitalRead(8)){
          myFile.print(analogRead(2));
          myFile.print(",");
        }
      if(!digitalRead(9)){
          myFile.print(analogRead(3));
          myFile.print(",");
        }
      myFile.println(" ");
      digitalWrite(3, HIGH);
      myFile.close();
    }
    delay( delay_time/2 );
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    delay( delay_time/2 );
  }else{
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
    }
}






