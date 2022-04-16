/*
 * file DFRobot_EC.ino
 * @ https://github.com/DFRobot/DFRobot_EC
 *
 * This is the sample code for Gravity: Analog Electrical Conductivity Sensor / Meter Kit V2 (K=1.0), SKU: DFR0300.
 * In order to guarantee precision, a temperature sensor such as DS18B20 is needed, to execute automatic temperature compensation.
 * You can send commands in the serial monitor to execute the calibration.
 * Serial Commands:
 *   enterec -> enter the calibration mode
 *   calec -> calibrate with the standard buffer solution, two buffer solutions(1413us/cm and 12.88ms/cm) will be automaticlly recognized
 *   exitec -> save the calibrated parameters and exit from calibration mode
 *
 * Copyright   [DFRobot](http://www.dfrobot.com), 2018
 * Copyright   GNU Lesser General Public License
 *
 * version  V1.0
 * date  2018-03-21
 */

#include "DFRobot_EC.h"
#include <EEPROM.h>

#define EC_PIN A1
float voltage,ecValue,temperature = 25;
DFRobot_EC ec;


//Carrega a biblioteca LiquidCrystal
#include <LiquidCrystal.h>
 
//Define os pinos que serão utilizados para ligação ao display
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

float calibration = 0.00; //change this value to calibrate
float calibration2;
const int analogInPin = A0; 
int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int buf[10],temp;
char e;
void setup()
{
  Serial.begin(115200);
  ec.begin();
  //Define o número de colunas e linhas do LCD
  lcd.begin(16, 2);
}

void loop(){
  
  char c = Serial.read();
  if(c == 'a'){
    while (e != 's'){
      e = Serial.read();
      Fun_conduti();
    }
    e = "mudar";
  }
  else if(c == 'b'){
    while (e != 's'){
      e = Serial.read();
      Fun_ph();
    }
    e = "mudar";
  }
  
}
float readTemperature()
{
  //add your code here to get the temperature from your temperature sensor
}

void Fun_ph(){
  for(int i=0;i<10;i++){ 
    buf[i]=analogRead(analogInPin);
   delay(30);
  }
  for(int i=0;i<9;i++){
   for(int j=i+1;j<10;j++){
    if(buf[i]>buf[j]){
      temp=buf[i];
      buf[i]=buf[j];
      buf[j]=temp;
    }
   }
  }
    avgValue=0;
    for(int i=2;i<8;i++)
    avgValue+=buf[i];
    float pHVol=(float)avgValue*5.0/1024/6;
    Serial.println(pHVol);
    float phValue = -5.70 * pHVol;
    Serial.println(phValue);
    if(phValue <= -19 && phValue >= -19.999){
      calibration2 = 23.94;
      Serial.println("1");
    }
    else if(phValue <= -17 && phValue >= -17.999){
      calibration2 = 25.12;
      Serial.println("2");
    }
    else if(phValue <= -15 && phValue >= -15.999){
      calibration2 = 25.78;
      Serial.println("3");
    }
    float phValue2 = -5.70 * pHVol + calibration2;
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("sensor = ");
    lcd.print(phValue2);
    delay(500);
}

void Fun_conduti(){
      static unsigned long timepoint = millis();
    if(millis()-timepoint>1000U)  //time interval: 1s
    {
      timepoint = millis();
      voltage = analogRead(EC_PIN)/1024.0*5000;   // read the voltage
      //temperature = readTemperature();          // read your temperature sensor to execute temperature compensation
      ecValue =  ec.readEC(voltage,temperature);  // convert voltage to EC with temperature compensation
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("temp:");
      lcd.print(temperature,1);
      lcd.print("^C");
      lcd.setCursor(1, 1);
      lcd.print("EC:");
      lcd.print(ecValue,2);
      lcd.print("ms/cm");
    }
    ec.calibration(voltage,temperature);          // calibration process by Serail CMD

}
