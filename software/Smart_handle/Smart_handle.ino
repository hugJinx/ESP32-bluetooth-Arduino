/*
   @file: Smart_handle
   @author: Zhou Xinyu
   @version: 1.0
   @date: 2021.12.31
   @brief 智能手柄
   @note:开发板为ESP32，这里用的是BleCombo库，主题设置为键盘但也可以控制鼠标
*/
#include <BleCombo.h>
#include <SimpleTimer.h>
SimpleTimer timer;//定义简单定时器对象
//定义全局变量 方便读取
volatile int KeyUp;
volatile int KeyDown;
volatile int KeyLeft;
volatile int KeyRight;
volatile int Key_Front_Left;
volatile int Key_Front_Right;
//以下三个定义为摇杆
volatile int  JoyX;
volatile int  JoyY;
volatile int  JoySW;

volatile bool Genshin = 0; //原神模式 默认为0 关闭 值为1时开启

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);//初始化波特率为115200
  Keyboard.begin();// 蓝牙键盘初始化
  Mouse.begin();//蓝牙鼠标初始化
  Serial.println("Starting BLE Keyboard&Mouse!");//串口打印信息

  pinMode(33, INPUT_PULLUP); //设置摇杆SW为上拉输入模式
  pinMode(34, INPUT);//摇杆34,35脚设置为输入模式
  pinMode(35, INPUT);

  //此处配置按键为输出模式并设为高电平
  pinMode(14, OUTPUT);
  digitalWrite(14, HIGH);
  pinMode(25, OUTPUT);
  digitalWrite(25, HIGH);
  pinMode(26, OUTPUT);
  digitalWrite(26, HIGH);
  pinMode(27, OUTPUT);
  digitalWrite(27, HIGH);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  pinMode(19, OUTPUT);
  digitalWrite(19, HIGH);

  timer.setInterval(0.001L, Simple_timer_1);//设置一个简单定时器1  1L=一毫秒
  Serial.println("Init OK!");
}

