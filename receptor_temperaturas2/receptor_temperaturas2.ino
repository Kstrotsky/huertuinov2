#include <EasyTransfer.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Objeto de transmision de datos serial
EasyTransfer ET; 

#define M0 5
#define M1 4
#define AUX 3
#define LED 2

#define OLED_RESET 12
Adafruit_SSD1306 display(OLED_RESET);

struct RECEIVE_DATA_STRUCTURE{
  int timer;
  float temp;
  int station; 
};

//give a name to the group of data
RECEIVE_DATA_STRUCTURE mydata;

int failCounter = 0;

void setup(){
  
  delay(2000);
  Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.display();
  delay(2000);

  display.clearDisplay();

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
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  
  display.println(mydata.temp, 1);
  display.setTextSize(1);
  display.setCursor(47,0);
  
  display.println((char) 247);
  display.setCursor(53,0);
  display.println('C');

  display.setTextSize(1);
  display.setCursor(0,22);
  
  display.println("Tiempo:");
  display.setCursor(41,22);

  int minutes = mydata.timer/60;

  display.println(minutes);

  display.setCursor(65,0);
  display.println("Estacion:");
  display.setTextSize(2);
  display.setCursor(65,22);
  display.println(mydata.station);

  
  display.display();

}

void setDisplayFail(void) {

  // text display
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  
  display.println("Sin Datos");
  display.display();

}
