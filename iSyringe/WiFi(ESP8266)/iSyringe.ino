#include <SPI.h>

#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>//Json库
#include <HttpClient.h>//HTTP库
const char *ssid     = "WIS113-gaming";//输入Wifi账号
const char *password = "lv589398";//输入WiFi密码
const char* host="ece198.uwaterloo.gay";//iSyringe管理后台地址
const char* nursestation="192.168.1.1";//Nurse Station IP
String CID="14";// iSyringe client ID #

int WEB_OK = 0;
int WEB_OK_PIN = 16;
int WEB_ERROR = 1;
int WEB_E_PIN = 5;
int WEB_EAT_DRUG = 0;
int DRUG_PIN = 4;
int WEB_FINISHED_SYRINGE = 0;
int FINISH_PIN = 10;
int ALARM = 0;
int ALARM_PIN = 2;


void setup() {
  pinMode(WEB_OK_PIN, OUTPUT);
  pinMode(WEB_E_PIN, OUTPUT);
  pinMode(DRUG_PIN, OUTPUT);
  pinMode(FINISH_PIN, INPUT);
  pinMode(ALARM_PIN, INPUT);

  digitalWrite(WEB_OK_PIN, 0);
  digitalWrite(WEB_E_PIN, 1);
  
  //analogWrite(LCD_BL_PIN, 1023 - (LCD_BL_PWM*10));
  // put your setup code here, to run once:
  Serial.begin(9600);//串口通讯波特率
  Serial.println("iSyringe debug console");
  Serial.println("Built by TEAM #32");
  Serial.println("Visit \"https://github.com/iwCJ1/ECE-198-Project\" for more information");
  Serial.println("Connecting WiFi...");
  WiFi.begin(ssid, password);//连接WiFi
  while ( WiFi.status() != WL_CONNECTED ){
    delay ( 500 );
    Serial.print ( "." );
  }
  digitalWrite(WEB_OK_PIN, 1);//WiFi Connected
  Serial.println("");
  Serial.println("WiFi connected");//连接成功
  Serial.print("IP address: ");//打印IP地址
  Serial.println(WiFi.localIP());

 delay(100);

   //创建TCP连接
  WiFiClient client; // Send to IRIS -> Device Online
  const int httpPort=80;
  if(!client.connect(host,httpPort)){
    Serial.println("connection failed");//网络请求无响应打印连接失败
    return;
  }//URL请求地址
  String url="/iscc?id="+CID+"&ip="+WiFi.localIP().toString().c_str();
  //发送网络请求 
  client.print(String("GET ")+url+" HTTP/1.1\r\n"+"Host: "+host+"\r\n"+"Connection: close\r\n\r\n");
  delay(5000);
  //定义answer变量用来存放请求网络服务器后返回的数据
  String answer;
  while(client.available()){
    String line=client.readStringUntil('\r');
    answer+=line;
    }
  //断开服务器连接
  client.stop();
  Serial.println();
  Serial.println("closing connection");
  //jsonAnswer=answer.substring(jsonIndex);
  Serial.println();
  Serial.println("JSON answer: ");
  Serial.println(answer);
  Serial.println(answer[answer.length()-1]);
  if(answer[answer.length()-1] == '1'){
    digitalWrite(WEB_E_PIN, 0);
      //创建TCP连接
    WiFiClient client; // Send to IRIS -> Device Working
    const int httpPort=80;
    if(!client.connect(host,httpPort)){
     Serial.println("connection failed");//网络请求无响应打印连接失败
     return;
    }//URL请求地址
    String url="/ises?id="+CID+"&ip=1&es=0";
    //发送网络请求 
    client.print(String("GET ")+url+" HTTP/1.1\r\n"+"Host: "+host+"\r\n"+"Connection: close\r\n\r\n");
    delay(5000);
      //定义answer变量用来存放请求网络服务器后返回的数据
    String answer;
      while(client.available()){
        String line=client.readStringUntil('\r');
        answer+=line;
      }
    //断开服务器连接
    client.stop();
    Serial.println();
    Serial.println("closing connection");
    //jsonAnswer=answer.substring(jsonIndex);
    Serial.println();
    Serial.println("JSON answer: ");
    Serial.println(answer);
    Serial.println(answer[answer.length()-1]);
  }
delay(5000);
}

