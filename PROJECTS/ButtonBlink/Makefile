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
CPP = arm-none-eabi-c++
LD = arm-none-eabi-c++
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
#Various programs used to compile and then output debug files

##########################################

############### COMPILER FLAGS #################

ASMFLAGS = -mlittle-endian -mthumb-interwork

CFLAGS =	-Wall -Wextra -g -O0 -specs=nosys.specs -nostdlib \
					-fdata-sections -ffunction-sections -Wno-unused-function \
					$(ASMFLAGS) $(INC)

CPPFLAGS = $(CFLAGS) -fno-rtti -fno-exceptions -fno-unwind-tables

LDFLAGS =	-fdata-sections -ffunction-sections -specs=nosys.specs \
					-T $(LDFILE) $(ASMFLAGS) -lc -lm -lnosys \
					-Wl,--gc-sections,-Map=$(DEBDIR)/ALL_MAP.debug \

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


.PRECIOUS: $(OBJINDIR) $(DEBINDIR) $(BOARDDIR)/$(TARGET).elf
#prevent make from deleting files

-include $(DEPINDIR)
#include GCC produced dependency file so we will remake any file modification

#################### TARGET RULES ####################

all: FOLDERS $(BOARDDIR)/$(TARGET).bin $(BOARDDIR)/$(TARGET).hex size

# ******** C FILES ***********
#Make C files and keep up to date
$(OBJDIR)/%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@
	@$(CC) -MM -MT$@ $(CFLAGS) $< -o $(OBJDIR)/$(basename $(notdir $@)).depends
	@echo
# ********** END ***********

# ******** C++ FILES ***********
#Make C++ files and keep up to date
$(OBJDIR)/%.o: %.cpp 
	$(CPP) $(CPPFLAGS) -c $< -o $@
	@$(CPP) -MM -MT$@ $(CPPFLAGS) $< -o $(OBJDIR)/$(basename $(notdir $@)).depends
	@echo
# ********** END ***********

# ********** ASSEMBLY FILES *********
#assembly files. 
INCDIR = inc
SRCDIR = src
$(OBJDIR)/%.o: %.s 
	$(CASM) $(ASMFLAGS) -c $< -o $@
	@echo
#********** END ***********

# ********* DEBUGGING *********
#outputs assembly for debugging 
$(DEBDIR)/%.debug: $(OBJDIR)/%.o
	@$(OBJDUMP) -D $< > $@
# ********** END *************



#make elf then binary file
%.elf: $(OBJINDIR) $(DEBINDIR) $(LDFILE)
	$(LD) $(LDFLAGS) $(OBJINDIR) -o $@

%.bin: %.elf
	@$(OBJCOPY) -O binary -S $< $@

%.hex: %.elf
	@$(OBJCOPY) -O ihex -S $< $@
######################################################


############ BOOK KEEPING RULES ###############

FOLDERS:
	@mkdir -p $(BOARDDIR) $(OBJDIR) $(DEBDIR) 

tidy:
	@rm -rf $(OBJDIR) $(DEBDIR) 

clean:
	@rm -rf $(BOARDDIR)

size:
	@echo
	@wc -c $(BOARDDIR)/$(TARGET).bin 
	@echo
#end of program rules

###############################################



