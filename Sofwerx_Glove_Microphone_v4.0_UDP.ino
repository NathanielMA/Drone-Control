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

//define LxW of OLED display
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET    -1  // Reset pin # (or -1 if sharing reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define UDP_PORT 4210

WiFiUDP UDP;
char packet[255];

//##############################################################################################
//ADS1115 Address Initialization
//Assign ADS1115 to 2 separate addresses. 0x48 ADDR ground || 0x49 ADDR to VCC
//0x4A ADDR to SDA || 0x4B ADDR to SCL
//##############################################################################################
Adafruit_ADS1115 ads(0x48);
Adafruit_ADS1115 ads2(0x49);

//##############################################################################################
//Variables and Constants
//##############################################################################################

//counter
int flex_counter = 0;
int UPLOAD = 0;
int starting = 0;
int RTL = 0;
int i;
bool Land = false;
bool display_SHOW = true;
bool startUp = false;
int Height;

//Stop-watch for time sensitive functions
unsigned long startMillis;
unsigned long currentMillis;
unsigned long startMillis_start;
unsigned long currentMillis_start;
unsigned long startMillis_BATT;
unsigned long startMillis_FLASH;
unsigned long currentMillis_BATT;
unsigned long startMillis_disp;
unsigned long currentMillis_disp;
unsigned long startMillis_VR;
unsigned long currentMillis_VR;
const unsigned long period = 5000; //5000 milliseconds
const unsigned long startup = 2000;
const unsigned long period_disp = 1000; 
const unsigned long period_BATT = 1000; 

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Maximum observed value for 2.2in flex sensors. Any value above should be set to the max value.
//Minimum value is 0
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const int Max_value = 10848;
const int scale_factor = 35;
const int Pointer_rest = 7000;

String gestureString;
String gestureString_BUFFER;
String multiString; 
int CURRENT_STATE_GESTURE_VR, CURRENT_STATE_GESTURE;

//NULL value
const int Null = 0;

// MPU6050 Slave Device Address
const uint8_t MPU6050SlaveAddress = 0x69;

// Select SDA and SCL pins for I2C communication
static const uint8_t D6 = 12;
static const uint8_t D7 = 13;
const uint8_t scl = D6;
const uint8_t sda = D7;

// Select RX and TX for VR V3
const uint8_t D5 = 14;
const uint8_t D0 = 16;

VR myVR(D5,D0);

uint8_t record[7];
uint8_t buf[64];

int group = 0;
int ret, spd, rate, VR_y, VR_x;
bool INIT_ALT;
bool VR_ret = false;
bool DIR_VR = false;
bool load_VR = false; bool YAW_VR = false; bool ALT_VR = false; bool land_VR = false; bool REL_VR = false; bool REL_DIR_VR = false;
bool ABS_VR = false; bool DEC_VR = false; bool INC_VR = false;
int mission_STATUS = 0;
int next_PAGE = 0;

String RECORD[46] = {"START", "CANCEL", "STOP", "NEXT", "BACK", "UPLOAD", "EXECUTE", "TERMINATE",\
"DIRECTION", "YAW", "ALTITUDE", "RELATIVE", "ABSOLUTE", "PORT", "STARBOARD", "INCREASE", "DECREASE", "NORTH",\
"EAST", "SOUTH", "WEST", "SLOW", "SLOWER", "FAST", "FASTER", "ORBIT", "LAND", "RTL", "ONE", "TWO", "THREE",\
"FOUR", "FIVE", "SIX", "SEVEN", "EIGHT", "NINE", "TEN", "TWENTY", "THIRTY", "FOURTY", "FIFTY", "DRONE", "DISABLE_VR", "ENABLE_VR", "\0"};

//VR module commands list
//constants Group 0
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
#define TEN_VR                    (37) //UNUSED
#define TWENTY_VR                 (38) //UNUSED
#define THIRTY_VR                 (39) //UNUSED
#define FOURTY_VR                 (40) //UNUSED
#define FIFTY_VR                  (41) //UNUSED

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
#define RTL_VR                    (27) //UNUSED
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

//setup addition ADC's from 0 - 7
int16_t adc0, adc1, adc2, adc3, adc4, adc5, adc6, adc7;
int ADC0, ADC1, Middle, Pointer, Digitus, ADC5, Pinky, Thumb, min_adc4, min_adc5, max_adc;

//Hand Gestures
int Point_up, Thumbs_up, Thumbs_up_nodir, Thumbs_down, closeFist_up, Beckon, Point_down, Rocker_up, Point_forward,\
Hand_flat, Okay, Hand_flat_TIN, Number_one, Number_two, Number_three, Number_four, Number_five;

