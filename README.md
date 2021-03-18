# Welcome to Our Senior Design Project
# SOFWERX Drone Control

Our project utilizes the ESP8266 12E to receive data from 5 flex sensors, Voice Recognition module, MPU-6050 IMU and a 0.96" OLED. 
The ESP8266 is able to communicate with our drone utilizes its' built-in WiFi module. By using the WiFiUdp library for the ESP8266 12E 
we are able to send UDP packets to and from the Drone!

## DESIGN 1 ASSIGNMENTS
### Bi-weekly Report:

[SOFWERX_DRONECONTROL_T.A.,F.A.,N.M.A.,M.Y. .xlsx](https://github.com/NathanielMA/DroneControlProject/files/6098788/SOFWERX_DRONECONTROL_T.A.F.A.N.M.A.M.Y.1.xlsx)

### System Requirements Document:

[Final_SRD_SOFWERX_DRONECONTROL_T.A.,F.A.,N.M.A.,Y.M..docx](https://github.com/NathanielMA/DroneControlProject/files/6098780/Final_SRD_SOFWERX_DRONECONTROL_T.A.F.A.N.M.A.Y.M.docx)

### Product Design Review:

[PDR_T.A.,F.A.,N.M.A.,M.Y..pptx](https://github.com/NathanielMA/DroneControlProject/files/6098851/PDR_T.A.F.A.N.M.A.M.Y.pptx)



## Prototype Glove Designs

### Version 1
![Prototype glove](https://user-images.githubusercontent.com/79669935/110272504-60c1c400-7f98-11eb-9000-8dd6350ede5b.jpeg)

### Version 2
![IMG_0960_2](https://user-images.githubusercontent.com/79669935/110272606-98307080-7f98-11eb-8947-2f7fc802cc9b.jpg)


### Version 3

## TEAMMATES PLEASE READ:

Seeing that you may be new to using this glove that I have made. Here is a quick rundown on how to adjust the parameters in the code to fit the glove. Since this code took me nearly three months to develop, there is a bit to learn about the code.

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

<img width="275" alt="img1" src="https://user-images.githubusercontent.com/79669935/110270867-cb710080-7f94-11eb-86e5-4cc07cf62224.png">

After connecting to the access point, open your browser and it will redirect to a generated html webpage

<img width="472" alt="img2" src="https://user-images.githubusercontent.com/79669935/110271158-741f6000-7f95-11eb-8d4c-dea5953edf73.png">

Click on **Configure WiFi** and you'll be able to select your network and input your credentials

<img width="363" alt="img3" src="https://user-images.githubusercontent.com/79669935/110271243-95804c00-7f95-11eb-96eb-b40ba34dd0dd.png">


## Uploading new code

The code also features the use of ArduinoOTA, or Arduino Over-The-Air. It will allow you to compile and upload to the ESP8266 remotely! Simply turn on the board and ensure that it is connected to WiFi. The go to **Tools** and change the **Port** to the IP of your ESP8266.

<img width="496" alt="img5" src="https://user-images.githubusercontent.com/79669935/110271327-cb253500-7f95-11eb-800b-48123bca88bc.png">


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
**NOTE: Do not reuse return values!**
```
//###################################################################################################################################
//Current list of commands
//  0   -- WAIT FOR NEXT COMMAND
//  1   -- UPLOAD MISSION
//  2   -- EXECUTE MISSION
//  3   -- TERMINATE MISSION
//  4   -- ADJUST YAW
//  5   -- DIRECTION_RELATIVE
//  6   -- DIRECTION_ABSOLUTE
//  7   -- ALTITUDE_ADJUSTMENT
//  8   -- ORBIT
//  9   -- LAND
//  10  -- RETURN TO LAND
//  11  -- STARTUP COMMAND ~~NOTE: HANDLED IN LOOP
// 101  -- Cancel RTL AND OR LAND
// 123  -- IDENTIFIER FOR FIRST PHASE OF RTL GESTURE  ~~NOTE: SOLELY FOR TROUBLESHOOTING
// 201  -- Acknowledge RTL AND OR LAND
// 301  -- WiFi communications LOCKED
```
These values **CANNOT** be reused. Otherwise, to reuse them, a nest statement **MUST** be made on the simulation side to enable reuse. Much like hand gestures. Since we are dealing with numbers that don't involve a limited number of hand gestures. There is no purpose to reusing the values.

## Adding in voice commands

Voice commands are relatively easy to add to the code

Within the **Main** tab there are already 45 voice commands uploaded. 
**Note: You'll have to add the same commands to your module!**
**Each voice command is spoken as it is written in the code without VR**

Simply define the new voice command here with its respective index.
Here is the list of each voice command that is currently supported. There is no need to use a group number when adding in voice commands. The purpose of the groups were to to help myself separate and model commands.

```
#define DRONE                     (42)
#define DISABLE_VR                (43)
#define ENABLE_VR                 (44)
#define CANCEL_VR                 (1)
#define STOP_VR                   (2)  
#define NEXT_VR                   (3)
#define BACK_VR                   (4)

//start Group 1
#define DRONE                     (42)
#define START_VR                  (0)

//height Group 2 
//constant - DRONE
#define TEN_VR                    (37)
#define TWENTY_VR                 (38)
#define THIRTY_VR                 (39)
#define FOURTY_VR                 (40)
#define FIFTY_VR                  (41)

//Identifier Group A
//constants - Drone, Next, Cancel
//group 3
#define UPLOAD_MISSION_VR         (5)
#define DIRECTION_VR              (8)
#define ADJUST_YAW_VR             (9)
#define ALTITUDE_VR               (10)

//group 4
//constants - back, cancel
#define LAND_VR                   (26)
#define RTL_VR                    (27)
#define ORBIT_VR                  (25)

//mission group 5
//constants group 3, except upload mission
//constants group 0, except drone
#define EXECUTE_MISSION_VR        (6)
//Active only after mission is executed
#define TERMINATE_MISSION_VR      (7)

//direction group 6
//constants - cancel
#define RELATIVE_VR               (11)
#define ABSOLUTE_VR               (12)

//direction group 7
//constants - cancel
#define NORTH_VR                  (17)
#define EAST_VR                   (18)
#define SOUTH_VR                  (19)
#define WEST_VR                   (20)

//yaw group 8
//constants - cancel
#define PORT_VR                   (13)
#define STARBOARD_VR              (14)

//altitude group 9
//constants - cancel
#define INCREASE_VR               (15)
#define DECREASE_VR               (16)

//speed group 10
//constants - cancel
#define SLOW_VR                   (21)
#define SLOWER_VR                 (22)
#define FAST_VR                   (23)
#define FASTER_VR                 (24)

//unassigned
#define ONE_VR                    (28)
#define TWO_VR                    (29)
#define THREE_VR                  (30)
#define FOUR_VR                   (31)
#define FIVE_VR                   (32)
#define SIX_VR                    (33)
#define SEVEN_VR                  (34)
#define EIGHT_VR                  (35)
#define NINE_VR                   (36)
```
There is no need to create groups with the voice commands. The commented groups you see in the code were for me to visualize 
where in the code they will go and how they are used.

## Setting up the code for use with the Simulation
When you are finally ready to test your newly made commands with the SITL drone be sure to uncomment and comment back certain portions of the code

This code can be found in the main loop of the code in **Sofwerx_Glove_Microphone_v4.0_UDP** tab
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
This should become:
```
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


//display_current();
```

The next bit of code to ensure is uncommented is found in the **Commands_VR** tab

```
 /* comment if not using simulation
      if(startUp == true){
        while(Alt < 0.95*Height){
          UDP_RECEIVE_VR();
          UDP_SEND_VR();
          
          if(Height == 0){
            display_HEIGHT_INIT_VR();
          }
        }
        if(Alt >= 0.95*Height){
          display_SHOW = true;
          INIT_ALT = true;
        }
      }
      
      if(startUp == true and INIT_ALT == true){
        display_SEVEN_VR();
        LOAD_VR();   
        CURRENT_STATE_GESTURE_VR = 0;
      }
      */
      
      //Uncomment if not using Simulation
      
      display_SEVEN_VR();
      LOAD_VR();   
      CURRENT_STATE_GESTURE_VR = 0;
```

This should become:
```
///* comment if not using simulation
      if(startUp == true){
        while(Alt < 0.95*Height){
          UDP_RECEIVE_VR();
          UDP_SEND_VR();
          
          if(Height == 0){
            display_HEIGHT_INIT_VR();
          }
        }
        if(Alt >= 0.95*Height){
          display_SHOW = true;
          INIT_ALT = true;
        }
      }
      
      if(startUp == true and INIT_ALT == true){
        display_SEVEN_VR();
        LOAD_VR();   
        CURRENT_STATE_GESTURE_VR = 0;
      }
//*/
      
      //Uncomment if not using Simulation
      
      /*
      display_SEVEN_VR();
      LOAD_VR();   
      CURRENT_STATE_GESTURE_VR = 0;
      */
```

There you have it! The simulation is ready to be tested using the code.

**NOTE: Be sure to upload all current voice commands to your VR module!**

Be sure to use the SITL code I have provided. If you require it again please send me a message and I'll get it to you ASAP.

## Using the Simulation
First things, first:
Update the ip within the code to the ip of the esp module. The ip does show on the OLED on startup and it can be found within the Arduino IDE in the **port** section.
```
ip="YOUR_IP_HERE"

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)

def get_data():

	global data, n

	send_data = str(vehicle.location.global_relative_frame.alt) + "~" + str(vehicle.battery.level) + "~" + str(vehicle.groundspeed) + "~" 
	s.sendto(send_data.encode('utf-8'), (ip, 4210))
	data, address = s.recvfrom(4096)

	n = data.decode('utf-8')
```
One thing you may want to do in your VM is to download the socket library.

Simply use this command:

```
sudo apt-get install socket
```

Within the code you won't need 
```
import subprocess
```
or 
```
from six.moves import urllib
```
Since we are using UDP and no longer pulling from HTTP. urlib is no longer needed.

You wont need subprocess either since the purpose of using subprocess was to use **cmd prompt** to find the devices **MAC** address and pull the devices **IP** automatically without having to input it. Seeing that the **MAC** does not change upon moving location.

But since we are dealing with a VM, we are unable to detect WiFi devices with it.

**NOTE: If you need any help creating your command within the simulation environment. Please let me know and I can create more small tutorials on such.**

**A helpful link to refer to when finding out how to make a certain command is provided [HERE](https://dronekit-python.readthedocs.io/en/latest/).**

## Using the custom library for the glove to read/write/recall gesture data
This is a newly made library that I created to easily read/write/recall from the 256 bytes of EEPROM on the ESP 12E. I tried to make this library as easy to use as possible. Possibly can make it even easier, but this is how it goes.

First: The header file
```
#include "GESTURE_REASSIGN.h" //Header to include the newly made library for read/write/recall operations
```

Second: int's and definitions. We'll need to create some variables of type float and size 7 and some definitions to be able to easily remember what each function is calling. Each byte can store a value from 0 - 255.

Those are here:
```
//int's store values for all 5 flex sensors and X/Y data from the MPU6050
int YAW_SAVE[7];
int ALT_SAVE[7];
int UPLOAD_SAVE[7];
int EXECUTE_SAVE[7];
int TERMINATE_SAVE[7];
int RELATIVE_SAVE[7];
int ABSOLUTE_SAVE[7];
int RTL_SAVE[7];
int LAND_SAVE[7];

//Used by the header file to call the correct gesture type. If a new gesture is added. A simple addition the header is required. 
#define REA_YAW_SAVE            0x00
#define REA_ALT_SAVE            0x01
#define REA_UPLOAD_SAVE         0x02
#define REA_ORBIT_SAVE          0x03
#define REA_EXECUTE_SAVE        0x04
#define REA_TERMINATE_SAVE      0x05
#define REA_RELATIVE_SAVE       0x06
#define REA_ABSOLUTE_SAVE       0x07
#define REA_RTL_SAVE            0x08
#define REA_LAND_SAVE           0x09

//Storage address in the EEPROM for the default values for each gesture.
//Only store a value of either 0 or 1 in these addresses.
//0: Default (installed) hand gesture
//1: New (Created) hand gesture
#define REA_YAW_DEFAULT         0x46    
#define REA_ALT_DEFAULT         0x47 
#define REA_UPLOAD_DEFAULT      0x48
#define REA_EXECUTE_DEFAULT     0x49
#define REA_TERMINATE_DEFAULT   0x4A
#define REA_ORBIT_DEFAULT       0x4B
#define REA_RELATIVE_DEFAULT    0x4C
#define REA_ABSOLUTE_DEFAULT    0x4D
#define REA_RTL_DEFAULT         0x4E
#define REA_LAND_DEFAULT        0x4F
```
**NOTE: The addresses for the default values are stored from largest to smallest.**

Third: We want to initialize the class by instantiating it with a given name. For simplicity, we'll be using these.
```
GESTURE_REASSIGN reassignYAW(Pointer, Middle, Digitus, Pinky, Thumb, DegX, DegY, REA_YAW_SAVE);
GESTURE_REASSIGN reassignALT(Pointer, Middle, Digitus, Pinky, Thumb, DegX, DegY, REA_ALT_SAVE);
GESTURE_REASSIGN reassignUPL(Pointer, Middle, Digitus, Pinky, Thumb, DegX, DegY, REA_UPLOAD_SAVE);
GESTURE_REASSIGN reassignORB(Pointer, Middle, Digitus, Pinky, Thumb, DegX, DegY, REA_ORBIT_SAVE);
GESTURE_REASSIGN reassignEXE(Pointer, Middle, Digitus, Pinky, Thumb, DegX, DegY, REA_EXECUTE_SAVE);
GESTURE_REASSIGN reassignTER(Pointer, Middle, Digitus, Pinky, Thumb, DegX, DegY, REA_TERMINATE_SAVE);
GESTURE_REASSIGN reassignREL(Pointer, Middle, Digitus, Pinky, Thumb, DegX, DegY, REA_RELATIVE_SAVE);
GESTURE_REASSIGN reassignABS(Pointer, Middle, Digitus, Pinky, Thumb, DegX, DegY, REA_ABSOLUTE_SAVE);
GESTURE_REASSIGN reassignRTL(Pointer, Middle, Digitus, Pinky, Thumb, DegX, DegY, REA_RTL_SAVE);
GESTURE_REASSIGN reassignLAN(Pointer, Middle, Digitus, Pinky, Thumb, DegX, DegY, REA_LAND_SAVE);
```
We'll create an instance for each gesture to ease confusion.

Lastly, we'll cover the function calls for reading/writing/recalling.

### REA_WRITE()
To Write to the EEPROM we'll use **REA_WRITE()**
```
reassignYAW.REA_WRITE();
```
What this will do is take 64 samples of data from each flex sensor and the X/Y data from the MPU6050 and average the values. Then it will save the data to the EEPROM so that it persists after **POWER OFF**.

**NOTE: Only using the REA_WRITE() function to create a NEW hand gesture for an existing command. It won't overwrite the existing (default) hand gesture. It will just take its' place.**

### REA_READ()
To Read from the EEPROM and upload existing data from it, we'll use **REA_READ()**.
```
reassignYAW.REA_READ(uint8_t var);
```
What this will do is access the EEPROM and read the values stored at the specific addresses of the sensors for the gesture you are looking for. In this case, Yaw. When reading from the EEPROM, you'll want to save the data to a new variable like shown here:
```
for(int reader = 0x00; reader < 0x07; reader++){
      YAW_SAVE[reader] = reassignYAW.REA_READ(reader);
}
```
The variable holding whether the default gesture to be used or not will be allocated in the **GESTURES** tab of the code to allow for the glove to set it accordingly based on user preference.
```
YAW_SAVE[7] = reassignYAW.REA_READ(REA_YAW_DEFAULT); //reads the default storage byte at 0xF6
```

Currently, **ALL** programmed gestures have been added and are able to be reassigned, cleared, reverted to/from default/saved.

Below, is an example of the structure for reading from the EEPROM and determining whether or not to use the default gesture or the saved gesture.
```
if(TERMINATE_SAVE[7] == 0){
    GESTURE_TERMINATE_MISSION     =   Pointer > 50 and Middle > 50 and Digitus > 50 and Pinky > 40 and Thumb < 15 and DegX > 60 and DegY < 0 and DegY > -40;

    REA_UPLOAD = false;
  }
  else
  {
    if(REA_UPLOAD == false){
      for(reader = 0; reader < 7; reader++){
        TERMINATE_SAVE[reader] = reassignTER.REA_READ(reader);
      }
      REA_UPLOAD = true;
    }
    
    R_TER  = TERMINATE_SAVE[0] - 10 < Pointer  and Pointer   < TERMINATE_SAVE[0] + 10 and \
                    TERMINATE_SAVE[1] - 10 < Middle   and Middle    < TERMINATE_SAVE[1] + 10 and \
                    TERMINATE_SAVE[2] - 10 < Digitus  and Digitus   < TERMINATE_SAVE[2] + 10 and \
                    TERMINATE_SAVE[3] - 10 < Pinky    and Pinky     < TERMINATE_SAVE[3] + 10 and \
                    TERMINATE_SAVE[4] - 10 < Thumb    and Thumb     < TERMINATE_SAVE[4] + 10 and \
                    TERMINATE_SAVE[6] - 20 < DegY     and DegY      < TERMINATE_SAVE[6] + 20;
  }
  ```

### Reverting back to Default gesture without erasing old gesture
One this you may want to do is revert back to the old gesture and not have to overwrite the newly made gesture.

This can be easily done using **REA_DEFAULT()**.
```
reassignYAW.REA_DEFAULT();
```
This will change a value stored at the end of the EEPROM which is reponsible for handling this. The stored 1 or 0 is what will be changed.

### Clearing the gesture
Clearing the gesture can be done in one of two ways. 

1. Overwriting the gesture with **REA_WRITE()**
2. Erasing the data completely with **REA_CLEAR()**
```
reassignYAW.REA_CLEAR();
```
This function will completely erase the stored gesture and prevent the user from using the **REA_DEFAULT()** function to switch between two gestures. Because, the second gesture was deleted.

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
