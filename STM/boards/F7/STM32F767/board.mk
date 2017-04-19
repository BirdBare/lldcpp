ifeq ($(wildcard boards/$(TARGET)/flash.ld),)
	LDFILE = $(BASEDIR)/STM/boards/F7/STM32F767/flash.ld
endif

VPATH +=	$(BASEDIR)/STM/boards/F7/common/driver/arm \
					$(BASEDIR)/STM/boards/F7/common/driver/st 
					#common f7 driver files

VPATH +=	$(BASEDIR)/STM/boards/F7/STM32F767/driver/st 
VPATH +=	$(BASEDIR)/STM/boards/F7/STM32F767/
					#device driver files

VPATH +=	\
					$(BASEDIR)/STM/lld/ADC/V1 \
					$(BASEDIR)/STM/lld/CACHE/V1 \
					$(BASEDIR)/STM/lld/CRC/V1 \
					$(BASEDIR)/STM/lld/DMA/V1 \
					$(BASEDIR)/STM/lld/DMA2D/V1 \
					$(BASEDIR)/STM/lld/EXTI/V1 \
					$(BASEDIR)/STM/lld/FLASH/V1 \
					$(BASEDIR)/STM/lld/FMC/V1 \
					$(BASEDIR)/STM/lld/FPU/V1 \
					$(BASEDIR)/STM/lld/GPIO/V1 \
					$(BASEDIR)/STM/lld/I2C/V1 \
					$(BASEDIR)/STM/lld/LPTIM/V1 \
					$(BASEDIR)/STM/lld/MPU/V1 \
					$(BASEDIR)/STM/lld/NVIC/V1 \
					$(BASEDIR)/STM/lld/PWR/V1 \
					$(BASEDIR)/STM/lld/RCC/V1 \
					$(BASEDIR)/STM/lld/RNG/V1 \
					$(BASEDIR)/STM/lld/RTC/V1 \
					$(BASEDIR)/STM/lld/SPI/V1 \
					$(BASEDIR)/STM/lld/SYSCFG/V1 \
					$(BASEDIR)/STM/lld/SYSTICK/V1 \
					$(BASEDIR)/STM/lld/TIM/V1 \
					$(BASEDIR)/STM/lld/USART/V1 \
					#hal library files
#add board files to path

ASMFLAGS += -mthumb -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard

CFLAGS += -mtune=cortex-m7 -D STM32F767xx
#add board specific flags for compiler


LLDOBJECTS =	\
							ADC_lld.o \
							CACHE_lld.o \
							CRC_lld.o \
							DMA_lld.o \
							#DMA2D_lld.o \
							
LLDOBJECTS +=							EXTI_lld.o \
							FLASH_lld.o \
							FMC_lld.o \
							FPU_lld.o \
							GPIO_lld.o \
							#I2C_lld.o \
							
LLDOBJECTS += LPTIM_lld.o \
							MPU_lld.o \
							NVIC_lld.o \
							#PWR_lld.o \

LLDOBJECTS +=	RCC_lld.o \
							RNG_lld.o \
							RTC_lld.o \
							SPI_lld.o \
							SYSCFG_lld.o \
							#SYSTICK_lld.o \
							
LLDOBJECTS += TIM_lld.o \
							#USART_lld.o \

