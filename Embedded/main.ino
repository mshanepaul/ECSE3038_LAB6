#include <Arduino.h>
#include <SoftwareSerial.h>
#define RX 10
#define TX 11
#define DEBUG true
#define TIMEOUT 3000


String ID = "WIFI_MODEM";
String PassWd = "MnD254";
String Host = "192.168.100.82";
int Port = 5000;

int tankID = 0;

SoftwareSerial espSerial(RX, TX); // RX, TX
String sendData(String command, const int timeout, boolean debug);
int getWaterLevel();

void setup() {
  espSerial.begin(9600);
  Serial.begin(9600);

  sendData("AT+RST", TIMEOUT, DEBUG);
  String SETUP = "AT+CWJAP=";
  SETUP.concat("\"" + ID + "\",\"" + PassWD + "\"");
  sendData(SETUP, TIMEOUT, DEBUG);
}

void loop() { 
  String START = "AT+CIPSTART=\"TCP\",";
  START.concat("\"" + HOST + "\"," + PORT);
  sendData(START, TIMEOUT, DEBUG);

  tankID ++;

  String BODY = "{\"tank_id\":" + String(tankID) + ", \"water_level\":" + String(getWaterLevel()) + "}\r\n";
  
  String POST = "POST /tank HTTP/1.1\r\n";                              //line 1
  POST.concat("Host: " + HOST + ":" + PORT + "\r\n");                   //line 2
  POST.concat("Content-Type: application/json\r\n");                    //line 3 
  POST.concat("Content-Length: " + String(BODY.length()) + "\r\n\r\n"); //line 4
  POST.concat(BODY);                                                    //line 5
  

  String SEND = "AT+CIPSEND=";
  SEND.concat(POST.length());
  sendData(SEND, TIMEOUT, DEBUG);

  sendData(POST, TIMEOUT, DEBUG);
}

String sendData(String command, const int timeout, boolean debug){
  command.concat("\r\n"); //Simulate enter btn
  String response = "";
  espSerial.print(command);

  long unsigned int time = millis();

  while((time + timeout) > millis()){
      while(espSerial.available()){
          char c = espSerial.read();
          response += c;
        }
    }

   if(debug){
      Serial.print(response); 
    }

  return(response);
}

int getWaterLevel(){
  // get random number from 10 - 200
  int randNumber = random(10, 201);
  return randNumber;
}
