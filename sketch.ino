#include "DHTesp.h"
#include <LiquidCrystal_I2C.h>


const int DHT_PIN = 15;
DHTesp dhtSensor;
#define HEATER_PIN 1


LiquidCrystal_I2C lcd(0x27,16,2);


enum STATE {

  IDLE,
  HEATING,//0 - 29.5
  STABILIZING, //29.6 - 30.5
  TARGET_REACHED, //30.6
  STABLE, //30.6-35
  OVERHEAT //>35
};



STATE current_state = IDLE;



#define TEMP_HEATING_THRESHOLD 29.5
#define TEMP_STABILIZING_MIN 29.5
#define TEMP_STABILIZING_MAX 30.5
#define TEMP_OVERHEAT 35
unsigned long start_time = 0;
const unsigned long STABILIZING_DURATION = 5000 ;


void setup(){
  Serial.begin(115200);
  pinMode(HEATER_PIN, OUTPUT);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("GREETING");
  
};


void loop(){

  TempAndHumidity data = dhtSensor.getTempAndHumidity();

  float temp = data.temperature;

  Serial.println("TEMP: " + String(temp,2) + "C" );


  switch (current_state){
    case IDLE: 
    {
    if (temp < TEMP_HEATING_THRESHOLD) {
      current_state = HEATING;
    } else if (temp >= TEMP_STABILIZING_MIN && temp < TEMP_STABILIZING_MAX) 
    {
      current_state = STABILIZING;
    }else if (temp >= TEMP_STABILIZING_MAX && temp < TEMP_OVERHEAT){
      current_state = STABLE;
    }
     else if (temp >= TEMP_OVERHEAT) {
      current_state = OVERHEAT;
    }
      break;
    }

    case HEATING: 
    {
      if (temp >= TEMP_STABILIZING_MIN && temp <= TEMP_STABILIZING_MAX) {
        current_state = STABILIZING;
      }else if (temp >= TEMP_STABILIZING_MAX && temp < TEMP_OVERHEAT){
      current_state = STABLE;
    }
      else if (temp >= TEMP_OVERHEAT) {
      current_state = OVERHEAT;
    }
      break;
    }

    case STABILIZING: {
      if (temp >= TEMP_STABILIZING_MIN && temp < TEMP_STABILIZING_MAX){
        if (start_time == 0) {
        start_time = millis();  // Start timing only once
        } 
        if(millis()-start_time >= STABILIZING_DURATION){
          current_state = TARGET_REACHED;
          start_time = 0;
        }
      }
      else {
      // Temperature went out of range, reset timer and go back
        start_time = 0;
        if (temp < TEMP_HEATING_THRESHOLD) {
        current_state = HEATING;
        } else if (temp >= TEMP_OVERHEAT) {
          current_state = OVERHEAT;
        }if (temp >= TEMP_STABILIZING_MAX && temp < TEMP_OVERHEAT) {
        current_state = STABLE;
      }
      }
      break;
    }

    case TARGET_REACHED: {
      if (temp >= TEMP_STABILIZING_MIN && temp < TEMP_OVERHEAT) {
        current_state = STABLE;
      } else if (temp >= TEMP_OVERHEAT) {
      current_state = OVERHEAT;
      } else if (temp < TEMP_HEATING_THRESHOLD) {
      current_state = HEATING;
      }
      break;
    }

    case STABLE: {
      Serial.println("System is stable");
      if (temp >= TEMP_OVERHEAT) {
      current_state = OVERHEAT;
      } else if (temp < TEMP_HEATING_THRESHOLD) {
      current_state = HEATING;
      }
      
      break;
    }

    case OVERHEAT: {
      Serial.println("System is overheated");
 
      break;
    }
  }

  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp: " + String(data.temperature, 2) + " C"); 
  lcd.setCursor(0,1);
  digitalWrite(HEATER_PIN, current_state == HEATING || current_state == STABILIZING);


  switch (current_state) {
    case IDLE: Serial.println("IDLE"); lcd.print("IDLE"); break;
    case HEATING: Serial.println("HEATING");lcd.print("HEATING"); break;
    case STABILIZING: Serial.println("STABILIZING"); lcd.print("STABILIZING"); break;
    case TARGET_REACHED: Serial.println("TARGET_REACHED"); lcd.print("TARGET_REACHED"); break;
    case OVERHEAT: Serial.println("OVERHEAT");lcd.print("OVERHEAT"); break;
    case STABLE: Serial.println("STABLE"); lcd.print("STABLE"); break;

  }
  delay(1000);

};