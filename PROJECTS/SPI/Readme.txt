					
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


