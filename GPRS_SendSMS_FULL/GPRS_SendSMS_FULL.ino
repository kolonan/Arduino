#include <SoftwareSerial.h>
#include <TinyGPS.h>
  TinyGPS gps;
SoftwareSerial gprsSerial(4, 5);
SoftwareSerial serialgps (9,8);


 int year;
      byte month,day,hour,minute,second,hundredths;
     unsigned long chars;
     unsigned short sentences, failed_checksum;
     float latitude,longitude;
     static void print_date(TinyGPS &gps);

     
 //String a="10023889";
 float b=123456;
  //String flat = String(latitude);
 // String flong = String (longitude);
  String   flat="10.0127459";
  String  flong="105.7567281";
  
 String c= String(b);
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
  gpsSerial.listen();
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
  while (serialgps.available()>0)
           {

      char c = serialgps.read();


         if (gps.encode(c))
       {

  gps.f_get_position(&latitude, &longitude);
  Serial.print("Lat/Long: ");
  Serial.print(latitude, 5);
  Serial.print(", ");
  Serial.println(longitude, 5);
  
  
   gps.stats(&chars, &sentences, &failed_checksum);
     delay (2000);
     }}
 }
void sendData(){
 gprsSerial.println("AT+HTTPDATA=29,10000");
   delay(2000);
   toSerial();

   gprsSerial.println("lat="+flat+"&long="+flong+"\"");
   
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
