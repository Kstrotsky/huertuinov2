#include <SoftEasyTransfer.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Arduino_ST7789_Fast.h>
#include <SoftwareSerial.h>

//Objeto de transmision de datos serial
SoftEasyTransfer ET; 
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

//Pines botones
#define key1 A2 //connect wire 1 to pin 2
#define key2 A3 //connect wire 2 to pin 3
#define key3 A4 //connect wire 3 to pin 4
#define key4 A5 //connect wire 4 to pin 5

struct RECEIVE_DATA_STRUCTURE{
  int timer;
  float temp;
  int station; 
};

//give a name to the group of data
RECEIVE_DATA_STRUCTURE mydata;

int failCounter = 0;
int scroll = 0;

//inicializamos el display
Arduino_ST7789 lcd = Arduino_ST7789(TFT_DC, TFT_RST);


void setup(){
  
  mySerial.begin(9600);
  delay(2000);
  Serial.begin(115200);

  lcd.init(SCR_WD, SCR_HT);
  lcd.fillScreen(BLACK);
  lcd.setCursor(0, 0);
  lcd.setTextColor(WHITE);
  lcd.setTextSize(2);
  lcd.println("Iniciando Transmisor");

  delay(2000);

  //pines transmisor
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT); 
  pinMode(AUX, INPUT);

  pinMode(LED, OUTPUT);

  // mode 0
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);

  // Botones mando
  pinMode(key1, INPUT_PULLUP);
  pinMode(key2, INPUT_PULLUP);
  pinMode(key3, INPUT_PULLUP);
  pinMode(key4, INPUT_PULLUP);
  
  ET.begin(details(mydata), &mySerial);

  lcd.println("Transmisor iniciado");

  Serial.println("Iniciado");
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


  digitalWrite(LED, LOW);
}

void setDisplayTemp(void) {

  // text display
  lcd.fillScreen(BLACK);
  lcd.setCursor(0, 0);
  lcd.print("Temperatura = ");
  lcd.println(mydata.temp);
  lcd.print("Tiempo F = " );
  lcd.println(mydata.timer);


}

void setDisplayFail(void) {

  // text display
  lcd.fillScreen(BLACK);
  lcd.setCursor(0, 0);
  lcd.println("Sin datos");

}
