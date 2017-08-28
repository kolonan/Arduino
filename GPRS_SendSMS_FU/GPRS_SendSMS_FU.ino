#include <SoftwareSerial.h>
SoftwareSerial gprsSerial(4, 5);

 String a="10023889";
void setup()
{
  Serial.begin(9600);
 gprsSetup();
  
}


void loop()
{
  sendData();

   
}
void gprsSetup(){
  gprsSerial.begin(9600);
  Serial.println("Config SIM900...");
  delay(2000);
  Serial.println("Done!...");
  gprsSerial.flush();
  Serial.flush();

  // attach or detach from GPRS service 
 


  // bearer settings
  gprsSerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  delay(1000);
  toSerial();

  // bearer settings
  gprsSerial.println("AT+SAPBR=3,1,\"APN\",\"www\"");
  delay(1000);
  toSerial();

  // bearer settings
  gprsSerial.println("AT+SAPBR=1,1");
  delay(1000);
  toSerial();

   gprsSerial.println("AT+SAPBR=2,1");
  delay(1000);
  toSerial();
  }
void sendData(){
     // initialize http service
   gprsSerial.println("AT+HTTPINIT");
   delay(1000); 
   toSerial();

   gprsSerial.println("AT+HTTPPARA=\"CID\",1");
   delay(1000); 
   toSerial();

   // set http param value
   gprsSerial.println("AT+HTTPPARA=\"URL\",\"sim800.herokuapp.com\"");
   delay(1000);
   toSerial();

   gprsSerial.println("AT+HTTPPARA=\"CONTENT\",\"application/x-www-form-urlencoded\"");
    delay(1000);
    toSerial();
    
   gprsSerial.println("AT+HTTPDATA=29,10000");
   delay(2000);
   toSerial();

   gprsSerial.println("lat=10.003187&long=105.781560\"");
   
   delay(1000);
   toSerial();
   

   // set http action type 0 = GET, 1 = POST, 2 = HEAD
   gprsSerial.println("AT+HTTPACTION=1");
   delay(10000);
   toSerial();
   
   // read server response
   gprsSerial.println("AT+HTTPREAD"); 
   delay(5000);
   toSerial();

   gprsSerial.println("AT+HTTPTERM");
   toSerial();
   delay(1000);

  }                                                                                                 

void toSerial()
{
  while(gprsSerial.available()!=0)
  {
    Serial.write(gprsSerial.read());
  }
}
