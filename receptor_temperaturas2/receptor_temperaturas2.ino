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
#define key1 A3 //Boton 1
#define key2 A2 //Boton 2
#define key3 A5 //Boton 3
#define key4 A4 //Boton 4

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
  Serial.begin(115200);
  delay(3000);

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

  printMenu();
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

  getButtonPress();
  
  //you should make this delay shorter then your transmit delay or else messages could be lost
  delay(250);


  digitalWrite(LED, LOW);
}

void setDisplayTemp(void) {

  // text display
  lcd.writeFillRect(1,1,238,40,WHITE);
  lcd.setTextColor(BLACK);
  lcd.setCursor(3, 3);
  lcd.setTextSize(2);
  lcd.print("Temperatura = ");
  lcd.println(mydata.temp);
  lcd.print("Tiempo F = " );
  lcd.println(mydata.timer);


}

void setDisplayFail(void) {

  // text display
  lcd.writeFillRect(1,1,238,40,WHITE);
  lcd.setTextColor(BLACK);
  lcd.setCursor(3, 3);
  lcd.setTextSize(2);
  lcd.println("Sin datos");

}

void getButtonPress(void) {

  // Pulsar boton
  int buttonState1 = digitalRead(key1);
  int buttonState2 = digitalRead(key2);
  int buttonState3 = digitalRead(key3);
  int buttonState4 = digitalRead(key4);

  if (buttonState1 == 0) {
    printMenuRecepcion();
  }

  if (buttonState1 == 0) {
  
  }

  if (buttonState1 == 0) {
  
  }

  if (buttonState1 == 0) {
  
  }
  

  Serial.println(buttonState1);
  Serial.println(buttonState2);
  Serial.println(buttonState3);
  Serial.println(buttonState4);

}

void printMenu(void) {

  lcd.writeFillRect(0,0,240,40,WHITE);
  lcd.writeFillRect(0,40,120,100,RED);
  lcd.writeFillRect(121,40,120,100,BLUE);
  lcd.writeFillRect(0,141,120,100,GREEN);
  lcd.writeFillRect(121,141,120,100,YELLOW);
  
  lcd.setTextSize(5);
  lcd.setTextColor(WHITE);
  
  lcd.setCursor(46, 60);
  lcd.print("1");

  lcd.setCursor(168, 60);
  lcd.print("2");

  lcd.setCursor(46, 160);
  lcd.print("3");

  lcd.setCursor(168, 160);
  lcd.print("4");

  lcd.setTextSize(2);
  lcd.setCursor(6, 100);
  lcd.print("RECEPCION");

  lcd.setCursor(140, 100);
  lcd.print("EMISION");

  lcd.setCursor(41, 200);
  lcd.print("LOG");

  lcd.setCursor(145, 200);
  lcd.print("CONFIG");

}

void printMenuRecepcion(void) {

  lcd.fillScreen(WHITE);
  lcd.writeFillRect(0,0,240,40,RED);

  lcd.setTextSize(2);
  lcd.setTextColor(WHITE);
  
  lcd.setCursor(3, 3);
  lcd.print("1 - Recepcion");

}
