# MPU9250-calibration
=====================

Based on the MPU9250_asukiaaa  .ino file, which I have modified as follows:

* I added my RollingAverage class so that the raw magnetometer values are accompanied by an adjacent rolling average value (length  set to 20)

* I commented out the bits to do with the gyro and accelerometer for the time being, since my sole focus is currently the magnetometer

* I added symbolic constants as follows (it was overly-optimistic on my part to think I would advance to the point where these might be usable!) 
#define X_OFFSET (0)
#define Y_OFFSET (0)
#define Y_SENSITIVITY_CORRECTION 1
* 
* I reformatted the output so that it is all on one line

