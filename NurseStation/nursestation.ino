#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <Keypad.h>
#include <NTPClient.h>        // include NTPClient library
#include <TimeLib.h>          // Include Arduino time library
#include <Adafruit_GFX.h>     // include Adafruit graphics library
#include <Adafruit_ST7735.h>  // include Adafruit ST7735 TFT library

ESP8266WebServer server(80);
// ST7735 TFT module connections
#define TFT_RST   D3     // TFT RST pin is connected to NodeMCU pin D4 (GPIO2)
#define TFT_CS    D8     // TFT CS  pin is connected to NodeMCU pin D3 (GPIO0)
#define TFT_DC    D4     // TFT DC  pin is connected to NodeMCU pin D2 (GPIO4)
// initialize ST7735 TFT library with hardware SPI module
// SCK (CLK) ---> NodeMCU pin D5 (GPIO14)
// MOSI(DIN) ---> NodeMCU pin D7 (GPIO13)
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// set WiFi network SSID and password
const char *ssid     = "WIS113-gaming";
const char *password = "lv589398";

//uint8_t buzzer = D0;
#define buzzer 16

String clients[5][2];
static byte previous_dow = 0;

const byte n_rows = 4;
const byte n_cols = 4;
char keys[n_rows][n_cols] = {
 
  {'1','2','3','A'},
 
  {'4','5','6','B'},
 
  {'7','8','9','C'},
 
  {'*','0','#','D'}
 
};
byte colPins[n_rows] = {3, 12, 4, 5};
byte rowPins[n_cols] = {10, 9, 9, 1};

Keypad myKeypad = Keypad( makeKeymap(keys), rowPins, colPins, n_rows, n_cols);

String finished_id = "_";
int finished_messagebox = 0;
int error_messagebox = 0;
String error_id = "_";
String digit_1 = "_";
String digit_2 = "_";
int menuu = 0;

WiFiUDP ntpUDP;

// 'time.nist.gov' is used (default server) with +1 hour offset (3600 seconds) 60 seconds (60000 milliseconds) update interval
NTPClient timeClient(ntpUDP, "time.nist.gov", 3600, 60000);
unsigned long unix_epoch;

void setup(void)
{
  pinMode (buzzer,OUTPUT) ;

  clients[0][0] = "14";
  clients[0][1] = "0";
  
  Serial.begin(9600);
  tft.initR(INITR_BLACKTAB);     // initialize a ST7735S chip, black tab
  tft.fillScreen(ST7735_BLACK);  // fill screen with black color
  tft.drawFastHLine(0, 44,  tft.width(), ST7735_BLUE);    // draw horizontal blue line at position (0, 44)
  tft.setTextColor(ST7735_WHITE, ST7735_BLACK);     // set text color to white and black background
  tft.setTextSize(1);                 // text size = 1
  tft.setCursor(1, 10);              // move cursor to position (43, 10) pixel
  tft.print("iSyringe NurseStation");
  tft.setCursor(12, 27);               // move cursor to position (4, 27) pixel
  tft.print("PUSH A FOR MENU");

  WiFi.begin(ssid, password);
  Serial.print("Connecting.");
  tft.setCursor(0, 54);              // move cursor to position (4, 27) pixel
  tft.print("Connecting..");
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("connected");
  tft.print("connected");
  delay(2000);
  tft.fillRect(0, 54, tft.width(), 8, ST7735_BLACK);

  tft.drawFastHLine(0, 102, tft.width(), ST7735_BLUE);  // draw horizontal blue line at position (0, 102)
  tft.setTextSize(2);                                   // text size = 2
  tft.setTextColor(ST7735_MAGENTA, ST7735_BLACK);       // set text color to magenta and black background
  tft.setCursor(37, 112);                               // move cursor to position (37, 112) pixel
  tft.print("TIME:");

  timeClient.begin();
  
  server.on("/", handle_OnConnect);
  server.on("/finish", handle_finish);
  server.on("/ises", handle_error);
  server.on("/eatdrug", handle_eatdrug);
  server.begin();
  Serial.println("HTTP server started");
}

