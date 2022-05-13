#include <esp_now.h>
#include <WiFi.h>
#include <SimpleTimer.h>
//引脚定义
volatile int pwm = 0;
volatile int IN1 = 32;
volatile int IN2 = 33;
volatile int IN3 = 25;
volatile int IN4 = 26;
volatile int ENA1 = 14;
volatile int ENA2 = 27;
SimpleTimer timer;//定义简单定时器

// 发送数据的结构示例
// 必须匹配接收器结构
typedef struct struct_message {
  int control;
  int pwm;
} struct_message;

// 创建一个名为myData的struct_message的数据结构
struct_message myData;
//此处简单定时器的作用为PWM控制小车移速
void Simple_timer_1() {
  pwm = pwm + 1;
  if (pwm == 255) {
    digitalWrite(ENA1, HIGH);
    digitalWrite(ENA2, HIGH);
    pwm = 0;

  }
  if (pwm == myData.pwm) {
    digitalWrite(ENA1, LOW);
    digitalWrite(ENA2, LOW);
  }
}
//小车电机控制
void car_run() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void car_left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void car_right() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void car_back() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void car_stop() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
}

// 当接收到数据时执行的回调函数 当接受到数据时才会运行以下函数
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  if (myData.control == 0) {
    car_stop();
    Serial.println("stop");
  } else if (myData.control == 1) {
    car_run();
    Serial.println("run");
  } else if (myData.control == 2) {
    car_back();
    Serial.println("back");
  } else if (myData.control == 3) {
    car_left();
    Serial.println("left");
  } else if (myData.control == 4) {
    car_right();
    Serial.println("right");
  }
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

  // 一旦ESPNow成功Init，我们将注册recv CB来获得recv packer信息
  esp_now_register_recv_cb(OnDataRecv);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA1, OUTPUT);
  pinMode(ENA2, OUTPUT);
  timer.setInterval(0.001L, Simple_timer_1);//1L=一毫秒
}

void loop() {
  timer.run();
}
