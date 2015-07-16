###########################################################################
# File:         <make file name>
# Copyright     2008, Research In Motion
# Author:       
# Created:      July 2008
# Description:  This is a makefile template
##########################################################################
# -------------------------------------------------------------------------
# Includes
include build/makefile/common.mak

ifeq ($(PLATFORM_SIM), true)
BOOTLOADERBIN= $(PLATFORM_LIB_DIR)/virtual_platform_bin/BR-mmp2_sim.bin
HWVERFILE= $(PLATFORM_LIB_DIR)/virtual_platform_bin/mmp2_sim.hwv
endif

# -------------------------------------------------------------------------
# Variables and Macros
THREAD =
GPIO =
BUGDISP = $(PLATFORM_REL_DIR)/PlatformBugDispDef.ext
ifeq ($(BUILD_TGT), nessus)
ifeq ($(NESSUSLIB), 1)	
	THREAD = $(PLATFORM_REL_DIR)/threads.h
	GPIO = $(PLATFORM_REL_DIR)/gpio.h
endif
endif
MODULES += $(MODULE_LIST)
SHELL := $(TOOLSDIR)/sh.exe

LIB_LIST += $(foreach i,$(MODULES), $(i)/obj/$(PLATFORM)_$(BUILD_TGT)/$(strip $(subst /,_,$(i))).lib)

##dxeparse##
QUIET_FLAG = -q
OMIT_SECTIONS = -o
FIRST_STAGE_INIT = -f
THIRD_STAGE_INIT = -3
ON_DEMAND_STAGE_INIT = -4
ZERO_RUN_COMP = -c
LEMPEL_ZIV_COMP = -z
ITCM_SECTIONS = -t
WLAN_SECTIONS = -w
OVERLAY_CONFIG = -v
STRIP_METRICS = -m
SIGNATURE = -s

OMIT_SECTIONS += UNINIT_DATA EndERAM PRESERVED_DATA PRESERVE_DATA_WB PRESERVED_DATA_IRAM IRAM_PRESERVED_DATA \
				PRESERVED_DATA_BUFFER KERNEL_STACKS FIRSTSTACK STACKS SLOW_STACKS \
				FAST_STACKS Iram_DmaBuffers BR_PRESERVED_DATA IRAM_PAGETABLE StartERAM
FIRST_STAGE_INIT += IRAM IRAM_KERNEL_DATA IRAM_DATA
SIGNATURE += EndOfFlash

WLAN_SECTIONS =
THIRD_STAGE_INIT =
LEMPEL_ZIV_COMP =

#XSCALE specific
FIRST_STAGE_INIT += IRAM_IVT
FIRST_STAGE_INIT += BSRAM
STRIP_METRICS =
ITCM_SECTIONS =

#No overlay
OVERLAY_CONFIG =

## Default target
#If building the SFI
ifneq ($(NESSUSLIB), 1)
default : $(PLATFORM_REL_DIR)/Platform_$(BUILD_TGT).sfi
#If building the nessus Library
else
default : $(PLATFORM_REL_DIR)/libPlatform_$(BUILD_TGT).a
endif
.PHONY: $(MODULES)
# -------------------------------------------------------------------------
# Rules: explicit rules

#Building the sfi 
$(PLATFORM_REL_DIR)/Platform_$(BUILD_TGT).sfi: $(PLATFORM_REL_DIR)/Platform_$(BUILD_TGT).bin
	$(PRINT) Creating $@...
	$(PRINT) $(PLATFORM_LIB_DIR)/$(PLATFORM)_exporttable.txt
	$(ARMOS_ROOT_DIR)/common/tools/ArmFlashImageCreator.exe -nologo -vd CREATE $@ $(PLATFORM_REL_DIR)/Platform_$(BUILD_TGT).bin $(PLATFORM_LIB_DIR)/$(PLATFORM)_exporttable.txt
        ifeq ($(PLATFORM_SIM), true)
	$(ARMOS_ROOT_DIR)/common/tools/ArmFlashImageCreator.exe ADDHWV -s $(PLATFORM_REL_DIR)/miniOSVP_SIM.bin $(PLATFORM_REL_DIR)/Platform_$(BUILD_TGT).sfi $(HWVERFILE) $(BOOTLOADERBIN)
        endif
	$(ARMOS_ROOT_DIR)/common/tools/msa33/dwarf2bd.exe -hx $(PLATFORM_REL_DIR)/Platform_$(BUILD_TGT).elf $(PLATFORM_REL_DIR)/Platform_$(BUILD_TGT).bd


#Building the .bin from the .elf. 
$(PLATFORM_REL_DIR)/Platform_$(BUILD_TGT).bin: $(PLATFORM_REL_DIR)/Platform_$(BUILD_TGT).elf $(PLATFORM_REL_DIR)/PlatformDebug.dat
	$(PRINT) Creating $@...
	$(DXEPARSE) $(QUIET_FLAG) procesself $(PLATFORM_REL_DIR)/Platform_$(BUILD_TGT).elf $@  \
												$(OMIT_SECTIONS) \
												$(FIRST_STAGE_INIT) \
												$(THIRD_STAGE_INIT) \
												$(ON_DEMAND_STAGE_INIT) \
												$(ZERO_RUN_COMP) \
												$(LEMPEL_ZIV_COMP) \
												$(ITCM_SECTIONS) \
												$(WLAN_SECTIONS) \
												$(OVERLAY_CONFIG) \
												$(STRIP_METRICS) \
												$(SIGNATURE)

