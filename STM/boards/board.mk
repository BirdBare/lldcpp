ifeq ($(findstring STM32F767,$(TARGET)),STM32F767)
include $(BASEDIR)/STM/boards/F7/STM32F767/board.mk
endif

ifeq ($(findstring STM32F407,$(TARGET)),STM32F407)
include $(BASEDIR)/STM/boards/F4/STM32F407/board.mk
endif

flash:
	st-flash --reset write $(OUT)/$(TARGET).bin 0x8000000


