//Творець: Alexandre / https://t.me/lllllllIlllllllllllllllllll
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // Встанавлюємо дисплей
#include <GyverTimer.h>
GTimer_ms myTimer;  
#define CLK 6
#define DT 5
#define SW 4
#include "encMinim.h"
encMinim enc(CLK, DT, SW, 1, 1); // пін clk, пін dt, пін sw, напрямок (0/1), тип (0/1)
#define PIN_RELAY 11 // пін реле

boolean flag = 0;  // STOP / START для Cycle
unsigned long last_press;

unsigned long lastflash;
int RPM;
int cycle = 100;


void setup() {
  Serial.begin(9600);
  
  attachInterrupt(0,sens,RISING); //Підключить приривання на 2 пін при збільшенні сигналу
  
  myTimer.setInterval(300);       //Інтервал для RPM

  pinMode(PIN_RELAY, OUTPUT);     // Объявляем пін реле як вихід
  digitalWrite(PIN_RELAY, HIGH);  // Вкл реле - відправляємо високий сигнал  //колись потім убрати
  
  lcd.init();                     
  lcd.backlight();          // Вмикаєм підсвічування дисплея
  lcd.setCursor(0, 2);
  lcd.print("Cycle:");      // Цикли
  lcd.setCursor(12, 2);
  lcd.print("STOP");        // Цикл вимкнутий
}

void loop() {
  while(true) {   // Безкінечний цикл, так AlexGyver сказав і , так, в мене немає власної думки
    if (myTimer.isReady()) {
      lcd.setCursor(0, 0);
      lcd.print("RPM: 0       ");  // Об/хв
      lcd.setCursor(5, 0);
      ((micros()-lastflash)>1000000) ? lcd.print("0") : lcd.print(RPM); // якщо велика пауза 0, якщо ні то вивести RPM
 }

enc.tick();

  if (enc.isClick() || cycle < 1) {
    if (flag == 0 && millis() - last_press > 100 ) {
     flag = 1;
     last_press = millis();
     Serial.println(flag);
     digitalWrite(PIN_RELAY, LOW); // Вкл реле - посылаем низкий уровень сигнала
     lcd.setCursor(12, 2);
     lcd.print("START");           // Цикл вімкнутий
  }
  
  if ((flag == 1 && millis() - last_press > 500) || cycle < 1) {
    flag = 0;
    digitalWrite(PIN_RELAY, HIGH); // Викл реле - посылаем высокий уровень сигнала 
    lcd.setCursor(15, 2);
    lcd.print("  ");
    lcd.setCursor(12, 2);
    lcd.print("STOP");             // Цикл вимкнутий
    cycle = 100;                   // Повертаємо значення кількості циклів до 100
  }
}
  if (enc.isTurn() && flag == 0) {                       //якщо був оберт будь куди
    if (enc.isRight() && cycle < 1000) cycle += 100;     //якщо в право
    if (enc.isLeft() && cycle > 100) cycle -= 100;       //якщо в ліво
    lcd.setCursor(10, 2);
    lcd.print(" "); 
  }


  lcd.setCursor(7, 2);  // вивести кількість циклів
  lcd.print("    ");    // вивести кількість циклів
  lcd.setCursor(7, 2);  // вивести кількість циклів
  lcd.print(cycle);     // вивести кількість циклів
  
  Time();   // Виводимо на дисплей час з момента запуска ардуЇни
  }
}

void sens() {               // функція RPM , а також при кожному оберті cycle --
  RPM=60/((float)(micros()-lastflash)/1000000);  //разрахунок
  lastflash=micros();       //запам'ятати час останього оберту
  if (flag == 1) {          // якщо START
   cycle -- ;               // цикл -1
 }
}

void Time() {                  // функція таймера
  lcd.setCursor(0, 3);
  lcd.print("Time: ");
  int time = millis()/1000;
  int h = time / 3600;
  if (h < 10)  lcd.print("0"); 
  lcd.print(h);
  lcd.print(":");
  int m = (time / 60) % 60;
  if (m < 10)  lcd.print("0"); 
  lcd.print(m);
  lcd.print(":");
  int s = time % 60;
  if (s < 10) lcd.print("0"); 
  lcd.print(s);
}
