//=====================================================================================================================//
// Project                 - Smart Waiter : Intelligent Voice Assistant in Restaurant 
// Author                  - https://www.hackster.io/maheshyadav216
// Hardware                - SKU_SEN0539-EN_Gravity_Voice_Recognition_Module, NodeMCU ESP32s, Nextion Display
//                           FireBeetle 2 ESP32-E, NodeMCUESP8266, DFplayer Mini
// Softare                 - Arduino IDE 1/ PLatformIO with VSCode, Nextion Editor
// GitHub Repo of Project  - https://github.com/maheshyadav216/Project-Intelligent-Voice-Assistant-in-restaurant
// Code last Modified on   - 20/09/2023
//======================================================================================================================//

// Code of Hardware at Chef's cabin(Kitchen) (with NodeMCU ESP8266 dev Board). Refer Circuit diagram for more details

//Include Important Libraries
#include <Arduino.h>
#include "Nextion.h"
#include <ESP8266WiFi.h>
#include <espnow.h>
#include "DFRobotDFPlayerMini.h"

#if (defined(ARDUINO_AVR_UNO) || defined(ESP8266))   // Using a soft serial port
#include <SoftwareSerial.h>
SoftwareSerial softSerial(/*rx =*/4, /*tx =*/5);
#define FPSerial softSerial
#else
#define FPSerial Serial1
#endif

DFRobotDFPlayerMini myDFPlayer;

#define statLED D5

// put function declarations here:
NexPage page0    = NexPage(0, 0, "page0");
NexPage page1    = NexPage(1, 0, "page1");


/*
 * Declare a number object [page id:0,component id:3, component name: "n0"]. 
 */
NexNumber n01 = NexNumber(0, 2, "n01");
NexNumber n02 = NexNumber(0, 3, "n02");
NexNumber n03 = NexNumber(0, 4, "n03");
NexNumber n04 = NexNumber(0, 5, "n04");
NexNumber n05 = NexNumber(0, 6, "n05");
NexNumber n06 = NexNumber(0, 7, "n06");

NexNumber n11 = NexNumber(1, 2, "n11");
NexNumber n12 = NexNumber(1, 3, "n12");
NexNumber n13 = NexNumber(1, 4, "n13");
NexNumber n14 = NexNumber(1, 5, "n14");
NexNumber n15 = NexNumber(1, 6, "n15");
NexNumber n16 = NexNumber(1, 7, "n16");


//Structure example to receive data
//Must match the sender structure
typedef struct tmenu_struct {
  uint8_t mcode;
} tmenu_struct;

tmenu_struct myOrder;

//Menu codes
uint8_t T1vada = 0;
uint8_t T1idli = 0;
uint8_t T1rice = 0;
uint8_t T1samosa = 0;
uint8_t T1butter = 0;
uint8_t T1biryani = 0;

uint8_t T2vada = 0;
uint8_t T2idli = 0;
uint8_t T2rice = 0;
uint8_t T2samosa = 0;
uint8_t T2butter = 0;
uint8_t T2biryani = 0;

uint8_t menu_code;


//callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myOrder, incomingData, sizeof(myOrder));
  menu_code = myOrder.mcode;
  //Serial.print("Bytes received: ");
  //Serial.println(len);
  //Serial.print("menu Code: ");
  //Serial.println(myOrder.mcode);
}

void blinkStat(){
  digitalWrite(statLED, HIGH);
  delay(1000);
  digitalWrite(statLED, LOW);
}


void setup() {
  // put your setup code here, to run once:
  FPSerial.begin(9600);
  Serial.begin(115200);
  nexInit();
  page0.show();

  pinMode(statLED, OUTPUT);
  digitalWrite(statLED, LOW);

  if (!myDFPlayer.begin(FPSerial, /*isACK = */true, /*doReset = */true)) {  //Use serial to communicate with mp3.
  Serial.println(F("Unable to begin:"));
  Serial.println(F("1.Please recheck the connection!"));
  Serial.println(F("2.Please insert the SD card!"));
  while(true){
    delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.volume(26);  //Set volume value. From 0 to 30
  //myDFPlayer.play(1);  //Play the first mp3
  delay(1000);

  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  //Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Check for which code received and Identifiy the food item ordered
  switch(menu_code) {
    case 11:
      blinkStat();
      T1vada +=1;
      page0.show();
      n01.setValue(T1vada);
      myDFPlayer.play(2);
      menu_code = 0;
      break;
    
    case 12:
      blinkStat();
      T1idli +=1;
      page0.show();
      n02.setValue(T1idli);
      myDFPlayer.play(3);
      menu_code = 0;
      break;

    case 13:
      blinkStat();      
      T1rice +=1;
      page0.show();
      n03.setValue(T1rice);
      myDFPlayer.play(4);
      menu_code = 0;
      break;

    case 14:
      blinkStat();    
      T1samosa +=1;
      page0.show();
      n04.setValue(T1samosa);
      myDFPlayer.play(5); 
      menu_code = 0;
      break;

    case 15:
      blinkStat();
      T1butter +=1;
      page0.show();
      n05.setValue(T1butter);
      myDFPlayer.play(6);
      menu_code = 0;
      break;

    case 16:
      blinkStat();
      T1biryani +=1;
      page0.show();
      n06.setValue(T1biryani);
      myDFPlayer.play(7);
      menu_code = 0;
      break;   

    case 21:
      blinkStat();
      T2vada +=1;
      page1.show();
      n11.setValue(T2vada);
      myDFPlayer.play(8);
      menu_code = 0;
      break;
    
    case 22:
      blinkStat();
      T2idli +=1;
      page1.show();
      n12.setValue(T2idli);
      myDFPlayer.play(9);
      menu_code = 0;
      break;

    case 23:
      blinkStat();
      T2rice +=1;
      page1.show();
      n13.setValue(T2rice);
      myDFPlayer.play(10);
      menu_code = 0;
      break;

    case 24:
      blinkStat();
      T2samosa +=1;
      page1.show();
      n14.setValue(T2samosa);
      myDFPlayer.play(11);
      menu_code = 0;
      break;

    case 25:
      blinkStat();
      T2butter +=1;
      page1.show();
      n15.setValue(T2butter);
      myDFPlayer.play(12);
      menu_code = 0;
      break;

    case 26:
      blinkStat();
      T2biryani +=1;
      page1.show();
      n16.setValue(T2biryani);
      myDFPlayer.play(1);
      menu_code = 0;
      break;   

    default:
      if (menu_code != 0) {
        Serial.print("menu_code = ");  //Printing command ID
        Serial.println(menu_code);
      }
  }
  delay(300);
}

//======================================================================================================================//