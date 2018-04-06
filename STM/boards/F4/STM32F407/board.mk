ifeq ($(wildcard boards/$(TARGET)/user/startup/flash.ld),)
	LDFILE = $(BASEDIR)/STM/boards/F4/STM32F407/user/startup/flash.ld
endif

VPATH +=	$(BASEDIR)/STM/boards/F4/common/driver/arm \
					$(BASEDIR)/STM/boards/F4/common/driver/st 
					#common f4 driver files

VPATH +=	$(BASEDIR)/STM/boards/F4/STM32F407/driver/st 
VPATH +=	$(BASEDIR)/STM/boards/F4/STM32F407/user
VPATH +=	$(BASEDIR)/STM/boards/F4/STM32F407/user/startup
					#device driver files

VPATH +=	\
					$(BASEDIR)/STM/lld \
					$(BASEDIR)/STM/lld/ADC/V1 \
					$(BASEDIR)/STM/lld/CRC/V1 \
					$(BASEDIR)/STM/lld/CLOCK/V1 \
					$(BASEDIR)/STM/lld/DMA/V1 \
					$(BASEDIR)/STM/lld/DMA2D/V1 \
					$(BASEDIR)/STM/lld/EXTI/V1 \
					$(BASEDIR)/STM/lld/FLASH/V1 \
					$(BASEDIR)/STM/lld/FMC/V1 \
					$(BASEDIR)/STM/lld/FPU/V1 \
					$(BASEDIR)/STM/lld/GPIO/V2 \
					$(BASEDIR)/STM/lld/I2C/V1 \
					$(BASEDIR)/STM/lld/LPTIM/V1 \
					$(BASEDIR)/STM/lld/MPU/V1 \
					$(BASEDIR)/STM/lld/NON_HAL/NVIC/V1 \
					$(BASEDIR)/STM/lld/PWR/V1 \
					$(BASEDIR)/STM/lld/NON_HAL/RCC/V1 \
					$(BASEDIR)/STM/lld/RNG/V1 \
					$(BASEDIR)/STM/lld/RTC/V1 \
					$(BASEDIR)/STM/lld/SPI/V1 \
					$(BASEDIR)/STM/lld/SYSCFG/V1 \
					$(BASEDIR)/STM/lld/SYSTICK/V1 \
					$(BASEDIR)/STM/lld/TIM/V1 \
					$(BASEDIR)/STM/lld/USART/V1 \
					#hal library files
#add board files to path

ASMFLAGS += -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard

CFLAGS += -mtune=cortex-m4 -D STM32F407xx
#add board specific flags for compiler

LLDOBJECTS =	\
nvic_lld.o \
rcc_lld.o \
gpio_lld.o \
dma_lld.o \
#clock_lld.o \
#spi_lld.o \
timer_lld.o \
#							ADC_lld.o \
							DMA_lld.o \
							
LLDOBJECTS +=						#	EXTI_lld.o \
							FPU_lld.o \
							#I2C_lld.o \
							
LLDOBJECTS += #MPU_lld.o \

LLDOBJECTS +=	#
							#RNG_lld.o \
							RTC_lld.o \
							#SYSTICK_lld.o \
							
LLDOBJECTS += #\

