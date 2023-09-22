//=============================================================================================================//
// Project                 - Smart Waiter : Intelligent Voice Assistant in Restaurant 
// Author                  - https://www.hackster.io/maheshyadav216
// Hardware                - SKU_SEN0539-EN_Gravity_Voice_Recognition_Module, NodeMCU ESP32s, Nextion Display
//                           FireBeetle 2 ESP32-E, NodeMCUESP8266, DFplayer Mini
// Softare                 - Arduino IDE 1/ PLatformIO with VSCode, Nextion Editor
// GitHub Repo of Project  - https://github.com/maheshyadav216/Project-Intelligent-Voice-Assistant-in-restaurant
// Code last Modified on   - 20/09/2023
//==============================================================================================================//

// Code of Hardware At table 2 (with FireBeetle 2 ESP32-E dev Board). Refer Circuit diagram for more details

//Include Important Libraries
#include <Arduino.h>
#include "DFRobot_DF2301Q.h"
#include "Nextion.h"
#include <WiFi.h>
#include <esp_now.h>

//Peripherals pin definitions
#define buzzer 25
#define statLED 26

//Menu codes
uint8_t T2menu[6] = {21, 22, 23, 24, 25, 26};

// REPLACE WITH YOUR ESP RECEIVER'S MAC ADDRESS
uint8_t broadcastAddress1[] = {0x60, 0x01, 0x94, 0x0E, 0xE2, 0xD7};
uint8_t broadcastAddress2[] = {0x80, 0x7D, 0x3A, 0x6E, 0x82, 0xF7};

typedef struct tmenu_struct {
  uint8_t x;
} tmenu_struct;

tmenu_struct t2order;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

NexPage page0    = NexPage(0, 0, "page0");
NexPage page1    = NexPage(1, 0, "page1");
NexPage page2    = NexPage(2, 0, "page2");
NexPage page3    = NexPage(3, 0, "page3");
NexPage page4    = NexPage(4, 0, "page4");
NexPage page5    = NexPage(5, 0, "page5");
NexPage page6    = NexPage(6, 0, "page6");


//I2C communication
DFRobot_DF2301Q_I2C asr;


void shortBeep(){
  digitalWrite(buzzer, LOW);
  delay(500);
  digitalWrite(buzzer, HIGH);
  
}

void longBeep(){
  digitalWrite(buzzer, LOW);
  delay(1000);
  digitalWrite(buzzer, HIGH);
}

