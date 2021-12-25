
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EasyTransfer.h>

//Objeto de transmision de datos serial
EasyTransfer ET; 

#define ONE_WIRE_BUS 4 //Pin digital sensor temperatura

OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

#define M0 12
#define M1 11
#define AUX 10

struct SEND_DATA_STRUCTURE{
  int timer;
  float temp;
  int station;  
};

//give a name to the group of data
SEND_DATA_STRUCTURE mydata;

void setup() {
  // start serial port
  Serial.begin(9600);
  
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT); 
  pinMode(AUX, INPUT);

  // mode 0
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);

  ET.begin(details(mydata), &Serial);

   // Iniciar sensor de temperatura
   sensors.begin();

}

void loop() {

   sensors.requestTemperatures(); // Send the command to get temperatures

   mydata.temp= sensors.getTempCByIndex(0);
   mydata.timer = millis()/1000.0;
   mydata.station = 1;

   ET.sendData();

   delay(2000);

}
