
ARMGNU ?= arm-none-eabi

COMMONOPS = -Wall -nostdlib -nostartfiles -ffreestanding

#AZO_MEM_VFP = 1
ifeq ($(AZO_MEM_VFP),1)
COMMONOPS := $(COMMONOPS) -mfpu=vfp -DAZO_MEM_VFP
endif

#AZO_BOARD = RPZERO
#AZO_BOARD = RP1
#AZO_BOARD = RP2
AZO_BOARD = RP3
ifeq ($(AZO_BOARD),RPZERO)
COMMONOPS := $(COMMONOPS) -DAZO_CHIP_BCM2835 -DAZO_BOARD_RPZERO
else ifeq ($(AZO_BOARD),RP1)
COMMONOPS := $(COMMONOPS) -DAZO_CHIP_BCM2835 -DAZO_BOARD_RP1
else ifeq ($(AZO_BOARD),RP2)
COMMONOPS := $(COMMONOPS) -DAZO_CHIP_BCM2836 -DAZO_BOARD_RP2
else ifeq ($(AZO_BOARD),RP3)
COMMONOPS := $(COMMONOPS) -DAZO_CHIP_BCM2837 -DAZO_BOARD_RP3
endif

ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
AZO_CPU_SINGLE = 1
ifeq ($(AZO_CPU_SINGLE),1)
COMMONOPS := $(COMMONOPS) -DAZO_CPU_SINGLE
endif
endif

COPS := -O0 -g3 $(COMMONOPS)
#COPS = -O3 $(COMMONOPS)
ASMOPS := -g3 $(COMMONOPS)

ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1))
gcc : rp_midi_out.hex kernel.img rp_midi_out.hex rp_midi_out.syms rp_midi_out.srec
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
gcc : rp_midi_out.hex kernel7.img rp_midi_out.hex rp_midi_out.syms rp_midi_out.srec
endif

all : gcc

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.syms
	rm -f *.srec
	rm -f *.clang.opt.s

vectors.o : vectors.S
	$(ARMGNU)-gcc $(ASMOPS) -c vectors.S -o vectors.o

rp_midi_out.o : rp_midi_out.c
	$(ARMGNU)-gcc $(COPS) -c rp_midi_out.c -o rp_midi_out.o

ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1))
rp_midi_out.elf : memmap_rp01 vectors.o rp_midi_out.o
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
rp_midi_out.elf : memmap_rp23 vectors.o rp_midi_out.o
endif
ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1))
	$(ARMGNU)-ld vectors.o rp_midi_out.o -T memmap_rp01 -o rp_midi_out.elf
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
	$(ARMGNU)-ld vectors.o rp_midi_out.o -T memmap_rp23 -o rp_midi_out.elf
endif
	$(ARMGNU)-objdump -D rp_midi_out.elf > rp_midi_out.list

ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1))
kernel.img : rp_midi_out.elf
	$(ARMGNU)-objcopy rp_midi_out.elf -O binary kernel.img
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
kernel7.img : rp_midi_out.elf
	$(ARMGNU)-objcopy rp_midi_out.elf -O binary kernel7.img
endif

rp_midi_out.hex : rp_midi_out.elf
	$(ARMGNU)-objcopy rp_midi_out.elf -O ihex rp_midi_out.hex

rp_midi_out.syms : rp_midi_out.elf
	$(ARMGNU)-nm -n rp_midi_out.elf > rp_midi_out.syms

rp_midi_out.srec : rp_midi_out.elf
	$(ARMGNU)-objcopy rp_midi_out.elf -O srec rp_midi_out.srec

