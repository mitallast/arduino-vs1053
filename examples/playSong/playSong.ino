/**
 * @depends SdFat library https://code.google.com/p/sdfatlib/
 */
#include <SPI.h>
#include <SdFat.h>
#include <vs1053.h>  
#include <LiquidCrystal.h>

SdFat sd;
VS1053 vs1053;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void setup() {
  Serial.begin(115200);
  SPI.begin();
  if (!sd.begin(10, SPI_HALF_SPEED)) sd.initErrorHalt();
  vs1053.begin();
  lcd.begin(16, 2);
}


char filename[32];
SdFile song;

void loop() {
  song.openNext(sd.vwd(), O_READ);
  playSong();
}
void playSong(){
  byte buf[32];
  Serial.println("soft reset");
  vs1053.softReset();
  
  song.getFilename(filename);
  Serial.println(filename);
  lcd.setCursor(0, 1);
  lcd.print(filename);
  int i=0;
  while(1){
    i = song.read(buf, 32);
    vs1053.write_data(buf, i);
    if(i<32){
      vs1053.write_register(SPI_MODE, SM_OUTOFWAV);
      vs1053.send_zeroes(2048);
      break;
    }
    vs1053.await_data_request();
  }  
  song.close();
  Serial.println("done");
  vs1053.printDetails();
}
