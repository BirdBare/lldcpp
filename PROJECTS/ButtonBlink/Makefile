BASEDIR = ../..
#Parent Build directory that contains all the files needed for compilation

############ TARGET CHECKING #########

TARGET = STM32F407
#Target Device to use in CAPS. Ex. STM32F767

ifndef TARGET
$(error TARGET NOT SET! Run Target.sh to set TARGET)
endif
#if target is not set. error out.

######################################

########### DIRECTORIES #############

INCDIR = inc
SRCDIR = src
OUT = bin
#directories in folders as variables

BOARDDIR = boards/$(TARGET)
#directory of board files. OBJ and DEBUG files will go here too.

OBJDIR = $(BOARDDIR)/Objects
DEBDIR = $(BOARDDIR)/Debug
#output directories

####################################


############# SOURCES ##############

OBJECTS = startup.o
#variable to hold our objects 

VPATH =	$(INCDIR) \
				$(SRCDIR) \
#for source paths

###################################


######### THIS CAN CHANGE WHEN INCLUDING board.mk #############

LDFILE = boards/$(TARGET)/user/startup/flash.ld 
#user defined linker file path

VPATH += boards/$(TARGET)/user 
VPATH += boards/$(TARGET)/user/startup
#user defined pins.h location. 
#added before board.mk so it is the first location vpath looks for board files

############## AGAIN THESE CAN CHANGE EACH REMAKE #############

############# COMPILERS ################

CC = arm-none-eabi-gcc
CASM = arm-none-eabi-as
CPP = arm-none-eabi-g++
LD = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
#Various programs used to compile and then output debug files

##########################################

############### COMPILER FLAGS #################

ASMFLAGS = -mlittle-endian

CFLAGS =	-Wall -g -O0 \
					-fdata-sections -ffunction-sections -ffast-math -Wno-unused-function \
					$(ASMFLAGS) $(INC)

CPPFLAGS = $(CFLAGS)

LDFLAGS =	-fdata-sections -ffunction-sections -ffast-math -specs=nano.specs \
					-lc -lm -lnosys -Wl,-T,$(LDFILE) $(ASMFLAGS) \
					-Wl,--gc-sections,-Map=$(DEBDIR)/ALL_MAP.debug

OBJDUMPFLAGS = -D -z, --disassemble-zeros
#variables to hold our flags

###############################################

include $(BASEDIR)/board.mk
#include file to decide what board is being used

include $(BASEDIR)/bare/bare.mk

include $(SRCDIR)/sources.mk
#include file to get user sources.

########## Accumulate C files to be compiled #############

OBJECTS += $(LLDOBJECTS)
#add hal objects to OBJECTS variables

OBJECTS += $(BAREOBJECTS)
#add bare objects to OBJECTS variables

INC = $(addprefix -I,$(VPATH))
#Set INC the same as VPATH variables so we can use local headers

###########################################################

############### files in directory variables for prereqs ############

OBJINDIR = $(addprefix $(OBJDIR)/,$(OBJECTS))
DEBINDIR = $(addprefix $(DEBDIR)/,$(addsuffix .debug,$(basename $(OBJECTS))))
#output files

DEPINDIR = $(addprefix $(OBJDIR)/,$(addsuffix .depends,$(basename $(OBJECTS))))
#for file dependencies. For remaking if any file is changed.

#####################################################################


.PRECIOUS: $(OBJINDIR) $(DEBINDIR) $(OUT)/$(TARGET).elf
#prevent make from deleting files

-include $(DEPINDIR)
#include GCC produced dependency file so we will remake any file modification

#################### TARGET RULES ####################

all: FOLDERS $(OUT)/$(TARGET).bin size

# ******** C FILES ***********
#Make C files and keep up to date
$(OBJDIR)/%.o: %.c 
	$(CC) -c $(CFLAGS) $< -o $@
	@$(CC) -MM -MT$@ $(CFLAGS) $< -o $(OBJDIR)/$(basename $(notdir $@)).depends
	@echo
# ********** END ***********

# ******** C++ FILES ***********
#Make C++ files and keep up to date
$(OBJDIR)/%.o: %.cpp 
	$(CPP) -c $(CPPFLAGS) $< -o $@
	@$(CPP) -MM -MT$@ $(CPPFLAGS) $< -o $(OBJDIR)/$(basename $(notdir $@)).depends
	@echo
# ********** END ***********

# ********** ASSEMBLY FILES *********
#assembly files. 
INCDIR = inc
SRCDIR = src
$(OBJDIR)/%.o: %.s 
	$(CASM) -c $(ASMFLAGS) $< -o $@
	@echo
#********** END ***********

# ********* DEBUGGING *********
#outputs assembly for debugging 
$(DEBDIR)/%.debug: $(OBJDIR)/%.o
	@$(OBJDUMP) -D $< > $@
# ********** END *************



#make elf then binary file
%.elf: $(OBJINDIR) $(DEBINDIR) $(LDFILE)
	$(LD) $(OBJINDIR) $(LDFLAGS) -o $@

%.bin: %.elf
	@$(OBJCOPY) -O binary -S $< $@

######################################################


############ BOOK KEEPING RULES ###############

FOLDERS:
	@mkdir -p $(OUT) $(BOARDDIR) $(OBJDIR) $(DEBDIR) 

clean:
	@rm -rf $(OBJDIR) $(DEBDIR) 
	@rm -f $(OUT)/$(TARGET).bin $(OUT)/$(TARGET).elf

size:
	@echo
	@wc -c $(OUT)/$(TARGET).bin | sed 's/$(OUT)\///'
	@echo
#end of program rules

###############################################