bool Point_UP, Thumbs_UP, Thumbs_UP_NODIR, Thumbs_DOWN, BECKON, Point_DOWN, closeFist_UP, Rocker_UP, Point_FORWARD,\
Hand_FLAT, OKAY, Hand_FLAT_TIN, Number_ONE, Number_TWO, Number_THREE, Number_FOUR, Number_FIVE;

// sensitivity scale factor respective to full scale setting provided in datasheet 
const uint16_t AccelScaleFactor = 16384;
const uint16_t GyroScaleFactor = 131;

// MPU6050 few configuration register addresses
const uint8_t MPU6050_REGISTER_SMPLRT_DIV   =  0x19;
const uint8_t MPU6050_REGISTER_USER_CTRL    =  0x6A;
const uint8_t MPU6050_REGISTER_PWR_MGMT_1   =  0x6B;
const uint8_t MPU6050_REGISTER_PWR_MGMT_2   =  0x6C;
const uint8_t MPU6050_REGISTER_CONFIG       =  0x1A;
const uint8_t MPU6050_REGISTER_GYRO_CONFIG  =  0x1B;
const uint8_t MPU6050_REGISTER_ACCEL_CONFIG =  0x1C;
const uint8_t MPU6050_REGISTER_FIFO_EN      =  0x23;
const uint8_t MPU6050_REGISTER_INT_ENABLE   =  0x38;
const uint8_t MPU6050_REGISTER_ACCEL_XOUT_H =  0x3B;
const uint8_t MPU6050_REGISTER_SIGNAL_PATH_RESET  = 0x68;

int16_t AccelX, AccelY, AccelZ, Temperature, GyroX, GyroY, GyroZ;

double Ax, Ay, Az, T, Gx, Gy, Gz; 
int DegX, DegY, DegZ, GZ, GY, j, k;

int starter;

String getValue(String data, char separator, int index){
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
  
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void setup() {
  myVR.begin(9600);
  
  Serial.begin(115200);
  Serial.println("Booting");
  
  WiFiManager wifiManager;
  wifiManager.autoConnect("SOFWERXGloveAutoAP");
  
  Serial.println(WiFi.hostname());  
  Serial.println();
  Serial.print("Configuring access point...");
  
  Wire.begin(sda, scl);           //wire sda and scl to pins D6 and D7
  MPU6050_Init();
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);                      // Don't proceed, loop forever
  }
  
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }
    
    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  //Have display show IP ADDRESS on Start-up
  display.display();
  delay(2);
  display.clearDisplay();
  
  display.clearDisplay();
  display.setTextColor(WHITE);//Initialize Display
  
  display.setTextSize(2);
  display.setCursor(0,0);
  display.print("IP ADDRESS");
  
  display.setTextSize(1);
  display.setCursor(10,30);
  display.print(String(WiFi.localIP()[0]) + "." + String(WiFi.localIP()[1]) + "."\
    + String(WiFi.localIP()[2]) + "." + String(WiFi.localIP()[3]));
  
  display.display();          //Display text
  delay(3000);
  
  UDP.begin(UDP_PORT);
  Serial.print("Listening on UDP part ");
  Serial.println(UDP_PORT);
  
  ads.begin();                //Begin ADS1115 readings
  
  if(myVR.clear() == 0){
    Serial.println("Recognizer cleared.");
  }else{
    Serial.println("VoiceRecognitionModule not found.");
  }
  
  
  record[0] = DRONE;
  
  if(myVR.load(record, 1) >= 0){
    printRecord(record, 1);
    Serial.println(F("loaded."));
  }
}

String Altitude, BATT, vval;
int Battery;
float Alt, Velocity;

//##############################################################################################
//LOOP
//##############################################################################################
void loop() {
  ArduinoOTA.handle();
  Data_Analyzer();
  GESTURE();
  
  int packetSize = UDP.parsePacket();
  if(packetSize){
    int len = UDP.read(packet, 255);
    if(len > 0){
      packet[len] = '\0';
      Altitude = getValue(packet, '~',0);
      BATT = getValue(packet, '~',1);
      vval = getValue(packet, '~', 2);
    }
    
    Battery = atoi(BATT.c_str());
    Alt = atof(Altitude.c_str());
    Velocity = atof(vval.c_str());
  }
  
  else{
  
    if(startUp == false){
      COMMAND_VR();
    if(OKAY){
      starter = 11;
    }
    String startingString = String(starter) + " " + String(Null) + " " + String(Null) + " " + String(Null) + " " + String(Null);
    
    UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
    UDP.write(startingString.c_str());
    UDP.endPacket();
    }
  }
  
  
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
}
