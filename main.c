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
#include "rt_test_root.h"
#include "oslib_test_root.h"
#include "LSM6DS3.h"
#include "chprintf.h"

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
  int g;
  uint16_t accelXCounts;
  uint16_t accelYCounts;
  uint16_t accelZCounts;
  beginLSM6DS3();

  while (true)
  {
    //if (palReadPad(GPIOA, GPIOA_BUTTON))
    //{
      // Read acceleration values
      accelZCounts = readAccelZ();

      // Convert from 2s complement
        //If first bit is 1, invert all bits, add 1, multiply by -1
      if (accelZCounts & 1<<15)
        accelZCounts = ~accelZCounts + 1;

      // Math to get g value
        // Count value should be in the range 0->32767 (taking absolute value of accelZCounts)
        // Divide 32768 by accelerometer sensitivity value (e.g. 2, 4, 8, 16)
        // Divide count value by sensitivity quotient
      g = accelZCounts/(32768/2);

      // Convert g value to thread wait time
      waitTime = g*100;


      // Toggle LED
      palTogglePad(GPIOC, GPIOC_LED4);

      // Thread sleep
      //sdWrite(&SD1, waitTime, sizeof(waitTime));
      chprintf((BaseSequentialStream *)&SD1, "Wait Time is:  %u", waitTime);

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
    //}
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
  return 0;
}
