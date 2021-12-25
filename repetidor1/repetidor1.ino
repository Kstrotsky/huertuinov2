#include <EasyTransfer.h>

//Objeto de transmision de datos serial
EasyTransfer ETin, ETout; 

#define M0 5
#define M1 4
#define AUX 3

struct RECEIVE_DATA_STRUCTURE{
  int timer;
  float temp;
  int station; 
};

struct SEND_DATA_STRUCTURE{
  int timer;
  float temp;
  int station; 
};

//give a name to the group of data
RECEIVE_DATA_STRUCTURE rxdata;
SEND_DATA_STRUCTURE txdata;

int failCounter = 0;

void setup(){
  
  Serial.begin(9600);

  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT); 
  pinMode(AUX, INPUT);

  // mode 0
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
  
  ETin.begin(details(rxdata), &Serial);
  ETout.begin(details(txdata), &Serial);
  
}

void loop(){

  //check and see if a data packet has come in. 
  if(ETin.receiveData()){

        txdata.timer = rxdata.timer;
        txdata.temp = rxdata.temp;
        txdata.station = 2;
        
        ETout.sendData();
        failCounter = 0;
          
  }
  else {
    
      failCounter++; 

      if (failCounter >= 30) {
        
        txdata.timer = -1;
        txdata.temp = -1;
        txdata.station = 2;

        ETout.sendData();
        failCounter = 0;
        
      }
    
  }
  
  //you should make this delay shorter then your transmit delay or else messages could be lost
  delay(500);

}


