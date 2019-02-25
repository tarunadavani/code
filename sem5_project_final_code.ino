#include <SoftwareSerial.h>
#define AOT 1
#define RX 2
#define TX 3
int trigPin = 9;
int echoPin = 10;
int led = 7;
String AP = "T$@";       // CHANGE ME
String PASS = "Tsa12345"; // CHANGE ME
String API = "RUU0MKRQNQ57LYT9";   // CHANGE ME
String HOST = "184.106.153.149";
int PORT = 80;
String field = "field2";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;
int sensor_pin = 7;
int output_value=1;
SoftwareSerial esp8266(RX,TX);



  
void setup() {
  /*  Serial.begin(9600);
  WiFi.disconnect();
  delay(3000);
  Serial.println("Start");
  
  while (WiFi.begin("Tejas","6a5e23e76a26") != WL_CONNECTED){
    delay(300);
  }
  Serial.println("Connected........");*/
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=3",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
  
}
void loop() {
  long duration, distance;
  digitalWrite(trigPin,HIGH);
  delay(3000);
  digitalWrite(trigPin, LOW);
  duration=pulseIn(echoPin, HIGH);
  distance =(duration/2)/29.1;
  Serial.print(distance);
  Serial.println("CM");
  delay(4000);
 
 if((distance<=10)) 
  {
    digitalWrite(led, HIGH);
}
   else if(distance>10)
 {
     digitalWrite(led, LOW);
   }
    valSensor = getSensorData();
    /*if(valSensor>20)
    {
      String messageurl = "GET  /send/?Mobile=9925200129&Password=Tejasnp1998@&Key=tejasOScXsg1onp0kd8Jte&Message=100&To=9925200129";
      sendCommand("AT+CIPMUX=1",5,"OK");
      sendCommand("AT+CIPSTART=4,\"TCP\",\""+ HOST2 +"\","+ PORT,15,"OK");
      sendCommand("AT+CIPSEND=4,"+String(messageurl.length()+4),4,">");
      
      esp8266.println(messageurl);delay(1500);
      sendCommand("AT+CIPCLOSE=0",5,"OK");
      Serial.print("done3");
    }*/
    String getData = "GET /update?api_key="+ API +"&"+ field +"="+String(analogRead(AOT));
   // https://smsapi.engineeringtgr.com
    
    sendCommand("AT+CIPMUX=1",5,"OK");
    sendCommand("AT+CIPSTART=4,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
    sendCommand("AT+CIPSEND=4,"+String(getData.length()+4),4,">");
    esp8266.println(getData);
    delay(1500);
    Serial.print("done2");   
    sendCommand("AT+CIPCLOSE=0",5,"OK");
    delay(1000);
}
int getSensorData(){
  output_value = analogRead(sensor_pin);
  output_value = map(output_value, 1023, 0, 0, 100);
  //Serial.print("Moisture:");
  Serial.print(output_value);
  Serial.print("%\n");
  return output_value;
} 
void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
