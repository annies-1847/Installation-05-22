/*
  IMU Serial:

  Send IMU (accelerometer and gyroscope) data over Serial as raw bytes.
  Look at corresponding MaxMSP patch for parsing this data.

  Libraries:

  You will need to install the Arduino_LSM6DS3 library via the library manager
  before uploading.
*/
//----------------------------------------------------------------------------
#include <Arduino_LSM6DS3.h>
//----------------------------------------------------------------------------
const bool DEBUG = false;
//----------------------------------------------------------------------------
float ax = 0, ay = 0, az = 0, gx = 0, gy = 0, gz = 0;
bool updateOutput = false;

enum ItemType {Toothbrush, HairBrush, MouthWash, Soap};
//----------------------------------------------------------------------------
void setup()
{
  Serial.begin(9600);
  while (!Serial);

  Serial.write(Toothbrush);

  if (!IMU.begin())
  {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  while (!Serial.available()); // wait for a message from Max
  
  while (Serial.available())
    Serial.read();
    
  if (DEBUG) printImuInfo();
}
//----------------------------------------------------------------------------
void loop()
{
  if (DEBUG)
  {
    printImuData();
  }
  else
  {
    if (IMU.accelerationAvailable())
    {
      IMU.readAcceleration(ax, ay, az);
      updateOutput = true;
    }
    if (IMU.gyroscopeAvailable())
    {
      IMU.readGyroscope(gx, gy, gz);
      updateOutput = true;
    }

    if (updateOutput)
    {
      writeFloatBytesToSerial();
      while (Serial.available())
      {
        if (Serial.read() == 255)
          NVIC_SystemReset();
      }

      while (Serial.available() == 0)
      {
        if (IMU.accelerationAvailable())
        {
          IMU.readAcceleration(ax, ay, az);
        }
        if (IMU.gyroscopeAvailable())
        {
          IMU.readGyroscope(gx, gy, gz);
        }
      }

      updateOutput = false;
    }
  }
}
//----------------------------------------------------------------------------
