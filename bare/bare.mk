VPATH += $(BASEDIR)/bare/include 
VPATH += $(BASEDIR)/bare/communication 
VPATH += $(BASEDIR)/bare/kernel
VPATH += $(BASEDIR)/bare/os
VPATH += $(BASEDIR)/bare/os/os
VPATH += $(BASEDIR)/bare/drivers
VPATH += $(BASEDIR)/bare/os/hal
VPATH += $(BASEDIR)/bare/os/hal/gpio
VPATH += $(BASEDIR)/bare/os/hal/spi
VPATH += $(BASEDIR)/bare/os/hal/timer

BAREOBJECTS = buffer.o \
dll.o \
mutex.o \
bareos.o \
spi_hal.o \
gpio_hal.o \
timer_hal.o \
