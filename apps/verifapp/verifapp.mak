###########################################################################
# File:         verifapp.mak
# Copyright     2009, Research In Motion
# Author:       Bin Lu
# Created:      Mar 3rd, 2009
# Description:  test app makefile
##########################################################################
# -------------------------------------------------------------------------
# Must define the module relative path
MODULE_DIR = apps/verifapp

# -------------------------------------------------------------------------
# Camera driver object files
OBJS = $(MOD_OBJ_DIR)/verifMain.o

ifeq ($(DRV_CAMERA),drivers/camera)
OBJS += $(MOD_OBJ_DIR)/camTest.o
endif

ifeq ($(DRV_DISPLAY),drivers/display)
OBJS +=  $(MOD_OBJ_DIR)/disTest.o
endif

# Header include path
INC   += -Idrivers/include

# -------------------------------------------------------------------------
# Common rules
include build/makefile/common.mak
include build/makefile/rules.mak

# -------------------------------------------------------------------------
# Module specific rules