void setup() {
  Serial.begin(115200);
  nexInit();
  page0.show(); 

// Peripherals pin definitions. Buzzer module is at Active LOW logic
  pinMode(buzzer, OUTPUT);
  pinMode(statLED, OUTPUT);
  digitalWrite(buzzer, HIGH);
  digitalWrite(statLED, LOW);
  
  // Init the sensor
  while (!(asr.begin())) {
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");

  /**
   * @brief Set voice volume
   * @param voc - Volume value(1~7)
   */
  asr.setVolume(7);

  /**
     @brief Set mute mode
     @param mode - Mute mode; set value 1: mute, 0: unmute
  */
  asr.setMuteMode(0);

  /**
     @brief Set wake-up duration
     @param wakeTime - Wake-up duration (0-255)
  */
  asr.setWakeTime(8);

  /**
     @brief Get wake-up duration
     @return The currently-set wake-up period
  */
  uint8_t wakeTime = 0;
  wakeTime = asr.getWakeTime();
  Serial.print("wakeTime = ");
  Serial.println(wakeTime);

  //asr.playByCMDID(1);   // Wake-up command

  /**
     @brief Play the corresponding reply audio according to the ID
     @param CMDID - command word ID
  */
  //asr.playByCMDID(23);  // Command word ID

  WiFi.mode(WIFI_STA);
 
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  // register peer
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // register first peer - The Manager
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // register second peer - The Chef/cook
  memcpy(peerInfo.peer_addr, broadcastAddress2, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void sendOrder(uint8_t value){
//ESPnow order sent to man-and-chef
  t2order.x = value;
  esp_err_t result = esp_now_send(0, (uint8_t *) &t2order, sizeof(tmenu_struct));
  
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}

void loop() {
  /**
     @brief Get the ID corresponding to the command word 
     @return Return the obtained command word ID, returning 0 means no valid ID is obtained
  */
  page0.show();
  uint8_t CMDID = asr.getCMDID();
  switch (CMDID) {
    case 5:                                                               //If the command is “Order Vada Pav”
      page1.show();                                                       //Show page 1
      digitalWrite(statLED, HIGH);
      sendOrder(T2menu[0]);
      shortBeep();
      delay(3000);
      Serial.println("received'Order Vada Pav',command flag'5'");         //Serial transmits "received"Order Vada pav",command flag"5
      digitalWrite(statLED, LOW);
      break;

    case 11:                                                               //If the command is “Order Vada Pav”
      page1.show();                                                       //Show page 1
      digitalWrite(statLED, HIGH);
      sendOrder(T2menu[0]);
      shortBeep();
      delay(3000);
      Serial.println("received'Order Vada Pav',command flag'11'");         //Serial transmits "received"Order Vada pav",command flag"11
      digitalWrite(statLED, LOW);
      break;

    case 6:                                                               //If the command is “Order IDLI Sambar”
      page2.show();                                                       //Show page 2
      digitalWrite(statLED, HIGH);
      sendOrder(T2menu[1]);
      shortBeep();
      delay(3000);
      Serial.println("received'Order IDLI Sambar',command flag'6'");      //Serial transmits "received"Order IDLI Sambar",command flag"6
      digitalWrite(statLED, LOW);
      break;

    case 12:                                                               //If the command is “Order IDLI Sambar”
      page2.show();                                                       //Show page 2
      digitalWrite(statLED, HIGH);
      sendOrder(T2menu[1]);
      shortBeep();
      delay(3000);
      Serial.println("received'Order IDLI Sambar',command flag'12'");      //Serial transmits "received"Order IDLI Sambar",command flag"12
      digitalWrite(statLED, LOW);
      break;
      

    case 7:                                                               //If the command is “Order Rice Plate”
      page3.show();                                                       //Show page 3
      digitalWrite(statLED, HIGH);
      sendOrder(T2menu[2]);
      shortBeep();
      delay(3000);
      Serial.println("received'Order Rice Plate',command flag'7'");       //Serial transmits "received"Order Rice Plate",command flag"7
      digitalWrite(statLED, LOW);
      break;

    case 13:                                                               //If the command is “Order Rice Plate”
      page3.show();                                                       //Show page 3
      digitalWrite(statLED, HIGH);
      sendOrder(T2menu[2]);
      shortBeep();
      delay(3000);
      Serial.println("received'Order Rice Plate',command flag'13'");       //Serial transmits "received"Order Rice Plate",command flag"13
      digitalWrite(statLED, LOW);
      break;

    case 8:                                                               //If the command is “Order Butter Chicken”
      page4.show();                                                       //Show page 5
      digitalWrite(statLED, HIGH);
      sendOrder(T2menu[3]);
      shortBeep();
      delay(3000);
      Serial.println("received'Order Samosa',command flag'8'");           //Serial transmits "received"Order Samosa",command flag"8
      digitalWrite(statLED, LOW);
      break;
      
    case 14:                                                               //If the command is “Order Butter Chicken”
      page4.show();                                                       //Show page 5
      digitalWrite(statLED, HIGH);
      sendOrder(T2menu[3]);
      shortBeep();
      delay(3000);
      Serial.println("received'Order Samosa',command flag'14'");           //Serial transmits "received"Order Samosa",command flag"14
      digitalWrite(statLED, LOW);
      break;

    case 9:                                                               //If the command is “Order Butter Chicken”
      page5.show();                                                       //Show page 4
      digitalWrite(statLED, HIGH);
      sendOrder(T2menu[4]);
      shortBeep();
      delay(3000);
      Serial.println("received'Order Butter Chicken',command flag'9'");   //Serial transmits "received"Order Butter Chicken",command flag"9
      digitalWrite(statLED, LOW);
      break;

    case 15:                                                               //If the command is “Order Butter Chicken”
      page5.show();                                                       //Show page 4
      digitalWrite(statLED, HIGH);
      sendOrder(T2menu[4]);
      shortBeep();
      delay(3000);
      Serial.println("received'Order Butter Chicken',command flag'15'");   //Serial transmits "received"Order Butter Chicken",command flag"15
      digitalWrite(statLED, LOW);
      break;
      
    case 10:                                                              //If the command is “Order Biryani”
      page6.show();                                                       //Show page 6
      digitalWrite(statLED, HIGH);
      sendOrder(T2menu[5]);
      shortBeep();
      delay(3000);
      Serial.println("received'Order Biryani',command flag'10'");          //Serial transmits "received"Order Biryani",command flag"10
      digitalWrite(statLED, LOW);
      break;

    case 16:                                                              //If the command is “Order Biryani”
      page6.show();                                                       //Show page 6
      digitalWrite(statLED, HIGH);
      sendOrder(T2menu[5]);
      shortBeep();
      delay(3000);
      Serial.println("received'Order Biryani',command flag'16'");          //Serial transmits "received"Order Biryani",command flag"16
      digitalWrite(statLED, LOW);
      break;

    default:
      if (CMDID != 0) {
        Serial.print("CMDID = ");  //Printing command ID
        Serial.println(CMDID);
      }
  }
  delay(300);
}

//========================================================================================================================================//