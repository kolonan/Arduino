#include <SoftwareSerial.h>
#include <TinyGPS.h>
  TinyGPS gps;
  SoftwareSerial gprsSerial(4, 5);
  SoftwareSerial serialgps (9,8);

String lat;
String lon;

void setup()
{
  Serial.begin(9600);
  serialgps.begin(9600);
  gprsSerial.begin(9600);
  gprsSerial.listen();
  setupGPRS();
  setupHTTP();
 }

void loop()
{
  serialgps.listen();
  gpsData();
  gprsSerial.listen();
  sendData();
   
}                                                                                                 

void setupGPRS(){
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
void setupHTTP(){
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

 }
 void gpsData(){
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    lat=String(flat,6);
    lon=String(flon,6);
  }
  
  gps.stats(&chars, &sentences, &failed);

  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");
 }
void sendData(){
 gprsSerial.println("AT+HTTPDATA=29,10000");
   delay(2000);
   toSerial();

   gprsSerial.println("lat="+lat+"&long="+lon+"\"");
   
   delay(1000);
   toSerial();
   

   // set http action type 0 = GET, 1 = POST, 2 = HEAD
   gprsSerial.println("AT+HTTPACTION=1");
   delay(15000);
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