void Simple_timer_1() {
  //简单定时器1
  //读取按键和摇杆IO口的电平
  KeyUp     = digitalRead(25);
  KeyDown   = digitalRead(27);
  KeyLeft   = digitalRead(14);
  KeyRight  = digitalRead(26);
  Key_Front_Left = digitalRead(4);
  Key_Front_Right = digitalRead(19);
  JoyX = analogRead(35);
  JoyY = analogRead(34);
  JoySW = digitalRead(33);
}
///////////////////////////////////////////////////////////////
void joy() {
  //摇杆函数 进入函数执行摇杆的一系列操作 此处可以自由发挥
  //判断摇杆y轴状态 JoyY这里定义>3500表示正推 <350表示反推
  if (JoyY > 3500)
  {
    Mouse.move(0, -1);//Mouse.move函数控制鼠标移动
    Serial.println("joyUp");
  }
  else if (JoyY < 350)
  {
    Mouse.move(0, 1);
    Serial.println("joyDown");
  }
  //判断摇杆x轴状态 JoyX这里定义>3500表示正推 <350表示反推
  if (JoyX > 3500)
  {
    Mouse.move(-1, 0);
    Serial.println("joyLeft");
  }
  else if (JoyX < 350)
  {
    Mouse.move(1, 0);
    Serial.println("joyRight");
  }
  //判断摇杆按钮状态 当按钮被按下，读取值为0
  if (JoySW == 0) {
    Genshin = 1;//切换为原神模式0w0
    while (JoySW == 0) {
      JoySW = digitalRead(33);
    }
    Serial.println("joyPress");
  }
}
void key() {
  //按键函数 进入函数执行按键的一系列操作 此处可以自由发货
  //这里按普通模式PPT来设置的按键
  if (KeyUp == 0) {
    Keyboard.write(KEY_F5);//Keyboard.write函数 键盘按键按下并释放
    while (KeyUp == 0) {
      KeyUp = digitalRead(25);
    }
    Serial.println("KeyOn YES!");
  }
  if (KeyDown == 0) {
    Keyboard.write(KEY_ESC);
    while (KeyDown == 0) {
      KeyDown = digitalRead(27);
    }
    Serial.println("KeyDown YES!");
  }
  if (KeyLeft == 0) {
    Keyboard.write(KEY_LEFT_ARROW);
    while (KeyLeft == 0) {
      KeyLeft = digitalRead(14);
    }
    Serial.println("KeyLeft YES!");
  }
  if (KeyRight == 0) {
    Keyboard.write(KEY_RIGHT_ARROW);
    while (KeyRight == 0) {
      KeyRight = digitalRead(26);
    }
    Serial.println("KeyRight YES!");
  }
  if (Key_Front_Left == 0) {
    Mouse.click(MOUSE_LEFT);//Mouse.click函数 鼠标单次点击
    while (Key_Front_Left == 0) {
      Key_Front_Left = digitalRead(4);
    }
    Serial.println("Key_Front_Left YES!");
  }
  if (Key_Front_Right == 0) {
    Mouse.click(MOUSE_RIGHT);
    while (Key_Front_Right == 0) {
      Key_Front_Right = digitalRead(19);
    }
    Serial.println("Key_Front_Right YES!");
  }
}
//////////////////////原神模式0w0//////////////////////////////////
void joy_Genshin() {
  //摇杆函数 进入函数执行摇杆的一系列操作 此处可以自由发挥
  //JoyX判断摇杆y轴状态 这里定义>3500表示正推 <350表示反推
  //JoySW判断摇杆按钮状态 当按钮被按下，读取值为0
  if (JoyY > 3500)
  {
    Keyboard.press('w');//Keyboard.press函数 按下按键 注意是按下 并没有释放
    Serial.println("joyPress上");
  } else if (JoyY < 3500) {
    Keyboard.release('w');//Keyboard.release函数 释放按键
  }
  if (JoyY < 350)
  {
    Keyboard.press('s');
    Serial.println("joyPress下");
  } else if (JoyY > 350) {
    Keyboard.release('s');
  }
  //JoyX判断摇杆x轴状态 这里定义>3500表示正推 <350表示反推
  if (JoyX > 3500)
  {
    Keyboard.press('a');
    Serial.println("joyPress左");
  } else if (JoyX < 3500) {
    Keyboard.release('a');
  }
  if (JoyX < 350)
  {
    Keyboard.press('d');
    Serial.println("joyPress右");
  } else if (JoyX > 350) {
    Keyboard.release('d');
  }
  if (JoySW == 0) {
    Keyboard.write(32);
    while (JoySW == 0) {
      JoySW = digitalRead(33);
      JoyX = analogRead(35);
      JoyY = analogRead(34);
      //当摇杆按下时进行的操作  当JoySW为0时则为摇杆按下 使用&&进行判断
      if (JoyY > 3500 && JoySW == 0)
      {
        Mouse.move(0, -1);
        Serial.println("joyMouse上");
      }
      if (JoyY < 350 && JoySW == 0)
      {
        Mouse.move(0, 1);
        Serial.println("joyMouse下");
      }
      //JoyX判断摇杆x轴状态 这里定义>3500表示正推 <350表示反推
      if (JoyX > 3500 && JoySW == 0)
      {
        Mouse.move(-1, 0);
        Serial.println("joyMouse左");
      }
      if (JoyX < 350 && JoySW == 0)
      {
        Mouse.move(1, 0);
        Serial.println("joyMouse右");
      }
    }
    Serial.println("joySW");
  }
}
void key_Genshin() {
  //按键函数 进入函数执行按键的一系列操作 此处可以自由发货
  if (KeyUp == 0) {
    Mouse.press(MOUSE_RIGHT);
    Serial.println("KeyOn YES!");
  } else if (KeyUp == 1) {
    Mouse.release(MOUSE_RIGHT);
  }
  if (KeyDown == 0) {
    Keyboard.write('q');
    Serial.println("KeyDown YES!");
  }
  if (KeyLeft == 0) {
    Mouse.press(MOUSE_LEFT);
    Serial.println("KeyLeft YES!");
  } else if (KeyLeft == 1) {
    Mouse.release(MOUSE_LEFT);
  }
  if (KeyRight == 0) {
    Keyboard.press('e');
    Serial.println("KeyRight YES!");
  } else if (KeyRight == 1) {
    Keyboard.release('e');
  }
  if (Key_Front_Left == 0) {
    Keyboard.write(KEY_ESC);
    while (Key_Front_Left == 0) {
      Key_Front_Left = digitalRead(4);
    }
    Serial.println("Key_Front_Left YES!");
  }
  if (Key_Front_Right == 0) {
    Genshin = 0;//切换为普通模式
    while (Key_Front_Right == 0) {
      Key_Front_Right = digitalRead(19);
    }
    Serial.println("Key_Front_Right YES!");
  }
}
////////////////////////////////////////////////////////////////////////
void loop() {
  // put your main code here, to run repeatedly:
  timer.run();//运行简单定时器
  if (Keyboard.isConnected()) {
    //通过 Keyboard.isConnected() 来判断ESP32是否与电脑连接成功，如果没有连接，写在这里面的程序是不会执行的，只有在电脑与ESP32配对连接之后，里面的程序才会执行。
    // codes here...
    if (Genshin == 0) {
      joy();
      key();
    }
    else if (Genshin == 1) {
      joy_Genshin();
      key_Genshin();
    }
  }
}

