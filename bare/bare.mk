VPATH += $(BASEDIR)/bare/include 
VPATH += $(BASEDIR)/bare/communication 
VPATH += $(BASEDIR)/bare/kernel
VPATH += $(BASEDIR)/bare/os
VPATH += $(BASEDIR)/bare/os/os
VPATH += $(BASEDIR)/bare/os/BareList
VPATH += $(BASEDIR)/bare/drivers
VPATH += $(BASEDIR)/bare/os/hal
VPATH += $(BASEDIR)/bare/os/gui
VPATH += $(BASEDIR)/bare/os/gui/lcd
VPATH += $(BASEDIR)/bare/os/gui/font
VPATH += $(BASEDIR)/bare/os/hal/
VPATH += $(BASEDIR)/bare/os/hal/gpio
VPATH += $(BASEDIR)/bare/os/hal/spi
VPATH += $(BASEDIR)/bare/os/hal/dma
VPATH += $(BASEDIR)/bare/os/hal/essential/timer
VPATH += $(BASEDIR)/bare/os/hal/essential/clock

BAREOBJECTS = \
gpio_hal.o \
#buffer.o \
mutex.o \
bareos.o \
\
spi_hal.o \
dma_hal.o \
timer_hal.o \
clock_hal.o \
nokia5110_register.o \
\
