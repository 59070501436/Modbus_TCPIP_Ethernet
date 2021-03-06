#include <SPI.h>
#include <Ethernet.h>
#include "MB_TCP_IP_Master_Ethernet.h"

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server(192,168,1,102);
IPAddress ip(192, 168, 1, 177);
IPAddress myDns(192, 168, 1, 1);

MB_TCPIP_Master MB_ET;

void setup() {
  // You can use Ethernet.init(pin) to configure the CS pin
    Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet

  Serial.begin(9600);

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet");
  Ethernet.begin(mac, ip, myDns);
  // give the Ethernet shield a second to initialize:
  delay(1000);
  ClientConnect();  //Connect to server
}

void loop() {      

  /////////////////////////////READ ANY NUMBER OF H REGISTERS//////////////////////////////////////////
  
  word ParRead[MAX_READ_PAR];   
  int Count = 3;
  
  MB_ET.MB_ReadHreg(0,MB_ET.GetStandardAdress(3,29),Count,ParRead);       //bool MB_ReadHreg(byte Slave_ID, word Adress, word Count, word* ReadHregBuff);
  for(int i=0;i<Count;i++)
  {
    Serial.print("Hreg #");
    Serial.print(i);
    Serial.print(" read: ");
    Serial.println(ParRead[i]);    
  }  
  Serial.println("");
  

  
  /////////////////////////////READ ONE DOUBLE H REGISTER//////////////////////////////////////////
  
  long ParDWORD;
  MB_ET.MB_Read_DWORD_Hreg(0,MB_ET.GetStandardAdress(20,22),&ParDWORD);   //bool MB_Read_DWORD_Hreg(byte Slave_ID, word Adress,long* DWordResp);
  Serial.println("Par Long leido:");
  Serial.println(ParDWORD);
  Serial.println("");
  
  
  /////////////////////////////WRITE ONE H REGISTER//////////////////////////////////////////   
  
  if(MB_ET.MB_WriteSingleHreg(0,MB_ET.GetStandardAdress(18,14),12345))   //bool MB_WriteSingleHreg(byte Slave_ID, word Adress, word Value);
  {
    Serial.println("Write Successful!");
    Serial.println("");
  }

  /////////////////////////////WRITE ONE DOUBLE H REGISTER//////////////////////////////////////////   
  //MB_ET.MB_Write_DWORD_Hreg(0,GetStandardAdress(20,23),123);

  /////////////////////////////MANAGE DISCONNECTION//////////////////////////////////////////   
  // if the server's disconnected, attempt reconnecct:
  if (!MB_ET.MB_ETH_client.connected()) {
    ClientConnect();
  }
  delay(1000);
}

bool ClientConnect()
{
  byte debug_resp = 255;
  Serial.print("connecting to ");
  Serial.print(server);
  Serial.println("...");

  debug_resp=MB_ET.MB_ETH_client.connect(server, MB_TCP_PORT);
  Serial.print("Conection Status: ");
  Serial.println(debug_resp);

  //si no conecta, reintentar
  while(debug_resp==0)
  {
    //delay(100);
    Serial.print("connecting to ... ");
    Serial.print(server);
    Serial.print(", port ");
    Serial.println(MB_TCP_PORT);
    // if you get a connection, report back via serial:
    debug_resp=MB_ET.MB_ETH_client.connect(server, MB_TCP_PORT);
    Serial.print("Conection Status: ");
    Serial.println(debug_resp);
  }

  // conecta success
  if (debug_resp) {
    Serial.print("connected to ");
    Serial.println(MB_ET.MB_ETH_client.remoteIP());  
    return true; 
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
    return false;
  }
}
