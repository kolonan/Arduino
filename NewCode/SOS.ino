#include <SoftwareSerial.h>
#include <TinyGPS.h>
SoftwareSerial gprsSerial(4,5);
SoftwareSerial gpsSerial (9,8);
String latitude;
String longitude;


void setup() {
  Serial.begin(9600);
   while (!Serial) {
    ;  }
  gpsSerial.begin(9600);
  gprsSerial.begin(9600);
}

void loop() {
  gpsSerial.listen();
  gpsData();
  delay(3000);
  gprsSerial.listen();
  gprsSetup();
  sendData();
 

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

  if (newData=true)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("LAT=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" LON=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
    latitude = flat;
    longitude = flon;
  }
  
  gps.stats(&chars, &sentences, &failed);
  Serial.print(" CHARS=");
  Serial.print(chars);
  Serial.print(" SENTENCES=");
  Serial.print(sentences);
  Serial.print(" CSUM ERR=");
  Serial.println(failed);
  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");
  }
void gprsSetup(){
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
