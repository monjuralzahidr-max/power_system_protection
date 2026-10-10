#include <Adafruit_GFX.h>   // Core graphics library
#include <MCUFRIEND_kbv.h>  // Hardware-specific library

MCUFRIEND_kbv tft;
Adafruit_GFX_Button on_btn, off_btn;


#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

#include <ZMPT101B.h>
#define SENSITIVITY 500.0f
#include "ACS712.h"

ZMPT101B voltageSensor(A8, 50.0);
ACS712 ACS(A9, 5.0, 1023, 100);

// Model Circut Parameter
float voltage;
float current;

bool load1 = true;
bool load2 = true;
bool load3 = true;
bool relay1 = true;
bool relay2 = true;
bool relay3 = true;
bool relay4 = true;


bool v_sensor;
bool c_sensor;

bool relay_s = true;
bool load_s = true;

void setup() {
  Serial.begin(9600);
  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(1);

  for (int i = 14; i <= 17; i++) {
    pinMode(i, OUTPUT);
    pinMode(i + 16, INPUT_PULLUP);
    digitalWrite(i, LOW);
  }

  pinMode(50, OUTPUT);
  pinMode(52, OUTPUT);
  digitalWrite(50, HIGH);
  digitalWrite(52, LOW);

  voltageSensor.setSensitivity(SENSITIVITY);
  ACS.autoMidPoint();

  //Animation
  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
  for (int i = 10000; i <= 10480; i++) {
    tft.setTextColor(CYAN);
    tft.print("100100011010101010100100010010101010011110111000100101001011101001");
    tft.println(i, BIN);
  }
  tft.fillScreen(BLACK);
  // Introduction
  tft.fillScreen(BLACK);
  tft.drawRect(50, 70, 390, 180, CYAN);
  tft.setCursor(102, 120);
  tft.setTextColor(GREEN);
  tft.setTextSize(1);
  tft.println("Ahsanullah University of Science and Technology");

  tft.setCursor(90, 135);
  tft.setTextColor(GREEN);
  tft.setTextSize(1);
  tft.println("Department of Electrical and Electronic Engineering");

  tft.setCursor(160, 150);
  tft.setTextColor(GREEN);
  tft.setTextSize(1);
  tft.println("Fourth Year First Semester");

  tft.setCursor(193, 165);
  tft.setTextColor(GREEN);
  tft.setTextSize(1);
  tft.println("POWER SYSTEM-II");

  tft.setCursor(200, 180);
  tft.setTextColor(GREEN);
  tft.setTextSize(1);
  tft.println("Section  : D1");

  tft.setCursor(209, 195);
  tft.setTextColor(BLUE);
  tft.setTextSize(1);
  tft.println("Group : 01");
  delay(2000);

  tft.fillScreen(BLACK);
  //DashBoard
  tft.setCursor(70, 135);
  tft.setTextColor(WHITE);
  tft.setTextSize(6);
  tft.println("DASH-BOARD");

  tft.setCursor(290, 300);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.println("- Designed by Mamunar Rahoman");

  delay(1000);



  tft.fillScreen(BLACK);
}