void handle_OnConnect(){
  server.send(200, "text/plain", "Welcome to iSyringe Nurse Station!");
}

void handle_finish() { // Finish Injetion
  if (server.hasArg("id")) {
    finished_id = server.arg("id");
    finished_messagebox = 1;
  }
  server.send(200, "text/plain", "1");
}

void handle_error(){
  if (server.hasArg("id")) {
    error_id = server.arg("id");
    error_messagebox = 1;
  }
  server.send(200, "text/plain", "1");
}

void handle_eatdrug(){
  if (server.hasArg("id")) {
    String id = server.arg("id");
    for(int i = 0; i<5; i++){
      if((clients[i][0] == id) && (clients[i][1] == "1")){ // Time to eat drug
          server.send(200, "text/plain", "1");
          clients[i][1] == "0";
      }
    }
  }
    server.send(200, "text/plain", "0");
}

void RTC_display()
{
  char dow_matrix[7][10] = {"SUNDAY", "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY"};
  byte x_pos[7] = {29, 29, 23, 11, 17, 29, 17};
  
  // print day of the week
  if( previous_dow != weekday(unix_epoch) )
  {
    previous_dow = weekday(unix_epoch);
    tft.fillRect(11, 55, 108, 14, ST7735_BLACK);     // draw rectangle (erase day from the display)
    tft.setCursor(x_pos[previous_dow-1], 55);
    tft.setTextColor(ST7735_CYAN, ST7735_BLACK);     // set text color to cyan and black background
    tft.print( dow_matrix[previous_dow-1] );
  }

  // print date
  tft.setCursor(4, 79);
  tft.setTextColor(ST7735_YELLOW, ST7735_BLACK);     // set text color to yellow and black background
  tft.printf( "%02u-%02u-%04u", day(unix_epoch), month(unix_epoch), year(unix_epoch) );
  // print time
  tft.setCursor(16, 136);
  tft.setTextColor(ST7735_GREEN, ST7735_BLACK);     // set text color to green and black background
  tft.printf( "%02u:%02u:%02u", hour(unix_epoch), minute(unix_epoch), second(unix_epoch) );
}

void ErrorMessageBox(){
    digitalWrite(buzzer,HIGH);
    tft.fillRect(11, 55, 108, 100, ST7735_BLACK);     // draw rectangle (erase day from the display)
    tft.setCursor(17, 55);
    tft.setTextColor(ST7735_RED, ST7735_BLACK);     // set text color to cyan and black background
    tft.print( "CLIENT" );
    tft.setCursor(20, 79);
    tft.setTextColor(ST7735_RED, ST7735_BLACK);     // set text color to cyan and black background
    tft.print( "ERROR" );
    tft.setCursor(25, 100);
    tft.setTextColor(ST7735_YELLOW, ST7735_BLACK);     // set text color to yellow and black background
    tft.print( "CID:" + error_id );
    delay(4000);
    digitalWrite(buzzer,LOW);
    error_messagebox = 0;
    previous_dow = 0;
    tft.fillRect(11, 55, 108, 100, ST7735_BLACK);
    tft.drawFastHLine(0, 102, tft.width(), ST7735_BLUE);  // draw horizontal blue line at position (0, 102)
  tft.setTextSize(2);                                   // text size = 2
  tft.setTextColor(ST7735_MAGENTA, ST7735_BLACK);       // set text color to magenta and black background
  tft.setCursor(37, 112);                               // move cursor to position (37, 112) pixel
  tft.print("TIME:");
}


