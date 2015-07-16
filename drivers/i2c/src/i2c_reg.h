/*****************************************************************************
 * Filename:    i2c_reg.h
 *
 * Copyright 2010, Research In Motion Ltd
 *
 * Author:      Bin Lu
 *
 * Created:     Mar 4th, 2010
 *
 * Description: This header file defines the I2C register map and Macros
 *
 *****************************************************************************/
 
#ifndef __I2C_REG_H__
#define __I2C_REG_H__

/* ---------------------------------------------------------------------------
 * Register definitions for CSI controller 
 * --------------------------------------------------------------------------- */
#define I2C_DEV_MAX	 6

// I2C base addresses
#define TWSI_I2C_1   0xD4011000
#define TWSI_I2C_2   0xD4031000
#define TWSI_I2C_3   0xD4032000
#define TWSI_I2C_4   0xD4033000
#define TWSI_I2C_5   0xD4033800
#define TWSI_I2C_6   0xD4034000

#define TWSI_I2C_BASE(x)	(((x) == 0) ? TWSI_I2C_1 : (\
                             ((x) == 1) ? TWSI_I2C_2 : (\
                             ((x) == 2) ? TWSI_I2C_3 : (\
                             ((x) == 3) ? TWSI_I2C_4 : (\
                             ((x) == 4) ? TWSI_I2C_5 : TWSI_I2C_6 )))))

#define TWSI_I2C_CLOCK(x)	(((x) == 0) ? CLOCK_TWSI1 : (\
                             ((x) == 1) ? CLOCK_TWSI2 : (\
                             ((x) == 2) ? CLOCK_TWSI3 : (\
                             ((x) == 3) ? CLOCK_TWSI4 : (\
                             ((x) == 4) ? CLOCK_TWSI5 : CLOCK_TWSI6 )))))

// I2C registers
#define TWSI_DBR(I2C_BASE)  (I2C_BASE + 0x0008)
#define TWSI_CR(I2C_BASE)   (I2C_BASE + 0x0010)
#define TWSI_SR(I2C_BASE)   (I2C_BASE + 0x0018)
#define TWSI_SAR(I2C_BASE)  (I2C_BASE + 0x0020)
#define TWSI_LCR(I2C_BASE)  (I2C_BASE + 0x0028)

// I2C register bitmasks
#define TWSI_CR_FM      (1<<15)
#define TWSI_CR_UR      (1<<14)
#define TWSI_CR_IUE     (1<<6)
#define TWSI_CR_SCLEA   (1<<5)
#define TWSI_CR_MA      (1<<4)
#define TWSI_CR_TB      (1<<3)
#define TWSI_CR_ACKNACK (1<<2)
#define TWSI_CR_STOP    (1<<1)
#define TWSI_CR_START   (1<<0)

#define TWSI_SR_BED     (1<<10)
#define TWSI_SR_ITE     (1<<6)
#define TWSI_SR_UB      (1<<2)

#define ILCR_DEFAULT    (0x082c1da1)

//#define TWSI_CR_ENABLE (  TWSI_CR_FM | TWSI_CR_IUE | TWSI_CR_SCLEA )
#define TWSI_CR_ENABLE (  TWSI_CR_IUE | TWSI_CR_SCLEA )

#define TWSI_SR_CLEAR	0x0003F7F0

#define I2C_ADDRESS_MASK  0xFFFE

// I2C pad configuration
#define I2C_PAD_CONFIG \
	{ TWSI1_SCL, TWSI1_SDA, TWSI1_SCL_FN, TWSI1_SDA_FN },\
	{ TWSI2_SCL, TWSI2_SDA, TWSI2_SCL_FN, TWSI2_SDA_FN },\
	{ TWSI3_SCL, TWSI3_SDA, TWSI3_SCL_FN, TWSI3_SDA_FN },\
	{ TWSI4_SCL, TWSI4_SDA, TWSI4_SCL_FN, TWSI4_SDA_FN },\
	{ TWSI5_SCL, TWSI5_SDA, TWSI5_SCL_FN, TWSI5_SDA_FN },\
	{ TWSI6_SCL, TWSI6_SDA, TWSI6_SCL_FN, TWSI6_SDA_FN }

#endif
