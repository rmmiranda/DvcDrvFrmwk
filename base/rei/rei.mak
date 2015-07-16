###########################################################################
# File:         rei.mak
# Copyright     2010, Research In Motion
# Author:       Daniel Hadi
# Created:      Jan 2010
# Description:  makefile for the platform rei module
##########################################################################
# -------------------------------------------------------------------------
# Must define the module relative path
MODULE_DIR = base/rei

# -------------------------------------------------------------------------
# Module specific variables
OBJS  = $(MOD_OBJ_DIR)/rei.o
OBJS += $(MOD_OBJ_DIR)/rei_minios_gpio.o
OBJS += $(MOD_OBJ_DIR)/rei_minios_clocks.o

INC   += -Ibase/rei/include


# -------------------------------------------------------------------------
# Includes
include build/makefile/common.mak
include build/makefile/rules.mak

# -------------------------------------------------------------------------
# Module specific rules
