/*
HydroController v0.1
Jacob Quick Jul-7-2017

This sketch contains control functions for the hydroponic system,
including manipulation of a 4 port relay shield controlling a
water pump, air pump, and two banks of grow lights. Also, various
sensor inputs will be included to control automatic functionality.
There will be two functional modes: automatic and maintenance.
Automatic mode will initiate functions based on sensor input
and maintenance mode will be operated through a set of switches.
LED indicator lights will also be used in conjunction with 
the switches and automatic functions.
*/

//Pin Assignments - DIGITAL PINS UNLESS DENOTED ANALOG IN THE NAME.

const int pumpSwitchIn = 5;
const int airSwitchIn = 6;
const int modeSelectSwitchIn = 7;
const int pumpRelayOut = 8;
const int airRelayOut = 9;
const int light1RelayOut = 10;
const int light2RelayOut = 11;
const int modeAutoLedOut = 12;
const int modeMaintLedOut = 13;
const int waterSensorAnalogIn = 3; //Analog 3
//const int envSensorIO = 4; // Optional if using DHT
const int waterTempSensorAnalogIn= 4; //Analog 4
const int indicatorLedOut = 3;

//variables
double waterSensorThreshold = 0.0;
unsigned long lastPumpCycle;
unsigned long lastLightCycle;
boolean pumpState = true;
boolean lightState = true;

//Pin modes, w/ exception of envSensorIO, will not change.

void setup() {
  pinMode(pumpSwitchIn, INPUT);
  pinMode(airSwitchIn, INPUT);
  pinMode(modeSelectSwitchIn, INPUT);
  pinMode(pumpRelayOut, OUTPUT);
  pinMode(airRelayOut, OUTPUT);
  pinMode(light1RelayOut, OUTPUT);
  pinMode(light2RelayOut, OUTPUT);
  pinMode(modeAutoLedOut, OUTPUT);
  pinMode(modeMaintLedOut, OUTPUT);
  pinMode(waterSensorAnalogIn, INPUT);
  //pinMode(envSensorIO, OUTPUT); //will swap during I2C exchange w/ DHT sensor
  pinMode(waterTempSensorAnalogIn, INPUT);
  
  
}

void loop(){
  //Checking for mode
  if(digitalRead(modeSelectSwitchIn) == HIGH){
    //Auto mode active
    
    //Verify maintenance mode switches are switched off for auto mode
    checkMaintSwitches(modeAutoLedOut, pumpSwitchIn, airSwitchIn);
    digitalWrite(modeAutoLedOut, HIGH);
    digitalWrite(airRelayOut, HIGH);
    digitalWrite(pumpRelayOut, HIGH);
    lastPumpCycle = millis();
    while(true){
    if(waterSensorAnalogIn > waterSensorThreshold){
      digitalWrite(pumpRelayOut, LOW);
    }
    if ((millis() - lastPumpCycle) > 900000.0){
      digitalWrite(pumpRelayOut, HIGH);
      lastPumpCycle = millis()+900000.0;
    }
    if ((millis() - lastLightCycle) > 43200000.0){
      lastLightCycle = cycleLight(light1RelayOut, light2RelayOut, lightState);
      if(lightState){
        lightState = false;
      }
      else{
        lightState =true;
      }
    }
    if(digitalRead(modeSelectSwitchIn) == LOW){
      digitalWrite(modeAutoLedOut, LOW);
      break;
    }
    }
  }
  else{
    //MaintMode
    checkMaintSwitches(modeMaintLedOut, pumpSwitchIn, airSwitchIn);
    digitalWrite(modeMaintLedOut, HIGH);
    while(true){
      if(digitalRead(pumpSwitchIn) == HIGH){
        digitalWrite(pumpRelayOut, HIGH);
      }
      else{
        digitalWrite(pumpRelayOut, LOW);
      }
      if(digitalRead(airSwitchIn) == HIGH){
        digitalWrite(airRelayOut,  HIGH);        
      }
      else{
        digitalWrite(airRelayOut, LOW);
      }
      if(digitalRead(modeSelectSwitchIn) == HIGH){
        break;
        digitalWrite(modeMaintLedOut, LOW);
      }
    }
    
  }
}

void checkMaintSwitches(int indicator, int s1, int s2){
  while((digitalRead(s1) == HIGH) || (digitalRead(s2) == HIGH)){
    digitalWrite(indicator, HIGH);
    delay(250);
    digitalWrite(indicator, LOW);
  }
}
unsigned long cycleLight(int r1, int r2, boolean state){
  if(state){
    digitalWrite(r1, HIGH);
    digitalWrite(r2, HIGH);
  }
  else{
    digitalWrite(r1, LOW);
    digitalWrite(r2, LOW);
  }
  return millis();
}
