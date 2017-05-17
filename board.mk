
ifeq ($(findstring STM,$(TARGET)),STM)
include $(BASEDIR)/STM/boards/board.mk
endif

