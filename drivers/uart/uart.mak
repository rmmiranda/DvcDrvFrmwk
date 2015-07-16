###########################################################################
# File:         <make file name>
# Copyright     2008, Research In Motion
# Author:       
# Created:      July 2008
# Description:  This is a makefile template
##########################################################################
# -------------------------------------------------------------------------
# Must define the module relative path
MODULE_DIR = drivers/uart

# -------------------------------------------------------------------------
# Module specific variables
OBJS   = $(MOD_OBJ_DIR)/uart.o

INC   += -Idrivers/uart/include -Idrivers/include -Idriver/

# -------------------------------------------------------------------------
# Common rules
include build/makefile/common.mak
include build/makefile/rules.mak

# -------------------------------------------------------------------------
# Module specific rules
