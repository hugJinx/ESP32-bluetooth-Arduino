/*
   @file: ESP32_CAR_TX
   @author: Zhou Xinyu
   @version: 1.0
   @date: 2021.12.31
   @brief 智能手柄控制小车
   @note:开发板为ESP32，两个开发板直接用的是WIFI模块和ESP-NOW协议进行通信
*/
#include <esp_now.h>
#include <WiFi.h>

volatile int  JoyX;
volatile int  JoyY;
volatile int  JoySW;
volatile int KeyUp;
volatile int KeyDown;


// 此处为接收的ESP32的MAC地址
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// 发送数据的结构示例
// 必须匹配接收器结构
typedef struct struct_message {
  int control;
  int pwm;
} struct_message;

// 创建一个名为myData的struct_message的数据结构
struct_message myData;

// 当数据发送时回调
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  // 将设备设置为Wi-Fi站
  WiFi.mode(WIFI_STA);

  // 初始化 ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // 一旦ESPNow成功初始化，我们将注册Send CB来获取发送报文的状态
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  myData.control = 0;//0:stop 1:run 2:back 3:left 4:right
  myData.pwm = 75;
  pinMode(33, INPUT_PULLUP); //设置摇杆SW为上拉输入模式
  pinMode(34, INPUT);//摇杆34,35脚设置为输入模式
  pinMode(35, INPUT);
  pinMode(25, OUTPUT);
  digitalWrite(25, HIGH);
  pinMode(27, OUTPUT);
  digitalWrite(27, HIGH);
}

void joy() {
  //摇杆函数 进入函数执行摇杆的一系列操作 此处可以自由发挥
  //判断摇杆y轴状态 JoyY这里定义>3500表示正推 <350表示反推
  if (JoyY > 3500)
  {
    myData.control = 1;
    Serial.println("joyUp");
  }
  else if (JoyY < 350)
  {
    myData.control = 2;
    Serial.println("joyDown");
  }
  //判断摇杆x轴状态 JoyX这里定义>3500表示正推 <350表示反推
  if (JoyX > 3500)
  {
    myData.control = 3;
    Serial.println("joyLeft");
  }
  else if (JoyX < 350)
  {
    myData.control = 4;
    Serial.println("joyRight");
  }
  //当摇杆没有操作时停止
  if (JoyX < 3500 && JoyX > 350 && JoyY > 350 && JoyY < 3500) {
    myData.control = 0;
  }
}

void key() {
  //按键函数 进入函数执行按键的一系列操作
  if (KeyUp == 0) {
    if (myData.pwm != 240) {
      myData.pwm += 15;
    }
    while (KeyUp == 0) {
      KeyUp = digitalRead(25);
    }
    Serial.println("KeyOn YES!");
  }
  if (KeyDown == 0) {
    if (myData.pwm != 15) {
      myData.pwm -= 15;
    }
    while (KeyDown == 0) {
      KeyDown = digitalRead(27);
    }
    Serial.println("KeyDown YES!");
  }
}

void loop() {
  JoyX = analogRead(35);
  JoyY = analogRead(34);
  JoySW = digitalRead(33);
  KeyUp  = digitalRead(25);
  KeyDown = digitalRead(27);
  joy();
  key();
  // 通过ESP-NOW发送信息
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}
