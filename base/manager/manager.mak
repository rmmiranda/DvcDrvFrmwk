###########################################################################
# File:         manager.mak
# Copyright     2008, Research In Motion
# Author:       
# Created:      July 2008
# Description:  makefile for the platform manager module
##########################################################################
# -------------------------------------------------------------------------
# Must define the module relative path
MODULE_DIR = base/manager

# -------------------------------------------------------------------------
# Module specific variables
OBJS = 	$(MOD_OBJ_DIR)/mgr_main.o

# -------------------------------------------------------------------------
# Includes
include build/makefile/common.mak
include build/makefile/rules.mak

# -------------------------------------------------------------------------
# Module specific rules
