#include <EasyTransfer.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Arduino_ST7789_Fast.h>
#include <SoftwareSerial.h>

//Objeto de transmision de datos serial
EasyTransfer ET; 
//END

//GESTION SERIAL
SoftwareSerial mySerial(8, 9); // RX, TX
//END

//Pines Trnsmisor RF
#define M0 5
#define M1 4
#define AUX 3
#define LED 2

//Pines LCD
#define TFT_DC    7
#define TFT_RST   6
#define SCR_WD   240
#define SCR_HT   240   // 320 - to allow access to full 240x320 frame buffer

struct RECEIVE_DATA_STRUCTURE{
  int timer;
  float temp;
  int station; 
};

//give a name to the group of data
RECEIVE_DATA_STRUCTURE mydata;

int failCounter = 0;

//inicializamos el display
Arduino_ST7789 lcd = Arduino_ST7789(TFT_DC, TFT_RST);


void setup(){
  
  mySerial.begin(9600);
  delay(2000);
  Serial.begin(9600);

  lcd.init(SCR_WD, SCR_HT);
  lcd.fillScreen(BLACK);
  lcd.setCursor(0, 0);
  lcd.setTextColor(WHITE,BLUE);
  lcd.setTextSize(3);
  lcd.println("HELLO WORLD");

  delay(2000);

  //pines transmisor
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT); 
  pinMode(AUX, INPUT);

  pinMode(LED, OUTPUT);

  // mode 0
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
  
  ET.begin(details(mydata), &Serial);
  
}

void loop(){
  //check and see if a data packet has come in. 
  if(ET.receiveData()){

      Serial.print("Temperatura = " );
      Serial.print(mydata.temp);
      Serial.print("  Tiempo funcionamiento = " );
      Serial.print(mydata.timer);
      Serial.println("");
      
      digitalWrite(LED, HIGH);
      setDisplayTemp();
      failCounter = 0;
      
  }

  else {
    
      failCounter++; 

      if (failCounter >= 30) {
        
        setDisplayFail();
        
      }
    
  }
  
  //you should make this delay shorter then your transmit delay or else messages could be lost
  delay(250);

  display.clearDisplay();
  digitalWrite(LED, LOW);
}

void setDisplayTemp(void) {

  // text display


}

void setDisplayFail(void) {

  // text display


}
