## Welcome to Our Senior Design Project
## SOFWERX Drone Control

Our project utilizes the ESP8266 12E to receive data from 5 flex sensors, Voice Recognition module, MPU-6050 IMU and a 0.96" OLED. 
The ESP8266 is able to communicate with our drone utilizes its' built-in WiFi module. By using the WiFiUdp library for the ESP8266 12E 
we are able to send UDP packets to and from the Drone!

**Note for teammates:**

Seeing that you may be new to using this glove that I have made. Here is a quick rundown on how to adjust the parameters in the code to fit the glove.

## Intro

To start, ensure you have all the reuqired libraries installed. Here is the list of headers used in the code.
```markdown
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_ADS1015.h>
#include <SPI.h>

//WiFi libraries
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

//OTA libraries
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

//WiFi setup library
#include <WiFiManager.h>

//Voice recognition module libraries
#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"
```
If you haven't already, you may need to download the WiFiManager library (by Tzapu) and AdafruitBusIO from the library manager.
I have also provided the **ZIP file** for the other libraries via WhatsApp. If you require me to resend them send me an email at **Natedagreat2003@gmail.com**.

## Connecting to WiFi

If you haven't already noticed, the WiFiManager library automatically puts the ESP8266 into Accesspoint mode if it cannot connect to a previous know network.
To have it connect to WiFi, simply connect to the device in your computers WiFi settings.

## Uploading new code

The code also features the use of ArduinoOTA, or Arduino Over-The-Air. It will allow you to compile and upload to the ESP8266 remotely! Simply turn on the board and ensure that it is connected to WiFi. The go to **Tools** and change the **Port** to the IP of your ESP8266.

## Changing parameters

The easiest way to change parameters for the flex sensors or adjusting the MPU6050 axis is to edit variables in the **Data_Analyzer** tab.

**Note: the ADS1115 introduce up to 8 new adc pins to the ESP8266 (The built in ADC in usused).**

```
adc0 = ads.readADC_SingleEnded(0);
adc1 = ads.readADC_SingleEnded(1);
adc2 = ads.readADC_SingleEnded(2);
adc3 = ads.readADC_SingleEnded(3);
adc4 = ads2.readADC_SingleEnded(0);
adc5 = ads2.readADC_SingleEnded(1);
adc6 = ads2.readADC_SingleEnded(2);
adc7 = ads2.readADC_SingleEnded(3);
```

If you need to change the flex sensor ADCs adjust their variable.

```
Pointer = map( ===> adc0, 0, 9500, 0 , 100);
if(Pointer > 100){ Pointer = 100;}
if(Pointer < 0){ Pointer = 0;}
//1.7
Middle = map( ===> adc1, 0, 12000, 0 , 100);
if(Middle > 100){ Middle = 100;}
if(Middle < 0){ Middle = 0;}
//1.1
Digitus = map( ===> adc2, 0, 15000, 0 , 100);
if(Digitus > 100){ Digitus = 100;}
if(Digitus < 0){ Digitus = 0;}
```
If you have the **2.2in ADAFRUIT flex sensors** installed for the pinky and thumb simply adjust here as well
```
//2.2in ADAFRUIT flex sensors. Utilizing a voltage divider of 47K OHM
//Set unflexed sensor value equal to 0
min_adc4 = Max_value- ===> adc4;
min_adc5 = Max_value- ===> adc5;

//Scale value by an observed maximum value setting range from 0 to 100
//Pinky = float((min_adc4/scale_factor) - 14);//Pinky
//Thumb = float(1.2*(min_adc5/scale_factor) - 1.2*(17));//Thumb
Pinky = map(adc4, 10000, 6200, 0, 100);
Thumb = map(adc5, 10000, 7500, 0, 100);
```
Changing the MPU6050 IMU axis is also relatively simple
```
DegX = Ax * 90;
DegY = Ay * 90;
DegZ = Az * 90;
```
Simply adjust Ax, Ay, or Az. flip them around or make them negative. Do as you see fit for your gestures!

### Adding in Gestures

To add in a gesture head on over to the **Gestures** tab.

If you are unaware of how to adjust the OLED display to display the sensor values. You'll need to simply make a few adjustments.
Go to the **Main** tab and make these changes at the very bottom in the **main loop**
```
/*
if(startUp == false and starting == 0){
display_STARTUP();
}
else if(starting == 1){
currentMillis_start = millis();
display_HEIGHT_INIT();
}   


if(display_SHOW == true){
display_current();
}
if(startUp == true){
COMMAND();
COMMAND_VR();
}
*/

display_current();
```

Otherwise, continue to the **Gestures** tab.
Each gesture is defined here. They are also assigned to a boolean variable of the same name (with minor differences EG. capitalized)

### Gesture
```
Thumbs_up      =   Pointer > 50 and Middle > 50 and Digitus > 50 and Pinky > 40 and Thumb < 15 and DegY > 60 and 0 > DegY > -40;
Thumbs_up_nodir      =   Pointer > 50 and Middle > 50 and Digitus > 50 and Pinky > 40 and Thumb < 15;
```
### Boolean
```
if(Thumbs_up      ? Thumbs_UP = true : Thumbs_UP = false);
if(Thumbs_up_nodir  ? Thumbs_UP_NODIR = true: Thumbs_UP_NODIR = false);
```

