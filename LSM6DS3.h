/*
 * LSM6DS3.h
 *
 *  Created on: Aug 6, 2018
 *      Author: hgray
 */

#ifndef LSM6DS3_H_
#define LSM6DS3_H_


#include "ch.h"

uint8_t beginLSM6DS3(void);

msg_t readRegister(uint8_t *cmd, uint8_t *data);

msg_t writeRegister(uint8_t *data);

uint16_t readAccelX(void);
uint16_t readAccelY(void);
uint16_t readAccelZ(void);
uint16_t readGyroX(void);
uint16_t readGyroY(void);
uint16_t readGyroZ(void);




#define FUNC_CFG_ACCESS         0x01 // Embedded functions configuration register

#define SENSOR_SYNC_TIME_FRAME  0x04 // Sensor sync configuration register


/*
 * FIFO configuration registers
 */
#define FIFO_CTRL1              0x06
#define FIFO_CTRL2              0x07
#define FIFO_CTRL3              0x08
#define FIFO_CTRL4              0x09
#define FIFO_CTRL5              0x0A


#define ORIENT_CFG_G            0x0B

#define INT1_CTRL               0x0D //INT1 pin control

#define INT2_CTRL               0x0E //INT2 pin control

#define WHO_AM_I                0x0F //Who I am ID


/*
 * Accelerometer and gyroscope control registers
 */
#define CTRL1_XL                0x10
#define CTRL2_G                 0x11
#define CTRL3_C                 0x12
#define CTRL4_C                 0x13
#define CTRL5_C                 0x14
#define CTRL6_C                 0x15
#define CTRL7_G                 0x16
#define CTRL8_XL                0x17
#define CTRL9_XL                0x18
#define CTRL10_C                0x19

#define MASTER_CONFIG 0x1A // I2C master configuration register

/*
 * Interrupts registers
 */
#define WAKE_UP_SRC             0x1B
#define TAP_SRC                 0x1C
#define D6D_SRC                 0x1D

#define STATUS_REG              0x1E // Status data register

/*
 * Temperature output data register
 */
#define OUT_TEMP_L              0x20
#define OUT_TEMP_H              0x21


/*
 * Gyroscope output register
 */
#define OUTX_L_G                0x22
#define OUTX_H_G                0x23
#define OUTY_L_G                0x24
#define OUTY_H_G                0x25
#define OUTZ_L_G                0x26
#define OUTZ_H_G                0x27


/*
 * Accelerometer output register
 */
#define OUTX_L_XL               0x28
#define OUTX_H_XL               0x29
#define OUTY_L_XL               0x2A
#define OUTY_H_XL               0x2B
#define OUTZ_L_XL               0x2C
#define OUTZ_H_XL               0x2D


/*
 * Sensor hub output registers
 */
#define SENSORHUB1_REG          0x2E
#define SENSORHUB2_REG          0x2F
#define SENSORHUB3_REG          0x30
#define SENSORHUB4_REG          0x31
#define SENSORHUB5_REG          0x32
#define SENSORHUB6_REG          0x33
#define SENSORHUB7_REG          0x34
#define SENSORHUB8_REG          0x35
#define SENSORHUB9_REG          0x36
#define SENSORHUB10_REG         0x37
#define SENSORHUB11_REG         0x38
#define SENSORHUB12_REG         0x39


/*
 * FIFO status registers
 */
#define FIFO_STATUS1            0x3A
#define FIFO_STATUS2            0x3B
#define FIFO_STATUS3            0x3C
#define FIFO_STATUS4            0x3D


/*
 * FIFO data output registers
 */
#define FIFO_DATA_OUT_L         0x3E
#define FIFO_DATA_OUT_H         0x3F


/*
 * Timestamp output registers
 */
#define TIMESTAMP0_REG          0x40
#define TIMESTAMP1_REG          0x41
#define TIMESTAMP2_REG          0x42


/*
 * Step counter timestamp registers
 */
#define STEP_TIMESTAMP_L        0x49
#define STEP_TIMESTAMP_H        0x4A


/*
 * Step counter output registers
 */
#define STEP_COUNTER_L          0x4B
#define STEP_COUNTER_H          0x4C


/*
 * Sensor hub output registers
 */
#define SENSORHUB13_REG         0x4D
#define SENSORHUB14_REG         0x4E
#define SENSORHUB15_REG         0x4F
#define SENSORHUB16_REG         0x50
#define SENSORHUB17_REG         0x51
#define SENSORHUB18_REG         0x52


/*
 * Interrupt registers
 */
#define FUNC_SRC                0x53
#define TAP_CFG                 0x58
#define TAP_THS_6D              0x59
#define INT_DUR2                0x5A
#define WAKE_UP_THS             0x5B
#define WAKE_UP_DUR             0x5C
#define FREE_FALL               0x5D
#define MD1_CFG                 0x5E
#define MD2_CFG                 0x5F
#define OUT_MAG_RAW_X_L         0x66


/*
 * External magnetometer raw data output registers
 */
#define OUT_MAG_RAW_X_H         0x67
#define OUT_MAG_RAW_Y_L         0x68
#define OUT_MAG_RAW_Y_H         0x69
#define OUT_MAG_RAW_Z_L         0x6A
#define OUT_MAG_RAW_Z_H         0x6B

/*******************************************************************************
* Register      : CTRL1_XL
* Address       : 0X10
* Bit Group Name: BW_XL
* Permission    : RW
*******************************************************************************/
typedef enum {
    CTRL1_XL_BW_400Hz         = 0x00,
    CTRL1_XL_BW_200Hz         = 0x01,
    CTRL1_XL_BW_100Hz         = 0x02,
    CTRL1_XL_BW_50Hz          = 0x03,
} CTRL1_XL_BW_t;

/*******************************************************************************
* Register      : CTRL1_XL
* Address       : 0X10
* Bit Group Name: FS_XL
* Permission    : RW
*******************************************************************************/
typedef enum {
    CTRL1_XL_FS_2g            = 0x00,
    CTRL1_XL_FS_16g           = 0x04,
    CTRL1_XL_FS_4g            = 0x08,
    CTRL1_XL_FS_8g            = 0x0C,
} CTRL1_XL_FS_t;

/*******************************************************************************
* Register      : CTRL1_XL
* Address       : 0X10
* Bit Group Name: ODR_XL
* Permission    : RW
*******************************************************************************/
typedef enum {
    CTRL1_XL_ODR_POWER_DOWN   = 0x00,
    CTRL1_XL_ODR_13Hz         = 0x10,
    CTRL1_XL_ODR_26Hz         = 0x20,
    CTRL1_XL_ODR_52Hz         = 0x30,
    CTRL1_XL_ODR_104Hz        = 0x40,
    CTRL1_XL_ODR_208Hz        = 0x50,
    CTRL1_XL_ODR_416Hz        = 0x60,
    CTRL1_XL_ODR_833Hz        = 0x70,
    CTRL1_XL_ODR_1660Hz       = 0x80,
    CTRL1_XL_ODR_3330Hz       = 0x90,
    CTRL1_XL_ODR_6660Hz       = 0xA0,
    CTRL1_XL_ODR_13330Hz      = 0xB0,
} CTRL1_XL_ODR_t;


#endif /* LSM6DS3_H_ */
