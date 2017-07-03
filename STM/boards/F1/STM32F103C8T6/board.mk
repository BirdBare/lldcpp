ifeq ($(wildcard boards/$(TARGET)/user/startup/flash.ld),)
	LDFILE = $(BASEDIR)/STM/boards/F1/STM32F103/user/startup/flash.ld
endif

VPATH +=	$(BASEDIR)/STM/boards/F1/common/driver/arm \
					$(BASEDIR)/STM/boards/F1/common/driver/st 
					#common f4 driver files

VPATH +=	$(BASEDIR)/STM/boards/F1/STM32F103/driver/st 
VPATH +=	$(BASEDIR)/STM/boards/F1/STM32F103/user
VPATH +=	$(BASEDIR)/STM/boards/F1/STM32F103/user/startup
					#device driver files

VPATH +=	\
					#hal library files
#add board files to path

ASMFLAGS += -mthumb -mcpu=cortex-m3 -mfpu=softvfp -mfloat-abi=soft

CFLAGS += -mtune=cortex-m3 -D STM32F103xB
#add board specific flags for compiler

LLDOBJECTS =	\
#gpio_lld.o \
clock_lld.o \
flash_lld.o \
systick_lld.o \
pwr_lld.o \
nvic_lld.o \
usart_lld.o \
spi_lld.o \
i2c_lld.o \
#							ADC_lld.o \
							DMA_lld.o \
							
LLDOBJECTS +=						#	EXTI_lld.o \
							FPU_lld.o \
							#I2C_lld.o \
							
LLDOBJECTS += #MPU_lld.o \

LLDOBJECTS +=	#rcc_lld.o \
							#RNG_lld.o \
							RTC_lld.o \
							#SYSTICK_lld.o \
							
LLDOBJECTS += #\

