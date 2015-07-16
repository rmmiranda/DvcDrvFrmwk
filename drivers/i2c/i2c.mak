###########################################################################
# File:         i2c.mak
# Copyright     2010, Research In Motion
# Author:       Mikhail Zakharov
# Created:      March 2009
# Description:  makefile for the I2C driver
##########################################################################
# -------------------------------------------------------------------------
# Must define the module relative path
MODULE_DIR = drivers/i2c

# -------------------------------------------------------------------------
# Module specific variables
OBJS   = $(MOD_OBJ_DIR)/i2c.o \
	 $(MOD_OBJ_DIR)/i2c_drvfwk.o

# -------------------------------------------------------------------------
# Common rules
include build/makefile/common.mak
include build/makefile/rules.mak

# -------------------------------------------------------------------------
# Module specific rules