void loop() {
  if (relay1 == false || relay2 == false || relay3 == false || relay4 == false) {
    relay_s = false;
  }
  if (load1 == false || load2 == false || load3 == false) {
    load_s = false;
  }
  voltage = voltageSensor.getRmsVoltage();
  if (voltage < 10) {
    voltage = 0;
  }
  Serial.println(voltage);
  float average = 0;
  uint32_t start = millis();
  for (int i = 0; i < 100; i++) {
    //  select sppropriate function
    //  average += ACS.mA_AC_sampling();
    average += ACS.mA_AC();
  }
  current = average / 100.0;
  uint32_t duration = millis() - start;
  current = current / 1000;
  if (current <= 0.2 || current > 2) {
    current = 0;
  }
  // Serial.println(current);

  if (current > 1.2) {
    digitalWrite(14, LOW);
    tft.fillScreen(BLACK);
    tft.drawRect(2, 2, 478, 318, RED);  //Border

    tft.setCursor(130, 120);
    tft.setTextColor(GREEN);
    tft.setTextSize(2);
    tft.println("OVER LOAD CONDITION");

    tft.setCursor(130, 145);
    tft.setTextColor(GREEN);
    tft.setTextSize(2);
    tft.println("CURRENT > 1.2 A");

    tft.setCursor(80, 180);
    tft.setTextColor(CYAN);
    tft.setTextSize(3);
    tft.println("PLEASE REDUCE LOAD");

    tft.setCursor(220, 220);
    tft.setTextColor(CYAN);
    tft.setTextSize(3);
    tft.println("AND");

    tft.setCursor(33, 260);
    tft.setTextColor(CYAN);
    tft.setTextSize(5);
    tft.println("RESTART SYSTEM");

    for (;;) {
      tft.setCursor(50, 50);
      tft.setTextColor(RED);
      tft.setTextSize(5);
      tft.println("!!!WARNING!!!");
      delay(500);
      tft.setCursor(50, 50);
      tft.setTextColor(BLACK);
      tft.setTextSize(5);
      tft.println("!!!WARNING!!!");
      delay(500);
    }

  } else {
    if (voltage > 0) {
      v_sensor = false;

      tft.fillRect(428, 58, 70, 10, BLACK);
      tft.setCursor(430, 60);
      tft.setTextColor(GREEN);
      tft.println("ACTIVE");
    } else {
      v_sensor = true;

      tft.fillRect(428, 58, 70, 10, BLACK);
      tft.setCursor(430, 60);
      tft.setTextColor(RED);
      tft.println("DEACTIVE");
    }


// Normal Voltage Condition
    if (voltage > 200 && voltage < 230) {
      digitalWrite(14, HIGH);
      relay1 = false;
      c_sensor = false;
      tft.drawFastVLine(45, 174, 25, BLACK);  //switch
      tft.drawFastHLine(45, 186, 10, BLACK);  //switch
      tft.drawFastVLine(35, 180, 24, RED);    //switch

      tft.fillRect(428, 73, 70, 10, BLACK);
      tft.setCursor(430, 75);
      tft.setTextColor(GREEN);
      tft.println("PRESENT");


      tft.fillRect(428, 178, 30, 10, BLACK);
      tft.setCursor(430, 178);
      tft.setTextColor(GREEN);
      tft.println("YES");

      tft.fillRect(428, 193, 30, 10, BLACK);
      tft.setCursor(430, 193);
      tft.setTextColor(RED);
      tft.println("NO");

      tft.fillRect(428, 208, 30, 10, BLACK);
      tft.setCursor(430, 208);
      tft.setTextColor(RED);
      tft.println("NO");
    } else {
      digitalWrite(14, LOW);
      relay1 = true;

      c_sensor = true;
      tft.drawFastVLine(35, 180, 12, BLACK);  //switch
      tft.drawFastVLine(45, 174, 25, WHITE);  //switch
      tft.drawFastHLine(45, 186, 10, WHITE);  //switch

      tft.fillRect(428, 73, 70, 10, BLACK);
      tft.setCursor(430, 75);
      tft.setTextColor(RED);
      tft.println("ABSENT");

      tft.fillRect(428, 178, 30, 10, BLACK);
      tft.setCursor(430, 178);
      tft.setTextColor(RED);
      tft.println("NO");
    }

    if (voltage <= 200) {
      tft.fillRect(428, 193, 30, 10, BLACK);
      tft.setCursor(430, 193);
      tft.setTextColor(GREEN);
      tft.println("YES");

      tft.fillRect(428, 178, 30, 10, BLACK);
      tft.setCursor(430, 178);
      tft.setTextColor(RED);
      tft.println("NO");

      tft.fillRect(428, 208, 30, 10, BLACK);
      tft.setCursor(430, 208);
      tft.setTextColor(RED);
      tft.println("NO");
    } else {
      tft.fillRect(428, 193, 30, 10, BLACK);
      tft.setCursor(430, 193);
      tft.setTextColor(RED);
      tft.println("NO");
    }

    if (voltage > 230) {
      tft.fillRect(428, 208, 30, 10, BLACK);
      tft.setCursor(430, 208);
      tft.setTextColor(GREEN);
      tft.println("YES");

      tft.fillRect(428, 178, 30, 10, BLACK);
      tft.setCursor(430, 178);
      tft.setTextColor(RED);
      tft.println("NO");

      tft.fillRect(428, 193, 30, 10, BLACK);
      tft.setCursor(430, 193);
      tft.setTextColor(RED);
      tft.println("NO");


    } else {
      tft.fillRect(428, 208, 30, 10, BLACK);
      tft.setCursor(430, 208);
      tft.setTextColor(RED);
      tft.println("NO");
    }
  }

  if (digitalRead(30) == LOW) {
    digitalWrite(17, HIGH);
    load1 = false;
    relay2 = false;

    tft.fillRect(428, 88, 30, 10, BLACK);
    tft.setCursor(430, 90);
    tft.setTextColor(GREEN);
    tft.println("ON");
  } else {
    digitalWrite(17, LOW);
    load1 = true;
    relay2 = true;

    tft.fillRect(428, 88, 30, 10, BLACK);
    tft.setCursor(430, 90);
    tft.setTextColor(RED);
    tft.println("OFF");
  }

  if (digitalRead(31) == LOW) {
    digitalWrite(16, HIGH);
    relay3 = false;
    load2 = false;

    tft.fillRect(428, 103, 30, 10, BLACK);
    tft.setCursor(430, 105);
    tft.setTextColor(GREEN);
    tft.println("ON");
  } else {
    digitalWrite(16, LOW);
    relay3 = true;
    load2 = true;

    tft.fillRect(428, 103, 30, 10, BLACK);
    tft.setCursor(430, 105);
    tft.setTextColor(RED);
    tft.println("OFF");
  }

  if (digitalRead(32) == LOW) {
    digitalWrite(15, HIGH);
    relay4 = false;
    load3 = false;

    tft.fillRect(428, 118, 30, 10, BLACK);
    tft.setCursor(430, 120);
    tft.setTextColor(GREEN);
    tft.println("ON");
  } else {
    digitalWrite(15, LOW);
    relay4 = true;
    load3 = true;

    tft.fillRect(428, 118, 30, 10, BLACK);
    tft.setCursor(430, 120);
    tft.setTextColor(RED);
    tft.println("OFF");
  }

  //Extension
  tft.fillRect(428, 133, 30, 10, BLACK);
  tft.setCursor(430, 135);
  tft.setTextColor(WHITE);
  tft.println("NO");


  tft.drawRect(2, 2, 478, 318, CYAN);      //Border
  tft.drawFastHLine(2, 32, 478, CYAN);     //Voltage
  tft.drawFastVLine(238, 2, 30, CYAN);     //Voltage
  tft.drawFastVLine(350, 32, 286, CYAN);   //Frame
  tft.drawFastHLine(2, 52, 476, CYAN);     //Current Status
  tft.drawFastHLine(350, 150, 132, CYAN);  //Warning Status
  tft.drawFastHLine(350, 170, 132, CYAN);  //Warning Status_Start
  tft.drawFastHLine(350, 223, 132, CYAN);  //Warning Status_End
  tft.drawFastHLine(350, 243, 132, CYAN);  //warning condition start

  //symbols line
  tft.drawFastHLine(2, 245, 348, CYAN);   //first
  tft.drawFastHLine(2, 260, 348, CYAN);   //second
  tft.drawFastHLine(2, 275, 348, CYAN);   //third
  tft.drawFastVLine(60, 260, 58, CYAN);   //1st
  tft.drawFastVLine(118, 260, 58, CYAN);  //2nd
  tft.drawFastVLine(178, 260, 58, CYAN);  //3rd
  tft.drawFastVLine(238, 260, 58, CYAN);  //4th
  tft.drawFastVLine(292, 260, 58, CYAN);  //5th

  // symbol test
  tft.setCursor(135, 249);
  tft.setTextSize(1);
  tft.setTextColor(BLUE);
  tft.println("CIRCUIT SYMBOLS");

  tft.setCursor(19, 265);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.println("LOAD");

  on_btn.initButton(&tft, 32, 297, 30, 25, MAGENTA, WHITE, BLACK, "L", 1);
  on_btn.drawButton(load_s);

  tft.setCursor(76, 265);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.println("RELAY");

  on_btn.initButton(&tft, 90, 297, 30, 25, MAGENTA, WHITE, BLACK, "R", 1);
  on_btn.drawButton(relay_s);

  tft.setCursor(125, 265);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.println("V.SENSOR");

  on_btn.initButton(&tft, 148, 297, 30, 25, MAGENTA, WHITE, BLACK, "V", 1);
  on_btn.drawButton(v_sensor);

  tft.setCursor(184, 265);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.println("C.SENSOR");

  on_btn.initButton(&tft, 207, 297, 30, 25, MAGENTA, WHITE, BLACK, "A", 1);
  on_btn.drawButton(c_sensor);

  tft.setCursor(249, 265);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.println("SOURCE");

  on_btn.initButton(&tft, 266, 297, 30, 25, MAGENTA, WHITE, BLACK, "S", 1);
  on_btn.drawButton(v_sensor);

  tft.setCursor(303, 265);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.println("SWITCH");

  tft.drawFastVLine(319, 285, 10, WHITE);
  tft.drawFastHLine(307, 295, 25, WHITE);
  tft.drawFastHLine(297, 302, 15, RED);
  tft.drawFastHLine(327, 302, 15, RED);

  //Last Box
  // tft.drawFastHLine(360, 270, 106, CYAN);
  // tft.drawFastVLine(413, 233, 73, CYAN);
  tft.fillRect(120, 8, 115, 20, BLACK);
  tft.setCursor(10, 10);
  tft.setTextSize(2);
  tft.setTextColor(GREEN);
  tft.println("VOLTAGE :");  //Voltage
  tft.setCursor(130, 10);
  tft.setTextColor(CYAN);
  tft.print(voltage, 2);
  tft.println(" V");

  tft.fillRect(360, 8, 100, 20, BLACK);
  tft.setCursor(248, 10);
  tft.setTextSize(2);
  tft.setTextColor(GREEN);
  tft.println("CURRENT :");  //Current
  tft.setCursor(365, 10);
  tft.setTextColor(CYAN);
  tft.print(current, 2);
  tft.println(" A");

  tft.setCursor(140, 39);
  tft.setTextSize(1);
  tft.setTextColor(BLUE);
  tft.println("MODEL CIRCUIT");  //MODEL CIRCUIT

  tft.setCursor(371, 39);
  tft.setTextSize(1);
  tft.setTextColor(BLUE);
  tft.println("CURRENT STATUS");  //CURRENT STATUS

  tft.setCursor(357, 60);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.println("VOLTAGE    :");

  tft.setCursor(357, 75);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.println("CURRENT    :");

  tft.setCursor(357, 90);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.println("LOAD-I     :");

  tft.setCursor(357, 105);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.println("LOAD-II    :");

  tft.setCursor(357, 120);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.println("LOAD-III   :");

  tft.setCursor(357, 135);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.println("EXTENSION  :");

  tft.setCursor(372, 157);
  tft.setTextSize(1);
  tft.setTextColor(BLUE);
  tft.println("WARNING STATUS");  //WARNING STATUS

  tft.setCursor(357, 178);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.println("NORMAL VOL.:");

  tft.setCursor(357, 193);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.println("UNDER VOL. :");

  tft.setCursor(357, 208);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.println("OVER VOL.  :");

  tft.setCursor(365, 230);
  tft.setTextSize(1);
  tft.setTextColor(BLUE);
  tft.println("WARNING CONDITION");  //WARNING CONDITION

  tft.setCursor(357, 249);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.println("OV RANGE   :");

  tft.setCursor(430, 249);
  tft.setTextSize(1);
  tft.setTextColor(YELLOW);
  tft.println(">230 V");

  tft.setCursor(357, 262);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.println("UV RANGE   :");

  tft.setCursor(430, 262);
  tft.setTextSize(1);
  tft.setTextColor(YELLOW);
  tft.println("<200 V");

  tft.setCursor(357, 277);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.println("NV RANGE   :");

  tft.setCursor(430, 277);
  tft.setTextSize(1);
  tft.setTextColor(YELLOW);
  tft.println("200-230V:");

  tft.setCursor(357, 292);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.println("RATED CUR. :");

  tft.setCursor(430, 292);
  tft.setTextSize(1);
  tft.setTextColor(YELLOW);
  tft.println(">1.2 A");

  tft.setCursor(357, 306);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.println("NORMAL CUR.:");

  tft.setCursor(430, 306);
  tft.setTextSize(1);
  tft.setTextColor(YELLOW);
  tft.println("0-1.2 A");

  // Model_Circuit_symbol
  on_btn.initButton(&tft, 35, 152, 30, 25, MAGENTA, WHITE, BLACK, "S", 1);
  on_btn.drawButton(v_sensor);

  on_btn.initButton(&tft, 85, 152, 30, 25, MAGENTA, WHITE, BLACK, "V", 1);
  on_btn.drawButton(v_sensor);

  on_btn.initButton(&tft, 35, 90, 30, 25, MAGENTA, WHITE, BLACK, "A", 1);
  on_btn.drawButton(c_sensor);

  on_btn.initButton(&tft, 85, 90, 30, 25, MAGENTA, WHITE, BLACK, "R1", 1);
  on_btn.drawButton(relay1);

  on_btn.initButton(&tft, 140, 125, 30, 25, MAGENTA, WHITE, BLACK, "R2", 1);
  on_btn.drawButton(relay2);
  on_btn.initButton(&tft, 140, 180, 30, 25, MAGENTA, WHITE, BLACK, "L1", 1);
  on_btn.drawButton(load1);

  on_btn.initButton(&tft, 200, 125, 30, 25, MAGENTA, WHITE, BLACK, "R3", 1);
  on_btn.drawButton(relay3);
  on_btn.initButton(&tft, 200, 180, 30, 25, MAGENTA, WHITE, BLACK, "L2", 1);
  on_btn.drawButton(load2);

  on_btn.initButton(&tft, 260, 125, 30, 25, MAGENTA, WHITE, BLACK, "R4", 1);
  on_btn.drawButton(relay4);
  on_btn.initButton(&tft, 260, 180, 30, 25, MAGENTA, WHITE, BLACK, "L3", 1);
  on_btn.drawButton(load3);

  tft.setCursor(290, 149);
  tft.setTextSize(1);
  tft.setTextColor(RED);
  tft.println("EXTENSION");

  // on_btn.initButton(&tft, 320, 152, 42, 25, MAGENTA, WHITE, BLACK, "MOTOR", 1);
  // on_btn.drawButton(motor);


  //Model Circuit wiring

  tft.drawFastVLine(140, 90, 23, RED);
  tft.drawFastVLine(200, 90, 23, RED);
  tft.drawFastVLine(260, 90, 23, RED);
  tft.drawFastVLine(320, 90, 50, RED);
  tft.drawFastVLine(140, 138, 30, RED);
  tft.drawFastVLine(200, 138, 30, RED);
  tft.drawFastVLine(260, 138, 30, RED);
  tft.drawFastVLine(140, 193, 26, RED);
  tft.drawFastVLine(200, 193, 26, RED);
  tft.drawFastVLine(260, 193, 26, RED);
  tft.drawFastVLine(320, 165, 54, RED);
  tft.drawFastVLine(85, 165, 54, RED);
  tft.drawFastVLine(35, 103, 37, RED);
  tft.drawFastVLine(85, 120, 20, RED);
  tft.drawFastVLine(35, 165, 15, RED);
  tft.drawFastVLine(35, 195, 24, RED);

  //Extention point

  tft.fillCircle(320, 142, 3, RED);
  tft.fillCircle(320, 162, 3, RED);

  tft.drawFastHLine(100, 90, 220, RED);
  tft.drawFastHLine(35, 219, 286, RED);
  tft.drawFastHLine(50, 90, 20, RED);
  tft.drawFastHLine(35, 120, 50, RED);
}
