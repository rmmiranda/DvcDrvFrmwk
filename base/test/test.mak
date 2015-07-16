###########################################################################
# File:         test.mak
# Copyright     2008, Research In Motion
# Author:       
# Created:      July 2008
# Description:  makefile for the platform test module
##########################################################################
# -------------------------------------------------------------------------
# Must define the module relative path
MODULE_DIR = base/test

# -------------------------------------------------------------------------
# Module specific variables
OBJS = 	$(MOD_OBJ_DIR)/gpio_test.o
OBJS += $(MOD_OBJ_DIR)/clk_test.o

# -------------------------------------------------------------------------
# Includes
include build/makefile/common.mak
include build/makefile/rules.mak

# -------------------------------------------------------------------------
# Module specific rules
