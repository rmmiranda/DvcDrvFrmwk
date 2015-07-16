###########################################################################
# File:         common.mak
# Copyright     2008, Research In Motion
# Author:       
# Created:      July 2008
# Description:  This makefile defines all the common variables and rules
##########################################################################

# -------------------------------------------------------------------------
# Common rules
  # General rule template for building .c files
BUGBUILD = $(TOOLSDIR)/bugbuild.exe
FIND = $(TOOLSDIR)/find.exe
SHELL := $(TOOLSDIR)/sh.exe
  
MAKEFILEPATH = $(PLATFORM_ROOT_DIR)/build/makefile
MAKEFILES = $(MAKEFILEPATH)/rules.mak $(MAKEFILEPATH)/common.mak $(MAKEFILEPATH)/platform.mak

$(MOD_OBJ_DIR)/%.o: %.c $(MAKEFILES)
	$(PRINT) Compiling file [$@] ...
	$(TEST) -d $(MOD_OBJ_DIR) || $(MKDIR) $(MOD_OBJ_DIR)
ifeq ($(DEBUG), _debug)
	$(CC16) -E $(CFLAGS) $(OPTIMISATION) $(INC) -c -o $(@:.o=.pp) $<
endif
ifeq ($(BUILD_TGT), nessus)
	$(BUGBUILD) $(BUGBUILD_FLAGS) /f:CORE_ARM $< > $(@:.o=.bugx)
endif
ifeq ($(BUILD_TGT), minios)
	$(BUGBUILD) $(BUGBUILD_FLAGS) /f:CORE_ARM $< > $(@:.o=.bugx)
endif
	$(CC16) $(CFLAGS) $(OPTIMISATION) $(INC) -c -o $@ $<

$(MOD_OBJ_DIR)/$(MODULE_NAME).lib : $(OBJS) 
	$(PRINT) Building module [$(@)] ...
	$(PRINT) $(LIB) -r $@ $^
	$(LIB) -r $@ $^
	$(PRINT) ========================================================================

#Include the header dependencies.
include makefile.dep