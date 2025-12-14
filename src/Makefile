#	Makefile for avr-gcc
#	Items marked by a * in the comments need to be updated for
#	each project.

#	=========================================================
#	Make Reminder
#	=============
#	target: dependencies
#		command-to-build-target-using-dependencies
#
#	$@ = target, $< = first dependency, $^ = all dependencies
#	=========================================================

#	*TARGET specifies the desired binary output.
#	*MCU is the name of the AVR device to program.
#	*AVR_PROG is the ISP programmer used to flash the AVR.
TARGET   = therm
MCU      = attiny85
AVR_PROG = usbtiny


#	*AVR_PORT is the USB port the ISP programmer is connected to.
#	Uncomment and set to appropriate port for non usbtiny ISP
#	programmers.
#AVR_PORT =


#	*AVRFUSES
# attiny2313 Int. 8 MHz (no divider), no clock out
#AVR_FUSES = -U lfuse:w:0xe4:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

# attiny2313 Int. 1 MHz, clock out
#AVR_FUSES = -U lfuse:w:0x24:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m


# 	AVRDUDEFLAGS are parameters sent to avrdude. Can add -B255 for
#	slow chips, or even fuse settings if desired.
AVRDUDEFLAGS = -c $(AVR_PROG) -p $(MCU)
ifdef AVR_PORT
AVRDUDEFLAGS += -P $(AVR_PORT)
endif


#	*PASSPARAM are defines "variables" passed and compiled into all
# 	C sources from this Makefile, for example, F_CPU, etc. Each
#	variable should have -D before it with no space between the
#	-D and the defined variable.
#	Comment out if unused.
PASSPARAM = -DF_CPU=1000000UL


#	*DEPS specify what non-compiled files need to trigger compilation
#	in the event that any of these files are changed. Specifiy
#	library header files here as well as Makefile in case Makefile is
#	sending defines parameters to C sources for compilation.
DEPS = Makefile


#	*OBJS are library/include objects based on included libraries.
#	Have an entry for each C library file here.
OBJS = $(TARGET).o


#	CC specifies the C compiler.
#	CFLAGS specifies the C compiler command-line flags.
CC     = avr-gcc
CFLAGS = -Os -Wall -I. -g -mmcu=$(MCU)


#	Send hex data to AVR, and report size info from elf binary
flash: $(TARGET).hex $(TARGET).elf
	avrdude $(AVRDUDEFLAGS) -U $<
	avr-size -C --mcu=$(MCU) $(TARGET).elf


#	Generate hex file from elf binary. The elf binary is chosen
#	over the standard ".bin" binary as it contains information
#	needed by avr-size for size reporting.
$(TARGET).hex: $(TARGET).elf
	avr-objcopy -O ihex -j .text -j .data $< $@


#	Compile elf binary from all object files
$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@


#	This is a catch-all line that makes sure all required object
#   files based on any C source will be compiled into the desired
#	object code. It makes sure that these objects are recompiled
#	whenever any of the dependancies in DEPS are changed.
#	Note that the CFLAGS must be added to the compile rule, though
#	it is not required to add $(CFLAGS) to the rule when using .o
#	object files because it will automatically be part of the rule.
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(PASSPARAM)

#	*Clean up tertiary .o, .hex, .elf files.
#	If compiling a library file, you can add the directory and
#	file to erase here in a second line.
.PHONY: clean
clean:
	rm -f *.o *.hex *.elf $(TARGET)
