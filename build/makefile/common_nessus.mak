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
include $(TOOLSDIR)/arm_armrvct31.mak

# -------------------------------------------------------------------------
#CFlags for .c -> .o files
CFLAGS += \
 -DRIM_TOOLSET_ARM_ARMRVCT31=1 \
 --apcs /noswst/interwork \
 --via $(PLATFORM_ROOT_DIR)/build/config/$(PLATFORM)_defines_c.txt \
 -D_AEABI_PORTABILITY_LEVEL=0 \
 -D__DEFAULT_AEABI_PORTABILITY_LEVEL=0 \
 $(BLD_FLAGS)\
 --fpmode ieee_no_fenv \
 --enum_is_int \
 --diag_remark 68 \
 --diag_remark 111 \
 --diag_suppress 1300 \
 --diag_warning 172 \
--diag_error 193 \
 $(DEBUGCFLAGS) \
 -O2 \
 $(DEFINES) \
 $(USERCFLAGS)

ARM_LINK_FLAG =\
  --noremove \
  --info sizes,totals,veneers \
  --symbols \
  --map \
  --list $(PLATFORM_REL_DIR)/nessus.listing \
  --symdefs $(PLATFORM_REL_DIR)/nessus.sym \
  --diag_suppress 6314  \
  --diag_suppress 6304  \
  --diag_suppress 6385  \
  --diag_suppress 6632

ARM_LINK_FLAG += --scatter $(PLATFORM_LIB_DIR)/$(SCATTER_FILE)

# Caprock 
ifeq ($(PLATFORM), caprock)
CFLAGS += \
 --cpu PXA270 \
 --fpu SoftVFP 
 
 SCATTER_FILE = tavor_xscale.sct
 EXTRA_LINK_FILE = $(PLATFORM_LIB_DIR)/nessus_hifi2.o
 EXTRA_LINK_LIB = $(PLATFORM_LIB_DIR)/codecAACDEC_WMMX.lib \
   $(PLATFORM_LIB_DIR)/codecMP3Dec_WMMX.lib \
   $(PLATFORM_LIB_DIR)/ippIP_WMMX.lib \
   $(PLATFORM_LIB_DIR)/codecWMADec_WMMX.lib \
   $(PLATFORM_LIB_DIR)/libdspaudioxscale.lib
   
ARM_LINK_FLAG += --libpath $(ARMOS_ROOT_DIR)/common/tools/rv31/nofplib      
endif

# Roundrock 
ifeq ($(PLATFORM), roundrock) 
CFLAGS += \
 --cpu xscale \
 --fpu VFPv3 
 
 SCATTER_FILE = tavor_pj4.sct
 EXTRA_LINK_FILE =
 EXTRA_LINK_LIB = \
   $(PLATFORM_LIB_DIR)/codecAC_XSCNCL.lib \
   $(PLATFORM_LIB_DIR)/ippAC_XSCNCL_r.lib \
   $(PLATFORM_LIB_DIR)/codecWMA_XSCNCL.lib \
   $(PLATFORM_LIB_DIR)/libdspaudioxscale.lib   
   
ifeq ($(BUILDLIB), MG1_A0_20091202)
	ARM_LINK_FLAG += --libpath $(ARMOS_ROOT_DIR)/common/tools/rv31/fplib
else
	ARM_LINK_FLAG += --libpath $(ARMOS_ROOT_DIR)/common/tools/rv31/nofplib
endif
endif

# Sprucewood 
ifeq ($(PLATFORM), sprucewood)
CFLAGS += \
 --cpu xscale \
 --fpu SoftVFP 
 
 SCATTER_FILE = tavor_pj4.sct
 EXTRA_LINK_FILE = 
 EXTRA_LINK_LIB = $(PLATFORM_LIB_DIR)/codecAC_XSCNCL.lib \
   $(PLATFORM_LIB_DIR)/ippAC_XSCNCL_r.lib  \
   $(PLATFORM_LIB_DIR)/codecWMA_XSCNCL.lib \
   $(PLATFORM_LIB_DIR)/libdspaudioxscale.lib  
   
ARM_LINK_FLAG += --libpath $(ARMOS_ROOT_DIR)/common/tools/rv31/nofplib      
endif
# mmp2_vp 
ifeq ($(PLATFORM), mmp2_vp)
CFLAGS += \
 --cpu xscale \
 --fpu SoftVFP 
 
 SCATTER_FILE = mmp_pj4.sct
 EXTRA_LINK_FILE = 
 EXTRA_LINK_LIB = 
 PLATFORM_SIM = true
ARM_LINK_FLAG += --libpath $(ARMOS_ROOT_DIR)/common/tools/rv31/nofplib      
endif

ifeq ($(PLATFORM), mmp2_flint)
CFLAGS += \
 --cpu xscale \
 --fpu SoftVFP 
 
 SCATTER_FILE = mmp_pj4.sct
 EXTRA_LINK_FILE = 
 EXTRA_LINK_LIB = 
ARM_LINK_FLAG += --libpath $(ARMOS_ROOT_DIR)/common/tools/rv31/nofplib      
endif

# -------------------------------------------------------------------------
#Include folders for .c or .s -> .o files
INC += \
 -Iinclude\
 -I$(ARMOS_ROOT_DIR)/\
 -I$(ARMOS_ROOT_DIR)/common/include \
 -I$(ARMOS_ROOT_DIR)/common/inc_pub \
 -I$(ARMOS_ROOT_DIR)/common/inc_mfg \
 -I$(ARMOS_ROOT_DIR)/common/firmware/include \
 -I$(ARMOS_ROOT_DIR)/common/firmware/clocks/inc_os \
 -I$(ARMOS_ROOT_DIR)/common/tools/rv31/include \
 -I$(ARMOS_ROOT_DIR)/nessus/inc_pub/ \
 -I$(ARMOS_ROOT_DIR)/nessus/include/ \
 -I$(ARMOS_ROOT_DIR)/common/config/$(PLATFORM)

# -------------------------------------------------------------------------
# from $(TOOLSDIR)/arm_armdefault.mak
OBJCONV_XDB_FILES := $(TOOLSDIR)/msa33/dwarf2bd.exe -hx $(TARGET_ELF)

ELFAR	:= $(TOOLSDIR)/msa33/elfar.exe

