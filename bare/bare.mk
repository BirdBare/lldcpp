VPATH += $(BASEDIR)/bare/include 
VPATH += $(BASEDIR)/bare/communication 
VPATH += $(BASEDIR)/bare/kernel
VPATH += $(BASEDIR)/bare/os
VPATH += $(BASEDIR)/bare/os/hal

BAREOBJECTS = buffer.o \
dll.o \
mutex.o \
event.o \
thread.o \
spi_hal.o \
gpio_hal.o \
