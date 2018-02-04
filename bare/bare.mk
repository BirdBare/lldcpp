VPATH += $(BASEDIR)/bare/include 
VPATH += $(BASEDIR)/bare/communication 
VPATH += $(BASEDIR)/bare/kernel
VPATH += $(BASEDIR)/bare/os
VPATH += $(BASEDIR)/bare/os/os
VPATH += $(BASEDIR)/bare/drivers
VPATH += $(BASEDIR)/bare/os/hal
VPATH += $(BASEDIR)/bare/os/hal/
VPATH += $(BASEDIR)/bare/os/hal/gpio
VPATH += $(BASEDIR)/bare/os/hal/spi
VPATH += $(BASEDIR)/bare/os/hal/essential/timer
VPATH += $(BASEDIR)/bare/os/hal/essential/flash
VPATH += $(BASEDIR)/bare/os/hal/essential/clock
VPATH += $(BASEDIR)/bare/os/hal/essential/power
VPATH += $(BASEDIR)/bare/os/hal/essential/interrupt
VPATH += $(BASEDIR)/bare/os/hal/essential/system

BAREOBJECTS = buffer.o \
dll.o \
mutex.o \
bareos.o \
\
system_hal.o \
spi_hal.o \
gpio_hal.o \
timer_hal.o \
\
