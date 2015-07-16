###########################################################################
# File:         camera.mak
# Copyright     2009, Research In Motion
# Author:       Bin Lu
# Created:      Mar 3rd, 2009
# Description:  Camera driver makefile
##########################################################################
# -------------------------------------------------------------------------
# Must define the module relative path
MODULE_DIR = drivers/camera

# -------------------------------------------------------------------------
# Camera driver object files
OBJS   = $(MOD_OBJ_DIR)/camif_drvfwk.o	 

# Platform specific object files
OBJS  += $(MOD_OBJ_DIR)/camif_tavorpv2.o
OBJS  += $(MOD_OBJ_DIR)/camif_i2c.o

# ISP specific code
OBJS  += $(MOD_OBJ_DIR)/camif_isp_st987_cut10.o
OBJS  += $(MOD_OBJ_DIR)/camif_isp_st987_cut09.o

# Driver test code
OBJS  += $(MOD_OBJ_DIR)/camif_test.o
OBJS  += $(MOD_OBJ_DIR)/camif_test_st987_cut10.o

VPATH += $(MOD_SRC_DIR)/isp

# Header include path
INC   += -Idrivers/camera/include -Idrivers/include -Idriver/

# -------------------------------------------------------------------------
# Common rules
include build/makefile/common.mak
include build/makefile/rules.mak

# -------------------------------------------------------------------------
# Module specific rules
ifeq ($(PLATFORM), sprucewood)
CFLAGS += -DMCLK_13MHZ
else
CFLAGS += -DMCLK_6_5MHZ
endif