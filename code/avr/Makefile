# This is a prototype Makefile. Modify it according to your needs.
# You should at least check the settings for
# DEVICE ....... The AVR device you compile for
# CLOCK ........ Target AVR clock rate in Hertz
# OBJECTS ...... The object files created from your source files. This list is
#                usually the same as the list of source files with suffix ".o".
# PROGRAMMER ... Options to avrdude which define the hardware you use for
#                uploading to the AVR and the interface where this hardware
#                is connected.
# FUSES ........ Parameters for avrdude to flash the fuses appropriately.

FILE       = zwaailicht
DEVICE     = attiny13
CLOCK      = 1200000
PROGRAMMER = -c avr910 -P /dev/tty.SLAB_USBtoUART
OBJECTS    = $(FILE).o
FUSES      = -U hfuse:w:0xff:m -U lfuse:w:0x6a:m
# attiny2313 fuse bits (fuse bits for other devices are different!):
# Example for 8 MHz internal oscillator
# Fuse high byte:
# 0xff = 1 1 1 1   1 1 1 1 <-- RSTDISBL	(1)	External Reset disable
#        ^ ^ ^ ^   ^ ^ ^------ BODLEVEL0(1)	Brownout detector level 0
#        | | | |   | +-------- BODLEVEL1(1)	Brownout detector level 1
#        | | | |   +---------- DWEN		(1)	debugWire enable
#        | | | +-------------- SELFPRGEN(1)	Self programming enable
#        | | +---------------- NA
#        | +------------------ NA
#        +-------------------- NA
# Fuse low byte:
# 0x6a = 0 1 1 0   1 0 1 0 <-- CKSEL0	(0)	Select clock source
#        ^ ^ ^ ^   ^ ^ ^------ CKSEL1	(1)	Select clock source
#        | | | |   | +-------- SUT0		(0)	Select start-up time
#        | | | |   +---------- SUT1		(1)	Select start-up time
#        | | | +-------------- CKDIV8	(0)	Divide clock by 8
#        | | +---------------- WDTON	(1)	Watchdog timr always on
#        | +------------------ EESAVE	(1)	Preserve EEPROM memory through chip erase
#        +-------------------- SPIEN	(0)	Enable serial programming and data downloading
# Tune the lines below only if you know what you are doing:

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE) -x devcode=0x57
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) -I./

# symbolic targets:
all:	$(FILE).hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:$(FILE).hex:i

fuse:
	$(AVRDUDE) $(FUSES)

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID $(FILE).hex

clean:
	rm -f $(FILE).hex $(FILE).elf $(OBJECTS)

# file targets:
$(FILE).elf: $(OBJECTS)
	$(COMPILE) -o $(FILE).elf $(OBJECTS)

$(FILE).hex: $(FILE).elf
	rm -f $(FILE).hex
	avr-objcopy -j .text -j .data -O ihex $(FILE).elf $(FILE).hex
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	$(FILE).elf
	avr-objdump -d $(FILE).elf

cpp:
	$(COMPILE) -E $(FILE).c
