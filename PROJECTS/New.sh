Makefile="BASEDIR = ../..
#Parent Build directory that contains all the files needed for compilation

############ TARGET CHECKING #########

TARGET = 
#Target Device to use in CAPS. Ex. STM32F767

ifndef TARGET
\$(error TARGET NOT SET! Run Target.sh to set TARGET)
endif
#if target is not set. error out.

######################################

########### DIRECTORIES #############

INCDIR = inc
SRCDIR = src
OUT = bin
#directories in folders as variables

BOARDDIR = boards/\$(TARGET)
#directory of board files. OBJ and DEBUG files will go here too.

OBJDIR = \$(BOARDDIR)/Objects
DEBDIR = \$(BOARDDIR)/Debug
#output directories

####################################


############# SOURCES ##############

OBJECTS = startup.o
#variable to hold our objects 

VPATH =	\$(INCDIR) \\
				\$(SRCDIR) \\
				\$(BASEDIR)/bare/include \\
#for source paths

###################################


######### THIS CAN CHANGE WHEN INCLUDING board.mk #############

LDFILE = boards/\$(TARGET)/flash.ld 
#user defined linker file path

STFILE = boards/\$(TARGET)/startup.s
#user defined startup file path

VPATH += boards/\$(TARGET) 
#user defined pins.h location. 
#added before board.mk so it is the first location vpath looks for board files

############## AGAIN THESE CAN CHANGE EACH REMAKE #############

############# COMPILERS ################

CC = arm-none-eabi-gcc
CASM = arm-none-eabi-as
CC++ = arm-none-eabi-g++
LD = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
#Various programs used to compile and then output debug files

##########################################

############### COMPILER FLAGS #################

ASMFLAGS = -mlittle-endian

CFLAGS =	-Wall -g -O2 \\
					-fdata-sections -ffunction-sections -ffast-math -Wno-unused-function \\
					\$(ASMFLAGS) \$(INC)

LDFLAGS =	-fdata-sections -ffunction-sections -ffast-math -specs=nano.specs \\
					-lc -lm -lnosys -Wl,-T,\$(LDFILE) \$(ASMFLAGS) \\
					-Wl,--gc-sections,-Map=\$(DEBDIR)/ALL_MAP.debug

OBJDUMPFLAGS = -D
#variables to hold our flags

###############################################

include \$(BASEDIR)/board.mk
#include file to decide what board is being used

include \$(SRCDIR)/sources.mk
#include file to get user sources.

########## Accumulate C files to be compiled #############

OBJECTS += \$(LLDOBJECTS)
#add hal objects to OBJECTS variables

INC = \$(addprefix -I,\$(VPATH))
#Set INC the same as VPATH variables so we can use local headers

###########################################################

############### files in directory variables for prereqs ############

OBJINDIR = \$(addprefix \$(OBJDIR)/,\$(OBJECTS))
DEBINDIR = \$(addprefix \$(DEBDIR)/,\$(addsuffix .debug,\$(basename \$(OBJECTS))))
#output files

DEPINDIR = \$(addprefix \$(OBJDIR)/,\$(addsuffix .depends,\$(basename \$(OBJECTS))))
#for file dependencies. For remaking if any file is changed.

#####################################################################


.PRECIOUS: \$(OBJINDIR) \$(DEBINDIR) \$(OUT)/\$(TARGET).elf
#prevent make from deleting files

-include \$(DEPINDIR)
#include GCC produced dependency file so we will remake any file modification

#################### TARGET RULES ####################

all: FOLDERS \$(OUT)/\$(TARGET).bin size

# ******** C FILES ***********
#Make C files and keep up to date
\$(OBJDIR)/%.o: %.c 
	\$(CC) -c \$(CFLAGS) \$< -o \$@
	@\$(CC) -MM -MT\$@ \$(CFLAGS) \$< -o \$(OBJDIR)/\$(basename \$(notdir \$@)).depends
	@echo
# ********** END ***********