//此处为按键值 从.h文件里面复制过来的
//const uint8_t KEY_LEFT_CTRL = 0x80;
//const uint8_t KEY_LEFT_SHIFT = 0x81;
//const uint8_t KEY_LEFT_ALT = 0x82;
//const uint8_t KEY_LEFT_GUI = 0x83;
//const uint8_t KEY_RIGHT_CTRL = 0x84;
//const uint8_t KEY_RIGHT_SHIFT = 0x85;
//const uint8_t KEY_RIGHT_ALT = 0x86;
//const uint8_t KEY_RIGHT_GUI = 0x87;
//
//const uint8_t KEY_UP_ARROW = 0xDA;
//const uint8_t KEY_DOWN_ARROW = 0xD9;
//const uint8_t KEY_LEFT_ARROW = 0xD8;
//const uint8_t KEY_RIGHT_ARROW = 0xD7;
//const uint8_t KEY_BACKSPACE = 0xB2;
//const uint8_t KEY_TAB = 0xB3;
//const uint8_t KEY_RETURN = 0xB0;
//const uint8_t KEY_ESC = 0xB1;
//const uint8_t KEY_INSERT = 0xD1;
//const uint8_t KEY_PRTSC = 0xCE;
//const uint8_t KEY_DELETE = 0xD4;
//const uint8_t KEY_PAGE_UP = 0xD3;
//const uint8_t KEY_PAGE_DOWN = 0xD6;
//const uint8_t KEY_HOME = 0xD2;
//const uint8_t KEY_END = 0xD5;
//const uint8_t KEY_CAPS_LOCK = 0xC1;
//const uint8_t KEY_F1 = 0xC2;
//const uint8_t KEY_F2 = 0xC3;
//const uint8_t KEY_F3 = 0xC4;
//const uint8_t KEY_F4 = 0xC5;
//const uint8_t KEY_F5 = 0xC6;
//const uint8_t KEY_F6 = 0xC7;
//const uint8_t KEY_F7 = 0xC8;
//const uint8_t KEY_F8 = 0xC9;
//const uint8_t KEY_F9 = 0xCA;
//const uint8_t KEY_F10 = 0xCB;
//const uint8_t KEY_F11 = 0xCC;
//const uint8_t KEY_F12 = 0xCD;
//const uint8_t KEY_F13 = 0xF0;
//const uint8_t KEY_F14 = 0xF1;
//const uint8_t KEY_F15 = 0xF2;
//const uint8_t KEY_F16 = 0xF3;
//const uint8_t KEY_F17 = 0xF4;
//const uint8_t KEY_F18 = 0xF5;
//const uint8_t KEY_F19 = 0xF6;
//const uint8_t KEY_F20 = 0xF7;
//const uint8_t KEY_F21 = 0xF8;
//const uint8_t KEY_F22 = 0xF9;
//const uint8_t KEY_F23 = 0xFA;
//const uint8_t KEY_F24 = 0xFB;
//
//const uint8_t KEY_NUM_0 = 0xEA;
//const uint8_t KEY_NUM_1 = 0xE1;
//const uint8_t KEY_NUM_2 = 0xE2;
//const uint8_t KEY_NUM_3 = 0xE3;
//const uint8_t KEY_NUM_4 = 0xE4;
//const uint8_t KEY_NUM_5 = 0xE5;
//const uint8_t KEY_NUM_6 = 0xE6;
//const uint8_t KEY_NUM_7 = 0xE7;
//const uint8_t KEY_NUM_8 = 0xE8;
//const uint8_t KEY_NUM_9 = 0xE9;
//const uint8_t KEY_NUM_SLASH = 0xDC;
//const uint8_t KEY_NUM_ASTERISK = 0xDD;
//const uint8_t KEY_NUM_MINUS = 0xDE;
//const uint8_t KEY_NUM_PLUS = 0xDF;
//const uint8_t KEY_NUM_ENTER = 0xE0;
//const uint8_t KEY_NUM_PERIOD = 0xEB;
//const MediaKeyReport KEY_MEDIA_NEXT_TRACK = {1, 0};
//const MediaKeyReport KEY_MEDIA_PREVIOUS_TRACK = {2, 0};
//const MediaKeyReport KEY_MEDIA_STOP = {4, 0};
//const MediaKeyReport KEY_MEDIA_PLAY_PAUSE = {8, 0};
//const MediaKeyReport KEY_MEDIA_MUTE = {16, 0};
//const MediaKeyReport KEY_MEDIA_VOLUME_UP = {32, 0};
//const MediaKeyReport KEY_MEDIA_VOLUME_DOWN = {64, 0};
//const MediaKeyReport KEY_MEDIA_WWW_HOME = {128, 0};
//const MediaKeyReport KEY_MEDIA_LOCAL_MACHINE_BROWSER = {0, 1}; // Opens "My Computer" on Windows
//const MediaKeyReport KEY_MEDIA_CALCULATOR = {0, 2};
//const MediaKeyReport KEY_MEDIA_WWW_BOOKMARKS = {0, 4};
//const MediaKeyReport KEY_MEDIA_WWW_SEARCH = {0, 8};
//const MediaKeyReport KEY_MEDIA_WWW_STOP = {0, 16};
//const MediaKeyReport KEY_MEDIA_WWW_BACK = {0, 32};
//const MediaKeyReport KEY_MEDIA_CONSUMER_CONTROL_CONFIGURATION = {0, 64}; // Media Selection
//const MediaKeyReport KEY_MEDIA_EMAIL_READER = {0, 128};
