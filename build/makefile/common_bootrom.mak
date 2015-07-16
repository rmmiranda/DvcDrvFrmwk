###########################################################################
# File:         common.mak
# Copyright     2008, Research In Motion
# Author:       
# Created:      July 2008
# Description:  This makefile defines all the common variables and rules
##########################################################################
# -------------------------------------------------------------------------
# include
# arm compiler tools
include $(TOOLSDIR)/arm_armrvct22.mak

ELFAR	:= $(TOOLSDIR)/msa33/elfar.exe

# -------------------------------------------------------------------------
#CFlags for .c -> .o files
CFLAGS += \
 --cpu xscale \
 --diag_remark 68 \
 --diag_remark 111 \
 --diag_suppress 1300 \
 --diag_warning 172 \
 --diag_error 193 \
 --apcs /noswst/interwork \
 --littleend \
 $(BLD_FLAGS) \
 $(DEBUGCFLAGS) \
 -O2 \
 $(DEFINES) \
 $(USERCFLAGS) 

# -------------------------------------------------------------------------
#Include folders for .c or .s -> .o files
INC += \
 -I$(ARMOS_ROOT_DIR)/common/include \
 -I$(ARMOS_ROOT_DIR)/common/firmware/include \
 -I$(ARMOS_ROOT_DIR)/common/inc_pub \
 -I$(ARMOS_ROOT_DIR)/common/tools/rv22/include \
 -I$(ARMOS_ROOT_DIR)/common/config/$(PLATFORM)/

# -------------------------------------------------------------------------
# from $(TOOLSDIR)/arm_armdefault.mak
OBJCONV_XDB_FILES = $(TOOLSDIR)/msa33/dwarf2bd.exe -hx $(TARGET_ELF)