# ********** ASSEMBLY FILES *********
#assembly files. 
INCDIR = inc
SRCDIR = src
\$(OBJDIR)/%.o: %.s 
	\$(CASM) -c \$(ASMFLAGS) \$< -o \$@
	@echo
#********** END ***********

# ********* DEBUGGING *********
#outputs assembly for debugging 
\$(DEBDIR)/%.debug: \$(OBJDIR)/%.o
	@\$(OBJDUMP) -D \$< > \$@
# ********** END *************



#make elf then binary file
%.elf: \$(OBJINDIR) \$(DEBINDIR) \$(LDFILE)
	\$(LD) \$(OBJINDIR) \$(LDFLAGS) -o \$@

%.bin: %.elf
	@\$(OBJCOPY) -O binary -S \$< \$@

######################################################


############ BOOK KEEPING RULES ###############

FOLDERS:
	@mkdir -p \$(OUT) \$(BOARDDIR) \$(OBJDIR) \$(DEBDIR) 

clean:
	@rm -rf \$(OBJDIR) \$(DEBDIR) 
	@rm -f \$(OUT)/\$(TARGET).bin \$(OUT)/\$(TARGET).elf

size:
	@echo
	@wc -c \$(OUT)/\$(TARGET).bin | sed 's/\$(OUT)\///'
	@echo
#end of program rules

###############################################


"

SOURCESMK="#User Source files. Suffix with .o
##########
#Example source.o where source is the filename

OBJECTS += main.o \\



"

MAINC="//
//
//
//
//


#include \"main.h\"

int main(void)
{



	return 1;
}



"

MAINH="//
//
//
//
//


#include \"pins.h\"
#include \"board_general_func.h\"



"

README="					
					DO NOT MODIFY MAKEFILE IN ANY WAY
					MAKEFILE WILL DETECT WHAT FILES TO USE.

					User Modified Files: sources.mk
					User Modified Folders: boards/TARGET/

					New Targets should be added in targets.txt

	Source files go in src.
		-Add source name to Sources.mk so your source will be compiled
	
	Header files go in inc.

	boards folder: In this folder will be devices you have tried to compile for.
		-Device Folders: Stores Device Specific files.
			-Object files will be placed here.
			-Dependecy files will be placed here.
			-Debug files will be placed here.
				IMPORTANT - 	You can copy board files to this location to override
				default files like the flash.ld, boards.c .h, and pins.h


	To change target board: Run Target.sh with board

	To Make project for board: 	make all
	To Clean board files: 		make clean
	To Flash to target board: 	make flash
				

	ENJOY!

"

TARGET="
	if [ -n \"\$1\" ]
	#check if variable is set to something other than null
	then
	#if so, check if it is a valid target.
		if grep -qw \"\$1\" ../../targets.txt
		#use grep to find exact string match in Targets.txt
		then
		#if found. Make the board file and change the target in makefile
			mkdir -p boards/\$1
			sed -i \"/TARGET\ =/c\TARGET\ =\ \$1\" Makefile
		else
		#if not found. List possible Targets
			echo
			echo \"Target Not Found!\"
			echo
			cat ../../targets.txt
			echo
		fi
	else
	#if not set. Tell user.
		echo
		echo Must Choose Target
		echo
		cat ../../targets.txt
		echo
	fi
"


if [ -z "$1" ];
then
	echo "Must Name Project"
	#check if var 1 is filled
else
	if [ -d "$1" ];
	#check if folder already exists with that name
	then
		echo "Project Already Exists. Choose another project name"
	else
	#if all is well. Make the project folder.
		mkdir $1
		cd $1
		mkdir boards inc src
		echo "$MAINC" >> src/main.c
		echo "$SOURCESMK" >> src/sources.mk
		echo "$MAINH" >> inc/main.h
		echo  "$Makefile" >> Makefile
		echo "$README" >> Readme.txt
		echo "$TARGET" >> Target.sh
		chmod 777 Target.sh
	fi
fi