#Creating the Platform debug.dat and Merging the Nessus debug.dat with the .dat from the Platform. 	
$(PLATFORM_REL_DIR)/PlatformDebug.dat: $(PLATFORM_REL_DIR)/Platform_$(BUILD_TGT).elf
	$(PRINT) Creating $@...
	$(PRINT) BEGIN CORE 0 > $(PLATFORM_REL_DIR)/preliminary.dat.tmp
	$(CAT) $(PLATFORM_REL_DIR)/PlatformBugFileList.ext | $(XARGS) -i $(CAT) {} >> $(PLATFORM_REL_DIR)/preliminary.dat.tmp
	$(PRINT) BEGIN SYMBOLS >> $(PLATFORM_REL_DIR)/preliminary.dat.tmp
	$(CAT) $(PLATFORM_REL_DIR)/nessus_sorted.sym >> $(PLATFORM_REL_DIR)/preliminary.dat.tmp
	$(PRINT) END SYMBOLS >> $(PLATFORM_REL_DIR)/preliminary.dat.tmp
	$(TOOLSDIR)/MergeDebugDats -0 $(PLATFORM_REL_DIR)/PlatformBugDispDef.ext $(PLATFORM_REL_DIR)/preliminary.dat.tmp $(PLATFORM_REL_DIR)/PlatformDebug.dat
	$(RM) $(PLATFORM_REL_DIR)/preliminary.dat.tmp
	#Merge the Nessus .dat with the platform .dat 
	$(TOOLSDIR)/MergeDebugDats -0 	$(PLATFORM_REL_DIR)/PlatformDebug.dat $(PLATFORM_LIB_DIR)/debug.dat $(PLATFORM_REL_DIR)/PlatformDebug.dat.tmp
	$(CAT) $(PLATFORM_REL_DIR)/PlatformDebug.dat.tmp > $(PLATFORM_REL_DIR)/PlatformDebug.dat
	$(RM) $(PLATFORM_REL_DIR)/PlatformDebug.dat.tmp

#Linking the platform objects with those required from Nessus.
$(PLATFORM_REL_DIR)/Platform_$(BUILD_TGT).elf: $(PLATFORM_TGT_CONF) $(PLATFORM_REL_DIR)/libPlatform_$(BUILD_TGT).a $(PLATFORM_LIB_DIR)/$(SCATTER_FILE)  $(PLATFORM_LIB_DIR)/nessus.lib $(PLATFORM_LIB_DIR)/os_export_table.o $(PLATFORM_LIB_DIR)/metrics.o $(EXTRA_LINK_FILE) $(EXTRA_LINK_LIB)
	$(PRINT) Creating $@...
	$(LINK) -o $@ $(ARM_LINK_FLAG) $(PLATFORM_REL_DIR)/libPlatform_$(BUILD_TGT).a "$(PLATFORM_LIB_DIR)/nessus.lib(*.o, *.a)" $(PLATFORM_LIB_DIR)/os_export_table.o $(PLATFORM_LIB_DIR)/metrics.o $(EXTRA_LINK_FILE) $(EXTRA_LINK_LIB) 
	$(VENEERFIX) $@
	$(PYTHON) $(TOOLSDIR)/symboltrans.py $(PLATFORM_REL_DIR)/nessus.listing $(PLATFORM_REL_DIR)/nessus.sym
	$(PL) $(TOOLSDIR)/symbol_sort.pl $(PLATFORM_REL_DIR)/nessus.sym > $(PLATFORM_REL_DIR)/nessus_sorted.sym

#Creating the Platform Software Library
$(PLATFORM_REL_DIR)/libPlatform_$(BUILD_TGT).a : $(THREAD) $(GPIO) $(MODULES) $(BUGDISP)
	$(PRINT) Linking $(LIB_LIST) ...
	$(LIB) -r $@ $(LIB_LIST)
	$(PRINT) Platform Software Code Sucessfully Compiled!

#Add the platform threads into nessus.	
$(PLATFORM_REL_DIR)/threads.h: $(ARMOS_ROOT_DIR)/nessus/inc_pub/threads_$(THREADS_APP).xml
	$(PRINT) Generating $@
	$(CCPRE) --via $(PLATFORM_ROOT_DIR)/build/config/$(PLATFORM)_defines_c.txt -E $^ | $(SED) '/^#/d' > $(PLATFORM_REL_DIR)/threads.0
	$(PL) $(THREADGEN) -l 0 -c 0 -o $(PLATFORM_REL_DIR)/threads $(PLATFORM_REL_DIR)/threads.0
	
#Add the platform into gpio.h 
$(PLATFORM_REL_DIR)/gpio.h: $(ARMOS_ROOT_DIR)/common/config/$(PLATFORM)/config_gpio.xml
	$(PRINT) Generating $@
	$(GPIOGEN) $^ $(ARMOS_ROOT_DIR)/nessus/drivers/config_gpio_$(PLATFORM).c $(PLATFORM_REL_DIR)/config_gpio.h RIM_OS

#Create the files needed for Bugdisp
$(BUGDISP): $(MODULES)
	$(PRINT) Generating $@
	$(FIND) $(PLATFORM_ROOT_DIR) -name "*.bugx" > $(PLATFORM_REL_DIR)/PlatformBugFileList.ext
	$(FIND) $(PLATFORM_ROOT_DIR) -name "*.h" | $(XARGS) -i $(BUGBUILD) /l {} > $(PLATFORM_REL_DIR)/PlatformBugDispDef.ext
	

# recursive make for each module
$(MODULES) :
	echo ">>>> Building Module [$@] for target $(BUILD_TGT)"
	date
	$(MAKE) -f $@/$(notdir $@).mak