void FinishMessageBox(){
    digitalWrite(buzzer,HIGH);
    tft.fillRect(11, 55, 108, 100, ST7735_BLACK);     // draw rectangle (erase day from the display)
    tft.setCursor(25, 55);
    tft.setTextColor(ST7735_CYAN, ST7735_BLACK);     // set text color to cyan and black background
    tft.print( "CLIENT" );
    tft.setCursor(17, 79);
    tft.setTextColor(ST7735_CYAN, ST7735_BLACK);     // set text color to cyan and black background
    tft.print( "FINISHED" );
    tft.setCursor(25, 100);
    tft.setTextColor(ST7735_YELLOW, ST7735_BLACK);     // set text color to yellow and black background
    tft.print( "CID:" + finished_id );
    delay(4000);
    digitalWrite(buzzer,LOW);
    finished_messagebox = 0;
    previous_dow = 0;
        tft.fillRect(11, 55, 108, 100, ST7735_BLACK);
      tft.drawFastHLine(0, 102, tft.width(), ST7735_BLUE);  // draw horizontal blue line at position (0, 102)
  tft.setTextSize(2);                                   // text size = 2
  tft.setTextColor(ST7735_MAGENTA, ST7735_BLACK);       // set text color to magenta and black background
  tft.setCursor(37, 112);                               // move cursor to position (37, 112) pixel
  tft.print("TIME:");
}


void menu(){
    tft.fillScreen(ST7735_BLACK);  // fill screen with black color
  tft.drawFastHLine(0, 50,  tft.width(), ST7735_BLUE);    // draw horizontal blue line at position (0, 44)
  tft.setTextColor(ST7735_WHITE, ST7735_BLACK);     // set text color to white and black background
  tft.setTextSize(1);                 // text size = 1
  tft.setCursor(1, 10);              // move cursor to position (43, 10) pixel
  tft.print("iSyringe NurseStation");
  tft.setCursor(2, 27);               // move cursor to position (4, 27) pixel
  tft.setTextSize(2);
  tft.print("DrugRemind");
  tft.fillRect(11, 55, 108, 14, ST7735_BLACK);     // draw rectangle (erase day from the display)
    tft.setCursor(1, 60);
    tft.setTextColor(ST7735_CYAN, ST7735_BLACK);     // set text color to cyan and black background
    tft.print( "Enter CID#" );
    tft.setTextSize(2);                                   // text size = 2
  tft.setTextColor(ST7735_MAGENTA, ST7735_BLACK);       // set text color to magenta and black background
  tft.setCursor(37, 90);                               // move cursor to position (37, 112) pixel
  tft.print(digit_1);
    tft.setCursor(60, 90);                               // move cursor to position (37, 112) pixel
    tft.print(digit_2);
    if(digit_1 == "_"){
      char diggt1 = myKeypad.waitForKey();
      //if(diggt1 != ""){
        digit_1 = diggt1;
      //}
      tft.setCursor(37, 90);                               // move cursor to position (37, 112) pixel
      tft.print(digit_1);
    }
    if(digit_2 == "_"){
        char diggt2 = myKeypad.waitForKey();
        //if(diggt2 != 0){
          digit_2 = diggt2;
        //}
      tft.setCursor(60, 90);                               // move cursor to position (37, 112) pixel
      tft.print(digit_2);
    }
    if((digit_1 != "_") && (digit_2 != "_")){
      tft.setCursor(1, 140);
      tft.setTextColor(ST7735_CYAN, ST7735_BLACK);     // set text color to cyan and black background
      tft.print( "A=OK C=X" );
      char keykey = myKeypad.waitForKey();
      if(keykey == 'C'){
        digit_2 = "_";
        digit_1 = "_";
        menuu = 0;
      }
      if(keykey == 'A'){
        for(int i = 0; i < 5; i++){
          if(clients[i][0] == digit_1 + digit_2){
            clients[i][1] = 1;
          }
        }
        digit_2 = "_";
        digit_1 = "_";
        menuu = 0;
      }
    }
    
 }

// main loop
void loop()
{
  
  
  timeClient.update();
  unix_epoch = timeClient.getEpochTime();   // get UNIX Epoch time


  server.handleClient();

  if(error_messagebox == 1){
    ErrorMessageBox();
  }

  if(finished_messagebox == 1){
    FinishMessageBox();
  }
  if(myKeypad.getKey() == 'B'){
    menuu = 1;
  }
  if(menuu == 1){
    menu();
  } else {
      RTC_display();
  }
   
  
  delay(200);    // wait 200ms

}
