###########################################################################
# File:         <make file name>
# Copyright     2008, Research In Motion
# Author:       
# Created:      July 2008
# Description:  This is a makefile template
##########################################################################
# -------------------------------------------------------------------------
# Must define the module relative path
MODULE_DIR = base/framework

# -------------------------------------------------------------------------
# Module specific variables
OBJS  = $(MOD_OBJ_DIR)/framework.o \
	 $(MOD_OBJ_DIR)/utility_message.o \
	 $(MOD_OBJ_DIR)/utility_debug.o

# -------------------------------------------------------------------------
# Includes
include build/makefile/common.mak
include build/makefile/rules.mak

# -------------------------------------------------------------------------
# Module specific rules
CFLAGS += -DBUILD_USER="\"$(USER)\""
