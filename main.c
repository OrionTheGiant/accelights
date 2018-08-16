/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"
//#include "rt_test_root.h"
//#include "oslib_test_root.h"
#include "LSM6DS3.h"
#include "NeoPixels.h"
#include "chprintf.h"
#include <math.h>

/*
 * Blue LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker1");
  while (true) {
    palClearPad(GPIOC, GPIOC_LED4);
    chThdSleepMilliseconds(500);
    palSetPad(GPIOC, GPIOC_LED4);
    chThdSleepMilliseconds(500);
  }
}

/*
 * Green LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread2, 128);
static THD_FUNCTION(Thread2, arg) {

  (void)arg;
  chRegSetThreadName("blinker2");
  while (true) {
    palClearPad(GPIOC, GPIOC_LED3);
    chThdSleepMilliseconds(250);
    palSetPad(GPIOC, GPIOC_LED3);
    chThdSleepMilliseconds(250);
  }
}

/*
 * IMU thread
 */
static THD_WORKING_AREA(waThreadIMU,128);
static THD_FUNCTION(ThreadIMU,arg)
{
  (void)arg;
  chRegSetThreadName("IMU");
  uint16_t waitTime;
  float Gx,Gy,Gz; // Gravity acceleration vectors
  float pitch, roll; // Pitch and roll calculated from Gx, Gy, Gz
  float radius, theta; // Parameters of polar coordinates (radius is value in HSV, theta is hue, saturation is 1)
  hsv HSV;
  rgb RGB;
  uint16_t accelXCounts;
  uint16_t accelYCounts;
  uint16_t accelZCounts;
  beginLSM6DS3();

  while (true)
  {
    // Read acceleration values
    accelXCounts = readAccelX();
    accelYCounts = readAccelY();
    accelZCounts = readAccelZ();

    // Convert from 2s complement
      //If first bit is 1, invert all bits, add 1, multiply by -1
    if (accelXCounts & 1<<15)
      accelXCounts = ~accelXCounts + 1;
    if (accelYCounts & 1<<15)
      accelYCounts = ~accelYCounts + 1;
    if (accelZCounts & 1<<15)
      accelZCounts = ~accelZCounts + 1;

    // Math to get g value
      // Count value should be in the range 0->32767 (taking absolute value of accelZCounts)
      // Divide 32768 by accelerometer sensitivity value (e.g. 2, 4, 8, 16)
      // Divide count value by sensitivity quotient
    Gx = accelXCounts/(32768.0/2);
    Gy = accelYCounts/(32768.0/2);
    Gz = accelZCounts/(32768.0/2);

    pitch = (atan2(Gy,(sqrt(Gx*Gx+Gy*Gy)))*180)/M_PI; // Will be in the range [-90,90]
    roll = (atan2(-Gx,Gz)*180)/M_PI; // Will be in the range [-180,180]

    pitch /= 90; // Normalize to [-1,1]
    roll /= 180; // Normalize to [-1,1]

    radius = sqrt(pitch*pitch+roll*roll);
    if (radius > 1)
      radius = 1; // Working with the unit circle so max radius is 1

    theta = atan2(pitch,roll);

    HSV.h = theta;
    HSV.s = 1;
    HSV.v = radius;

    RGB = hsv2rgb(HSV);

    neoPixelStrandColor(RGB);
    // Convert g value to thread wait time
    waitTime = Gz*200;


    // Toggle LED
    palTogglePad(GPIOC, GPIOC_LED4);




    //setNeoPixels(R,G,B);


    // Thread sleep
    //chprintf((BaseSequentialStream *)&SD1, "W:%3.3u", waitTime);

    chThdSleepMilliseconds(waitTime);


    //msg = i2cMasterTransmitTimeout(&I2CD1, 0b1101010, cmd, sizeof(cmd), data, sizeof(data), 10000);
    //sdWrite(&SD1, data, sizeof(data));
    /*if (msg != MSG_OK){
      palTogglePad(GPIOC, GPIOC_LED4);
      chThdSleepMilliseconds(50);
      palTogglePad(GPIOC, GPIOC_LED4);
      chThdSleepMilliseconds(50);
      palTogglePad(GPIOC, GPIOC_LED4);
      chThdSleepMilliseconds(50);
      palTogglePad(GPIOC, GPIOC_LED4);
      chThdSleepMilliseconds(50);

      sdWrite(&SD1, "Transmit Failure\r\n", 8);
    }*/
  }
}

/*
 * I2C1 config.
 * Timing values are taken from the RM except the PRESC set to 9 because
 * the input clock is 72MHz.
 * The timings are critical, please always refer to the STM32 Reference
 * Manual before attempting changes.
 */
static const I2CConfig i2cconfig = {
  STM32_TIMINGR_PRESC(15U) |
  STM32_TIMINGR_SCLDEL(4U) | STM32_TIMINGR_SDADEL(2U) |
  STM32_TIMINGR_SCLH(15U)  | STM32_TIMINGR_SCLL(21U),
  0,
  0
};



/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();



  /*
   * Activate the I2C driver 1 and set I2C1 pins to alternate mode 1
   */
  i2cStart(&I2CD1, &i2cconfig);
  palSetPadMode(GPIOB, 6, PAL_MODE_ALTERNATE(1) | PAL_STM32_OTYPE_OPENDRAIN); /* I2C1 SCL */
  palSetPadMode(GPIOB, 7, PAL_MODE_ALTERNATE(1) | PAL_STM32_OTYPE_OPENDRAIN); /* I2C1 SDA */


  /*
   * Activates the serial driver 1 using the driver default configuration.
   * PA9 and PA10 are routed to USART1.
   */
  sdStart(&SD1, NULL);
  palSetPadMode(GPIOA, 9, PAL_MODE_ALTERNATE(1));  /* USART1 TX. */
  palSetPadMode(GPIOA, 10, PAL_MODE_ALTERNATE(1)); /* USART1 RX. */

  /*
   * Creates the blinker threads.
   */
  //chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO-1, Thread1, NULL);
  //chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Thread2, NULL);


  /*
   * Creates the IMU thread.
   */
  chThdCreateStatic(waThreadIMU, sizeof(waThreadIMU), NORMALPRIO, ThreadIMU, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state, when the button is
   * pressed the test procedure is launched with output on the serial
   * driver 1.
   */
  while (true)
  {

    //unsigned i;
    //msg_t msg;
    //static const uint8_t cmd[] = {0, 0};
    //uint8_t data[16];

    //if (palReadPad(GPIOA, GPIOA_BUTTON))
    //{
      //test_execute((BaseSequentialStream *)&SD1, &rt_test_suite);
      //test_execute((BaseSequentialStream *)&SD1, &oslib_test_suite);
      //chprintf((BaseSequentialStream *)&SD1, "ABC\r\n");
      //msg = i2cMasterTransmitTimeout(&I2CD1, 0x52, cmd, sizeof(cmd), data, sizeof(data), TIME_INFINITE);
      //palTogglePad(GPIOC, GPIOC_LED3);
    //}
    //if (msg != MSG_OK)
      //palTogglePad(GPIOC, GPIOC_LED3);
    //for (i = 0; i < 256; i++)
    //{
      //msg = i2cMasterReceiveTimeout(&I2CD1, 0x52, data, sizeof(data), TIME_INFINITE);
      //if (msg != MSG_OK)
        //palTogglePad(GPIOC, GPIOC_LED3);
    //}
    palTogglePad(GPIOC, GPIOC_LED3);
    chThdSleepMilliseconds(1000);
  }
  //return 0;
}
