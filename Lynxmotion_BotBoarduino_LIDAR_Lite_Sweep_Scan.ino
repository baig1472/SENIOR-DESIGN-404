
/***********************************************************************************************************
/* Lynxmotion BotBoarduino LIDAR Lite Sweep Scan
/* Version: 1.0.0
/* By: scharette
/* Date: 2015-03-31
/* 
/* Short example based on the Servo library example "Sweep" example and the Pulsed Light 3D examples.
/* The servo motor* is attached to pin 5 and rotates from 0-180 degrees continuously by 1 degree increments.
/* The LIDAR lite is attached to VCC (+5 V DC), GND, SDA and SCL.
/* 
/* This examples uses the Wire library.
/*
/* Hardware used:
/* BotBoarduino: http://www.robotshop.com/en/lynxmotion-botboarduino-robot-controller.html?product-code=RB-Lyn-363
/* LIDAR-Lite Laser Rangefinder: http://www.robotshop.com/en/lidar-lite-laser-rangefinder-pulsedlight.html?product-code=RB-Pli-01
/* HS-645MG servo motor: http://www.robotshop.com/en/hitec-hs645mg-servo-motor.html?product-code=RB-Hit-29
/* (alternative) HS-422 servo motor: http://www.robotshop.com/en/hitec-hs422-servo-motor.html?product-code=RB-Hit-27
/* Bracket to hold the LIDAR: http://www.robotshop.com/en/aluminum-l-connector-bracket-asb-06.html?product-code=RB-Lyn-83
/* or this one: http://www.robotshop.com/en/lynxmotion-multi-purpose-sensor-housing.html?product-code=RB-Lyn-75
/* Bracket to hold the servo motor: http://www.robotshop.com/en/lynxmotion-aluminum-multi-purpose-servo.html?product-code=RB-Lyn-81
/*
/* *: Please note that while we used a HS-645MG in our example, the HS-422 and almost any other standard RC servo motor could perform this task.
/*
/**********************************************************************************************************/

#include <Wire.h>

#define    LIDARLite_ADDRESS   0x62          // Default I2C Address of LIDAR-Lite.
#define    RegisterMeasure     0x00          // Register to write to initiate ranging.
#define    MeasureValue        0x04          // Value to initiate ranging.
#define    RegisterHighLowB    0x8f          // Register to get both High and Low bytes in 1 call.

#include <Servo.h>


Servo myservo;
Servo myservo2;

/////// Lidar Information//////// 
double lat1 = 30.620848; 
double x1 = 757873.74;
double lon1 = -96.339896; 
double y_1 = 3390657.14;
double z1 = 0;
////////////////////////////////

double x2; double y2; double z2;

int phi; 
int theta;
int distance;

void setup()
{
  // Serial output
  Serial.begin(9600);
  Serial.println("< START >");
  
  // Servo control
  myservo.attach(6);
  myservo2.attach(7);


 
  // LIDAR control
  Wire.begin(); // join i2c bus
}

// Get a measurement from the LIDAR Lite
int lidarGetRange(void)
{
  int val = -1;
  
  Wire.beginTransmission((int)LIDARLite_ADDRESS); // transmit to LIDAR-Lite
  Wire.write((int)RegisterMeasure); // sets register pointer to  (0x00)  
  Wire.write((int)MeasureValue); // sets register pointer to  (0x00)  
  Wire.endTransmission(); // stop transmitting

  delay(20); // Wait 20ms for transmit

  Wire.beginTransmission((int)LIDARLite_ADDRESS); // transmit to LIDAR-Lite
  Wire.write((int)RegisterHighLowB); // sets register pointer to (0x8f)
  Wire.endTransmission(); // stop transmitting

  delay(20); // Wait 20ms for transmit
  
  Wire.requestFrom((int)LIDARLite_ADDRESS, 2); // request 2 bytes from LIDAR-Lite

  if(2 <= Wire.available()) // if two bytes were received
  {
    val = Wire.read(); // receive high byte (overwrites previous reading)
    val = val << 8; // shift high byte to be high 8 bits
    val |= Wire.read(); // receive low byte as lower 8 bits
  }
  
  return val;
}

    void serialPrintRange(double x2, double y2, double z2)
  //void serialPrintRange(double lat2, double lon2)
{
   // Serial.print("Latitude      : ");
    //Serial.print(lat2);
    //Serial.print("\t\tLongitude    :");
    //Serial.print(lon2);
    //Serial.print("\n");
    Serial.print("x2 : ");
    Serial.print(x2);
    Serial.print("\ty2  : ");
    Serial.println(y2);
    Serial.print("\t\tz2  : ");
    Serial.println(z2);
}

void loop()
{
      
  for(phi = 0; phi <= 90; phi +=1)
  {
    myservo2.write(phi);
    distance = lidarGetRange();    
    
    double delta_x = distance * cos(theta) * sin(phi);
    double delta_y = distance * cos(theta) * cos(phi);
    double delta_z = distance * sin(theta);
   
    x2 = x1 - delta_x; 
    y2 = y_1 - delta_y;
    z2 = delta_z - z1;
    
    serialPrintRange(x2, y2, z2);
    
  for(theta = 0; theta <= 90; theta += 90)
  {
    myservo.write(theta);
    distance = lidarGetRange();

    double delta_x = distance * cos(theta) * sin(phi);
    double delta_y = distance * cos(theta) * cos(phi);
    double delta_z = distance * sin(theta);
   
    x2 = x1 - delta_x; 
    y2 = y_1 - delta_y;
    z2 = delta_z - z1;
    
    serialPrintRange(x2, y2, z2);
    
    delay(500);
  }
    
  }
  
  for(phi = 90; phi <= 0; phi -=1)
  {
    myservo2.write(phi);
    distance = lidarGetRange();
    
    double delta_x = distance * cos(theta) * sin(phi);
    double delta_y = distance * cos(theta) * cos(phi);
    double delta_z = distance * sin(theta);
   
    x2 = x1 - delta_x; 
    y2 = y_1 - delta_y;
    z2 = delta_z - z1;
    
    serialPrintRange(x2, y2, z2);

   for(theta = 90; theta>=0; theta-=90)
  {
    myservo.write(theta);
    distance = lidarGetRange();

    double delta_x = distance * cos(theta) * sin(phi);
    double delta_y = distance * cos(theta) * cos(phi);
    double delta_z = distance * sin(theta);
   
     
    x2 = x1 - delta_x; 
    y2 = y_1 - delta_y;
    z2 = delta_z - z1;
    
    serialPrintRange(x2, y2, z2);
    delay(500);
  }
    
  }
  
}