To make it easy for you to add in gestures simply add their variable type and name to the **Gestures** tab instead of **Main**

### Re-using Gestures

One thing you may want to do is use pre existing gestures to aid in new gestures. Seeing that making new unique gestures can become rather confusing
in the grand scheme of things. Here's an example on how to re-use the existing gestures.
```
int COMMAND(){
  static enum { WAIT_COMMAND, LOCK_TOGGLE_1, LOCK_TOGGLE_2, UPLOAD_MISSION, EXECUTE_MISSION, TERMINATE_MISSION, STATIONARY, ADJUST_YAW, DIRECTION_REL, DIRECTION_ABS,\
  ALTITUDE_ADJ, ORBIT, RTL_STATE_1, MODE_RTL, LAND, LAND_STATE_1, ARM_AND_TAKEOFF} state = WAIT_COMMAND;
  
  int *STATE_GESTURE, HOLD_STATE_GESTURE, COUNTER;
  String Degy = String(-DegY); 
  String Degx = String(DegX); 
  String GyY = String(GY);
  
  //#########################
  bool REUSE_EXAMPLE = false; 
  //#########################
  
case WAIT_COMMAND:              //wait for a command
      STATE_GESTURE = &CURRENT_STATE_GESTURE;
      CURRENT_STATE_GESTURE = 0;
      
      UDP_SEND();
      
      if(currentMillis_disp - startMillis_disp >= period_disp){
        display_SHOW = true;
      }
      
      //#############################
      if(REUSE_EXAMPLE == false){       //Basic hand gestures for overall use
      //#############################
      
        if(closeFist_UP){
          delay(250);
          state = LOCK_TOGGLE_1;
          startMillis = millis();
        }
        if(Point_UP and UPLOAD == 0 and startUp == true){
          state = UPLOAD_MISSION;
        }
        if(Thumbs_UP_NODIR){
          state = ALTITUDE_ADJ;
        }
        if(Rocker_UP){
          state = ORBIT;
        }
        if(Thumbs_UP and UPLOAD == 1){
          state = EXECUTE_MISSION;
          startMillis_disp = millis();
        }
        if(Thumbs_DOWN and UPLOAD == 2){
          state = TERMINATE_MISSION;
          startMillis_disp = millis();
        }
        if(Hand_FLAT){
          state = DIRECTION_REL;
        }
        if(Hand_FLAT_TIN){
          state = DIRECTION_ABS;
        }
        if(Point_FORWARD){
          state = ADJUST_YAW;
          startMillis_disp = millis();
        }
        if(BECKON){
          delay(250);
          state = RTL_STATE_1;
          startMillis = millis();
        }      
        if(Point_DOWN){
          state = LAND;
          startMillis = millis();
        }
      }
      //##############################
      else if(REUSE_EXAMPLE == true){     //Enable premade hand gestures to be reused for another purpose (EG. Waypoints)
      //##############################
      
        ...
        ...
        ...
        
        if(Thumbs_DOWN){
          REUSE_EXAMPLE = false;          //breakout and return to basic gesture uses
        }
      }
    break;
    
    ...
    ... 
    ...
    }
```

Ensure when using the case statement to add your case to the **static enum** variable string.
```
int COMMAND(){
  static enum { WAIT_COMMAND, EXAMPLE_STATE } state = WAIT_COMMAND;
  
  currentMillis_disp = millis();
  
  switch (state){
    case WAIT_COMMAND:              //wait for a command
    
    switch(state){
      case WAIT_COMMAND:
      ...
      if(EXAMPLE_GESTURE){
        state = EXAMPLE_COMMAND;
      }
      ...
      ...
      break;
      
      case EXAMPLE_COMMAND:
      break;
    }
```

A good way to allow the statement to continually stay within the desired command, is to use the ! operator.
```
      ...
      ...
      ...
      
      case EXAMPLE_COMMAND:
        CURRENT_STATE_GESTURE = 1234;
        UDP_SEND();
        
        if(!EXAMPLE_GESTURE){
        state = WAIT_COMMAND; //break from current gesture and resturn to wait for another command
        }
      break;
    }
```

## Adding in voice commands

Voice commands are relatively easy to add to the code

Within the **Main** tab there are already 45 voice commands uploaded. 
**Note: You'll ahve to add the same commands to your module!**

Simply define the new voice command here with its respective index
```
#define DRONE                     (42)
#define DISABLE_VR                (43)
#define ENABLE_VR                 (44)
#define CANCEL_VR                 (1)
#define STOP_VR                   (2)  
#define NEXT_VR                   (3)
#define BACK_VR                   (4)
```
There is no need to create groups with the voice commands. The commented groups you see in the code were for me to visualize 
where in the code they will go and how they are used.

## MPU-6050 not working

If your MPU-6050 is not working for some reason. The main problem could be due to the AD0 pin.
The AD0 pin is the Slave address pin of the MPU-6050. It allows for up to two MPU-6050 boards to be installed.
The pin has two states, a high and a low state. 
When the pin is **high**, its' address is set to **0x01101001** or **0x69**.
When the pin is **Low**, its' address is set to **0x01101000** or **0x68**.

You can change this value in the **Main** tab. It'll look something like this.
```
// MPU6050 Slave Device Address
const uint8_t MPU6050SlaveAddress = 0x69;
```
