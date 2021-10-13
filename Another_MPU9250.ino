#include "Arduino.h"
#include <MPU9250_asukiaaa.h>

#ifdef _ESP32_HAL_I2C_H_
#define SDA_PIN 21
#define SCL_PIN 22
#endif

#define ROLLING_AVERAGE_LENGTH 20

class RollingAverage {  // declaration & definition of the RollingAverage class
public:
	// the default constructor
	RollingAverage() {
		index = 0;
		rollingTotal = 0;
	}

	//method to update rolling average with the latest reading
	float updateRav(float newValue) {
		rollingTotal -= buffer[index];  // subtract the oldest value from the rolling total
		rollingTotal += newValue;		// add the newValue to the rolling total
		buffer[index] = newValue;       // overwrite the oldest value in the buffer with newValue
		index = (index + 1) % ROLLING_AVERAGE_LENGTH;  // increment the buffer index (modulo the buffer size) ready for next time
		return rollingTotal/ROLLING_AVERAGE_LENGTH;
	}
private:
	// member variables
	float buffer[ROLLING_AVERAGE_LENGTH];  // cyclic buffer to hold the last N readings
	int index; 			// the index for the cyclic buffer, labelling the entry point for the next reading
	float rollingTotal; // the sum of the last N readings
};


MPU9250_asukiaaa mySensor;
float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;


void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("started");

#ifdef _ESP32_HAL_I2C_H_ // For ESP32
  Wire.begin(SDA_PIN, SCL_PIN);
  mySensor.setWire(&Wire);
#endif

  mySensor.beginAccel();
  mySensor.beginGyro();
  mySensor.beginMag();

  // You can set your own offset for mag values
  // mySensor.magXOffset = -50;
  // mySensor.magYOffset = -55;
  // mySensor.magZOffset = -10;
}

RollingAverage xRollingAv;
RollingAverage yRollingAv;
RollingAverage zRollingAv;
RollingAverage headingRollingAv;
float xRav, yRav, zRav, headingRav;

#define X_OFFSET (0)
#define Y_OFFSET (0)
#define Y_SENSITIVITY_CORRECTION 1




void loop() {
  uint8_t sensorId;

  /*if (mySensor.readId(&sensorId) == 0) {
    Serial.println("sensorId: " + String(sensorId));
  } else {
    Serial.println("Cannot read sensorId");
  }
*/

/*if (mySensor.accelUpdate() == 0) {
    aX = mySensor.accelX();
    aY = mySensor.accelY();
    aZ = mySensor.accelZ();
    aSqrt = mySensor.accelSqrt();
    Serial.println("accelX: " + String(aX));
    Serial.println("accelY: " + String(aY));
    Serial.println("accelZ: " + String(aZ));
    Serial.println("accelSqrt: " + String(aSqrt));
  } else {
    Serial.println("Cannod read accel values");
  }
*/

  /*if (mySensor.gyroUpdate() == 0) {
    gX = mySensor.gyroX();
    gY = mySensor.gyroY();
    gZ = mySensor.gyroZ();
    Serial.println("gyroX: " + String(gX));
    Serial.println("gyroY: " + String(gY));
    Serial.println("gyroZ: " + String(gZ));
  } else {
    Serial.println("Cannot read gyro values");
  }
*/


  if (mySensor.magUpdate() == 0) {
    mX = mySensor.magX() + X_OFFSET;;
    mY = (mySensor.magY() + Y_OFFSET)* Y_SENSITIVITY_CORRECTION;
    mZ = mySensor.magZ();
    xRav = xRollingAv.updateRav(mX);
    yRav = yRollingAv.updateRav(mY);
    zRav = zRollingAv.updateRav(mZ);

    mDirection = mySensor.magHorizDirection();
    headingRav = headingRollingAv.updateRav(mDirection);
    Serial.print("magX: " + String(mX) + "   " + String(xRav));
    Serial.print("\t  magY: " + String(mY) + "   " + String(yRav));
    Serial.print("\t     magZ: " + String(mZ) + "   " + String(zRav));
    Serial.println("\t Heading: " + String(mDirection) + "\t" +  String(headingRav));
  } else {
    Serial.println("Cannot read mag values");
  }

  //Serial.println("at " + String(millis()) + "ms");
  //Serial.println(""); // Add an empty line
  delay(500);
}
