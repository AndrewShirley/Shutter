#include <Arduino.h>
#include <iostream>
#include "Shutter.h"
#include "Timer.h"

void HandleTimer(TimerInstance *T)
{
  Serial.printf("HandlerTimer is called with %s\n",T->TimerName);
}

void setup() {
  Serial.begin(115200);

  Timer *T = new Timer();

  long KillTime = millis() + 15000;
  T->AddTimer("Testers",1000,true,HandleTimer);
  T->AddTimer("Testers2",3000,true,HandleTimer);
  T->AddTimer("TestersOS",3000,false,HandleTimer);


  while(millis() < KillTime)
  {
   // Serial.printf(".");
    T->loop();
  }






  pinMode(GPIO_LDR,INPUT);
  analogSetPinAttenuation(GPIO_LDR, ADC_6db);
  pinMode(GPIO_LDR, INPUT_PULLDOWN);

  pinMode(GPIO_LED_RED, OUTPUT);
  pinMode(GPIO_LED_GREEN, OUTPUT);
  pinMode(GPIO_LED_BLUE, OUTPUT);
  pinMode(GPIO_SHUTTER, OUTPUT);
  digitalWrite(GPIO_SHUTTER, ShutterOFF);

  // Serial.printf("TRUE ");
  // digitalWrite(GPIO_SHUTTER, true);
  // delay(1000);

  // Serial.printf("FALSE ");
  // digitalWrite(GPIO_SHUTTER, false);
  // delay(1000);

  // Serial.printf("TRUE ");
  // digitalWrite(GPIO_SHUTTER, true);
  // delay(1000);

  // Serial.printf("FALSE ");
  // digitalWrite(GPIO_SHUTTER, false);
  // delay(1000);

}


long Timer_StatusUpdate = millis();   // Force an Update
long Timer_Sample = millis();

int LDR_LastReading = 0;
int LDR_Reading = 0;
int LDR_Delta = 0;
int FlashStage = 0;         // 0 = Waiting, 1=Flash Detected, Status LED ON, 2=Status LED OFF
int Timer_LEDStage=0;       // 
int LEDStage1_Time = 1000;    // minimum amount of time to ignore more flashes
int LEDStage2_Time = 30;
int FlashDetectionThreshold = 300;
long SampleTime = 10;       // Sample each 10mS
long Timer_ShutterOff = 0;  // Future time to shut off GPIO pin for the Shutter
long ShutterTimeOn = 1000;   // 100mS. Amount of time to short the shutter for

void loop()
{
  if(Timer_ShutterOff && millis() >= Timer_ShutterOff)
  {
    digitalWrite(GPIO_SHUTTER,ShutterOFF);
    Timer_ShutterOff = 0;
  }


  if(millis() >= Timer_Sample)
  {
    Timer_Sample += SampleTime;

    // Get new reading and calculate Delta from last reading
    LDR_Reading = analogRead(GPIO_LDR);
    if(LDR_LastReading)
      LDR_Delta = LDR_Reading - LDR_LastReading;

    if(millis() >= Timer_StatusUpdate)
    {
      Timer_StatusUpdate += 1000;
      Serial.printf("Reading=%d\n", LDR_Reading);
    }

    switch(FlashStage)
    {
      case 0:           // Waiting
        if(LDR_Delta > FlashDetectionThreshold)
        {
          FlashStage = 1;
          Timer_LEDStage = millis() + LEDStage1_Time;
          digitalWrite(GPIO_LED_RED, true);          
          digitalWrite(GPIO_SHUTTER,ShutterON);
          Timer_ShutterOff = millis() + ShutterTimeOn;

          Serial.printf("End of stage 0");
        }
        break;
      case 1:           // LEDs are ON for x mS. Should they be turned off?
          if(millis() > Timer_LEDStage)
          {
            FlashStage = 2;
            Timer_LEDStage = millis() + LEDStage2_Time;
            digitalWrite(GPIO_LED_RED, false);
            Serial.printf("End of stage 1");
          }
        break;
      case 2:           // LEDs are OFF for x mS. Should we end the lock-out?
          if(millis() > Timer_LEDStage)
          {
            FlashStage = 0;
            Timer_LEDStage = 0;
            Serial.printf("End of stage 2");
          }
        break;
    }

    LDR_LastReading = LDR_Reading;
  }
}