void loop() {
  // put your main code here, to run repeatedly:
    digitalWrite(DRUG_PIN, 0); // Reset Eat Drug Status
   if(digitalRead(FINISH_PIN) == 0){ // Send to Nurse Station: Finished injection
      //创建TCP连接
      WiFiClient client;
      const int httpPort=80;
      if(!client.connect(nursestation,httpPort)){
        Serial.println("Nurse Station [finish injection] - connection failed");//网络请求无响应打印连接失败
        digitalWrite(WEB_E_PIN, 1);
        return;
      }//URL请求地址
      digitalWrite(WEB_E_PIN, 0);
      String url="/finish?id="+CID;// finish injection
      //发送网络请求 
      client.print(String("GET ")+url+" HTTP/1.1\r\n"+"Host: "+host+"\r\n"+"Connection: close\r\n\r\n");
      delay(500);
      //定义answer变量用来存放请求网络服务器后返回的数据
      String answer;
      while(client.available()){
        String line=client.readStringUntil('\r');
        answer+=line;
        }
      //断开服务器连接
      client.stop();
      Serial.println();
      Serial.println("closing connection");
      //获得json格式的数据
      String jsonAnswer;
      int jsonIndex = 0;
      jsonAnswer=answer.substring(jsonIndex);
      Serial.println();
      Serial.println("JSON answer Finish Injection Nurse Station: ");
      Serial.println(answer);
      Serial.println(answer[answer.length()-1]);
   }
   if(digitalRead(ALARM_PIN) == 0){ // ERROR -> Send to IRIS
      //创建TCP连接
      WiFiClient client;
      const int httpPort=80;
      if(!client.connect(host,httpPort)){
        Serial.println("IRIS [set error status] - connection failed");//网络请求无响应打印连接失败
        digitalWrite(WEB_E_PIN, 1);
        return;
      }//URL请求地址
      digitalWrite(WEB_E_PIN, 0);
      String url="/ises?id="+CID+"&ip=0&es=0";// set error status
      //发送网络请求 
      client.print(String("GET ")+url+" HTTP/1.1\r\n"+"Host: "+host+"\r\n"+"Connection: close\r\n\r\n");
      delay(500);
      //定义answer变量用来存放请求网络服务器后返回的数据
      String answer;
      while(client.available()){
        String line=client.readStringUntil('\r');
        answer+=line;
        }
      //断开服务器连接
      client.stop();
      Serial.println();
      Serial.println("closing connection");
      //获得json格式的数据
      String jsonAnswer;
      int jsonIndex = 0;
      jsonAnswer=answer.substring(jsonIndex);
      Serial.println();
      Serial.println("JSON answer ALARM - ERROR iRIS: ");
      Serial.println(answer);
      Serial.println(answer[answer.length()-1]);
   }
      if(digitalRead(ALARM_PIN) == 0){ // ERROR -> Send to Nurse Station
      //创建TCP连接
      WiFiClient client;
      const int httpPort=80;
      if(!client.connect(nursestation,httpPort)){
        Serial.println("Nurse Station [ERROR] - connection failed");//网络请求无响应打印连接失败
        digitalWrite(WEB_E_PIN, 1);
        return;
      }//URL请求地址
      digitalWrite(WEB_E_PIN, 0);
      String url="/ises?id="+CID+"&ip=0&es=0";// ERROR STATUS
      //发送网络请求 
      client.print(String("GET ")+url+" HTTP/1.1\r\n"+"Host: "+host+"\r\n"+"Connection: close\r\n\r\n");
      delay(500);
      //定义answer变量用来存放请求网络服务器后返回的数据
      String answer;
      while(client.available()){
        String line=client.readStringUntil('\r');
        answer+=line;
        }
      //断开服务器连接
      client.stop();
      Serial.println();
      Serial.println("closing connection");
      //获得json格式的数据
      String jsonAnswer;
      int jsonIndex = 0;
      jsonAnswer=answer.substring(jsonIndex);
      Serial.println();
      Serial.println("JSON answer ALARM - ERROR Nurse Station: ");
      Serial.println(answer);
      Serial.println(answer[answer.length()-1]);
   }
   //创建TCP连接 //Eat Drug
    WiFiClient client; // Send to IRIS -> Eat Drug GET IRIS
    const int httpPort=80;
    if(!client.connect(host,httpPort)){
     Serial.println("IRIS [Eat Drug] - connection failed");//网络请求无响应打印连接失败
     return;
    }//URL请求地址
    String url="/eatdrug?id="+CID;
    //发送网络请求 
    client.print(String("GET ")+url+" HTTP/1.1\r\n"+"Host: "+host+"\r\n"+"Connection: close\r\n\r\n");
    delay(5000);
      //定义answer变量用来存放请求网络服务器后返回的数据
    String answer;
      while(client.available()){
        String line=client.readStringUntil('\r');
        answer+=line;
      }
    //断开服务器连接
    client.stop();
    Serial.println();
    Serial.println("closing connection");
    //jsonAnswer=answer.substring(jsonIndex);
    Serial.println();
    Serial.println("JSON answer of Eat Drug IRIS");
    Serial.println(answer);
    Serial.println(answer[answer.length()-1]);
    if(answer[answer.length()-1] == '1'){
        digitalWrite(DRUG_PIN, 1);
    }
     //创建TCP连接 //Eat Drug
    WiFiClient client2; // Send to IRIS -> Eat Drug GET Nurse Station
    const int httpPort2=80;
    if(!client2.connect(nursestation,httpPort)){
     Serial.println("Nurse Station [Eat Drug] - connection failed");//网络请求无响应打印连接失败
     return;
    }//URL请求地址
    String url2="/eatdrug?id="+CID;
    //发送网络请求 
    client2.print(String("GET ")+url2+" HTTP/1.1\r\n"+"Host: "+host+"\r\n"+"Connection: close\r\n\r\n");
    delay(5000);
      //定义answer变量用来存放请求网络服务器后返回的数据
    String answer2;
      while(client2.available()){
        String line=client2.readStringUntil('\r');
        answer2+=line;
      }
    //断开服务器连接
    client2.stop();
    Serial.println();
    Serial.println("closing connection");
    //jsonAnswer=answer.substring(jsonIndex);
    Serial.println();
    Serial.println("JSON answer of Eat Drug Nurse Station");
    Serial.println(answer2);
    Serial.println(answer2[answer2.length()-1]);
    if(answer2[answer2.length()-1] == '1'){
        digitalWrite(DRUG_PIN, 1);
    }

  delay(10000);
}
