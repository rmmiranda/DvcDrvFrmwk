/*****************************************************************************
 * Filename:    cam_tavorpv2.h
 *
 * Copyright 2009, Research In Motion Ltd
 *
 * Author:      Bin Lu
 *
 * Created:     Mar 9th, 2009
 *
 * Description: This header file defines the PV2 CI and CSI controller 
 *              register map
 *
 *****************************************************************************/
#ifndef __CAM_HS2I2C_H__
#define __CAM_HS2I2C_H__

#include "hnb0_i2c.h"

typedef enum {
    I2C_OP_WRITE,
    I2C_OP_READ,
    I2C_OP_START
} I2cOperation;

typedef struct {
    I2cOperation  op;
    BYTE          *buffer;
    WORD          len;
} I2cDescriptor;

#define I2C_ADDRESS_MASK  0xFFFE

/* High speed HS2I2C */
#define HS2I2C_BASE 0x40400000

#define HS2I2C_ICR_REG32   (HS2I2C_BASE + 0x1690)
#define HS2I2C_ISR_REG32   (HS2I2C_BASE + 0x1698)
#define HS2I2C_ISAR_REG32  (HS2I2C_BASE + 0x16a0)
#define HS2I2C_IDBR_REG32  (HS2I2C_BASE + 0x1688)
#define HS2I2C_IBMR_REG32  (HS2I2C_BASE + 0x1680)
#define HS2I2C_ILCR_REG32  (HS2I2C_BASE + 0x16A8)

#define HS2I2C_ILCR_VALUE  0x082CBB56

#define I2C_ICR_HS         (1<<16) 

void Hs2I2cInit(void);

#endif
