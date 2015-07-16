###########################################################################
# File:         common.mak
# Copyright     2008, Research In Motion
# Author:       
# Created:      July 2008
# Description:  This makefile defines all the common variables and rules
##########################################################################

# -------------------------------------------------------------------------
# Variables
# Important Directories
ROOT_DIR = ..
PLATFORM_ROOT_DIR = $(ROOT_DIR)/platformSW
ARMOS_ROOT_DIR 	  = $(ROOT_DIR)/armos
TOOLSDIR = $(ARMOS_ROOT_DIR)/common/tools
PLATFORM_REL_DIR  = $(PLATFORM_ROOT_DIR)/build/release/$(PLATFORM)_$(BUILD_TGT)
PLATFORM_LIB_DIR  = $(PLATFORM_ROOT_DIR)/build/lib/$(PLATFORM)_$(BUILDLIB)
PLATFORM_TGT_CONF = $(PLATFORM_ROOT_DIR)/build/config/$(PLATFORM).conf
SHELL := $(TOOLSDIR)/sh.exe

# Module related directories
MODULE_NAME = $(subst /,_,$(MODULE_DIR))
MOD_OBJ_DIR = $(MODULE_DIR)/obj/$(PLATFORM)_$(BUILD_TGT)
MOD_SRC_DIR = $(MODULE_DIR)/src
MOD_TST_DIR = $(MODULE_DIR)/test

# -------------------------------------------------------------------------
# from the main makefile
SED      = $(TOOLSDIR)/sed.exe
TOASM    = $(TOOLSDIR)/toASM.sed
GREP     = $(TOOLSDIR)/grep.exe
H2INC    = $(TOOLSDIR)/H2INC.exe
HTOINC   = $(TOOLSDIR)/h_to_inc.exe #our version
HTOMAK   = $(TOOLSDIR)/h_to_mak2.exe
CP       = $(TOOLSDIR)/cp.exe
MV       = $(TOOLSDIR)/mv.exe
DXEPARSE = $(TOOLSDIR)/dxeparse.exe
ELF2BIN  = $(TOOLSDIR)/elf2bin.exe
CHECKBIN = $(TOOLSDIR)/checkbinsize.pl
PL       = $(TOOLSDIR)/perl510.exe -I $(TOOLSDIR)/perl510/lib
CD       = cd
SET      = set
RM       = $(TOOLSDIR)/rm.exe
PRINT    = $(TOOLSDIR)/echo.exe
MOVE     = $(TOOLSDIR)/mv.exe
MKDIR    = $(TOOLSDIR)/mkdir.exe -p
TR       = $(TOOLSDIR)/tr.exe
XARGS    = $(TOOLSDIR)/xargs.exe
CAT      = $(TOOLSDIR)/cat.exe
PASTE    = $(TOOLSDIR)/paste.exe
MAKEDEP  = $(TOOLSDIR)/makedepend.exe
CHMOD    = $(TOOLSDIR)/chmod.exe
BRSIGN   = $(TOOLSDIR)/brsign.exe
FSZ      = $(TOOLSDIR)/fsz.exe
BRL      = $(TOOLSDIR)/brl.exe
TEST     = test
THREADADJUST = $(ARMOS_ROOT_DIR)/nessus/tools/threadadjust.pl
THREADGEN = $(ARMOS_ROOT_DIR)/nessus/tools/threadgen.pl
GPIOGEN  = $(TOOLSDIR)/python24/python.exe $(TOOLSDIR)/gpiogen.py
PYTHON   = $(TOOLSDIR)/python24/python.exe
FIND     = $(TOOLSDIR)/find.exe
BUGBUILD = $(TOOLSDIR)/BugBuild.exe

# -------------------------------------------------------------------------
# Paths
VPATH += $(MOD_SRC_DIR) $(MOD_TST_DIR) 

INC += \
    -Ibase/arch/$(ARCH)/\
    -Ibase/arch/$(ARCH)/$(PLATFORM)/\
	-I$(MODULE_DIR)/include/\
	-Ibase/include/\
    -Ibase/test/include/\
	-Idrivers/include/\
    -Ibase/\
	-Iapps/\
	-I$(PLATFORM_REL_DIR)\
	-I$(PLATFORM_LIB_DIR)/include/

# -------------------------------------------------------------------------
# include
# Include the target specific makefile
ifeq "$(strip $(BUILD_TGT))" "bootrom"
include build/makefile/common_bootrom.mak
else
include build/makefile/common_nessus.mak
endif
