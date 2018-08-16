/*
 * LSM6DS3.c
 *
 *  Created on: Aug 6, 2018
 *      Author: hgray
 */

#include "LSM6DS3.h"
#include "hal.h"

#define I2C_ADDR 0b1101010 // IF SA0 is connected to GND
//#define I2C_ADDR 0b1101011 // IF SA0 is connected to VCC


uint8_t beginLSM6DS3()
{
  msg_t msg;
  uint8_t CTRL1_XL_SETUP = CTRL1_XL_ODR_104Hz \
                         | CTRL1_XL_FS_2g     \
                         | CTRL1_XL_BW_200Hz;

  uint8_t cmd[] = {CTRL1_XL, CTRL1_XL_SETUP};

  msg = writeRegister(cmd, sizeof(cmd));
  if (msg != MSG_OK)
    return -1;

  return 0;
}

msg_t readRegister(uint8_t *cmd, uint8_t *data, size_t dataLen)
{
  msg_t msg;
  msg = i2cMasterTransmitTimeout(&I2CD1, I2C_ADDR, cmd, 1, data, dataLen, 10000);

  return msg;
}

msg_t writeRegister(uint8_t *cmd, size_t cmdLen)
{
  msg_t msg;
  msg = i2cMasterTransmitTimeout(&I2CD1, I2C_ADDR, cmd, cmdLen, NULL, 0, 10000);

  return msg;
}

uint16_t readAccelX(void)
{
  uint8_t lowData[] = {0};
  uint8_t highData[] = {0};
  uint16_t accelX;

  uint8_t cmd[] = {OUTX_L_XL};
  readRegister(cmd, lowData, sizeof(lowData));

  cmd[0] = OUTX_H_XL;
  readRegister(cmd, highData, sizeof(highData));

  accelX = highData[0]<<8 | lowData[0];
  return accelX;
}


uint16_t readAccelY(void)
{
  uint8_t lowData[] = {0};
  uint8_t highData[] = {0};
  uint16_t accelY;

  uint8_t cmd[] = {OUTY_L_XL};
  readRegister(cmd, lowData, sizeof(lowData));

  cmd[0] = OUTY_H_XL;
  readRegister(cmd, highData, sizeof(highData));

  accelY = highData[0]<<8 | lowData[0];
  return accelY;
}


uint16_t readAccelZ(void)
{
  uint8_t lowData[] = {0};
  uint8_t highData[] = {0};
  uint16_t accelZ;

  uint8_t cmd[] = {OUTZ_L_XL};
  readRegister(cmd, lowData, sizeof(lowData));

  cmd[0] = OUTZ_H_XL;
  readRegister(cmd, highData, sizeof(highData));

  accelZ = highData[0]<<8 | lowData[0];
  return accelZ;
}


uint16_t readGyroX(void)
{
  uint8_t lowData[] = {0};
  uint8_t highData[] = {0};
  uint16_t gyroX;

  uint8_t cmd[] = {OUTX_L_G};
  readRegister(cmd, lowData, sizeof(lowData));

  cmd[0] = OUTX_H_G;
  readRegister(cmd, highData, sizeof(highData));

  gyroX = highData[0]<<8 | lowData[0];
  return gyroX;
}


uint16_t readGyroY(void)
{
  uint8_t lowData[] = {0};
  uint8_t highData[] = {0};
  uint16_t gyroY;

  uint8_t cmd[] = {OUTY_L_G};
  readRegister(cmd, lowData, sizeof(lowData));

  cmd[0] = OUTY_H_G;
  readRegister(cmd, highData, sizeof(highData));

  gyroY = highData[0]<<8 | lowData[0];
  return gyroY;
}


uint16_t readGyroZ(void)
{
  uint8_t lowData[] = {0};
  uint8_t highData[] = {0};
  uint16_t gyroZ;

  uint8_t cmd[] = {OUTZ_L_G};
  readRegister(cmd, lowData, sizeof(lowData));

  cmd[0] = OUTZ_H_G;
  readRegister(cmd, highData, sizeof(highData));

  gyroZ = highData[0]<<8 | lowData[0];
  return gyroZ;
}

