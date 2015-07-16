###########################################################################
# File:         display.mak
# Copyright     2009, Research In Motion
# Author:       Victor Chen
# Created:      April 13, 2009
# Description:  display driver makefile
##########################################################################
# -------------------------------------------------------------------------
# Must define the module relative path
MODULE_DIR = drivers/display

# -------------------------------------------------------------------------
# Display driver object files
OBJS   = $(MOD_OBJ_DIR)/dispif_drvfwk.o

# Platform specific object files
OBJS  += $(MOD_OBJ_DIR)/display.o \
         $(MOD_OBJ_DIR)/dsi.o \
         $(MOD_OBJ_DIR)/hdmi.o \
         $(MOD_OBJ_DIR)/ddc.o \
         $(MOD_OBJ_DIR)/parallel.o         

# Panel specific object files - initialization sequence for DriverIC
OBJS  += $(MOD_OBJ_DIR)/svp_panels.o \
         $(MOD_OBJ_DIR)/svp_lcd_optisTmd.o \
         $(MOD_OBJ_DIR)/svp_lcd_mirage_sharp.o 

# Driver test code
OBJS  += $(MOD_OBJ_DIR)/display_test.o

VPATH += $(MOD_SRC_DIR)/dss \
         $(MOD_SRC_DIR)/panels \
         $(MOD_SRC_DIR)/test 

# Header include path
INC   += -Idrivers/display/include -Idrivers/include -Idriver/ 

# -------------------------------------------------------------------------
# Common rules
include build/makefile/common.mak
include build/makefile/rules.mak

# -------------------------------------------------------------------------
# Module specific rules

