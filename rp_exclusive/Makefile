
ARMGNU ?= arm-none-eabi

COPS = -Wall -O0 -g3 -nostdlib -nostartfiles -ffreestanding
#COPS = -Wall -O3 -nostdlib -nostartfiles -ffreestanding
ASMOPS = -g3

#AZO_MEM_VFP = 1
ifeq ($(AZO_MEM_VFP),1)
COPS := $(COPS) -mfpu=vfp -DAZO_MEM_VFP
ASMOPS := $(ASMOPS) -mfpu=vfp --defsym AZO_MEM_VFP=1
endif

#AZO_BOARD = RP2
AZO_BOARD = RP3
ifeq ($(AZO_BOARD),RP2)
COPS := $(COPS) -DAZO_CHIP_BCM2836 -DAZO_BOARD_RP2
ASMOPS := $(ASMOPS) --defsym AZO_CHIP_BCM2836=1 --defsym AZO_BOARD_RP2=1
else ifeq ($(AZO_BOARD),RP3)
COPS := $(COPS) -DAZO_CHIP_BCM2837 -DAZO_BOARD_RP3
ASMOPS := $(ASMOPS) --defsym AZO_CHIP_BCM2837=1 --defsym AZO_BOARD_RP3=1
endif

ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1))
gcc : rp_exclusive.hex kernel.img rp_exclusive.hex rp_exclusive.syms rp_exclusive.srec
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
gcc : rp_exclusive.hex kernel7.img rp_exclusive.hex rp_exclusive.syms rp_exclusive.srec
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
	$(ARMGNU)-as $(ASMOPS) vectors.S -o vectors.o

rp_exclusive.o : rp_exclusive.c
	$(ARMGNU)-gcc $(COPS) -c rp_exclusive.c -o rp_exclusive.o

rp_exclusive.elf : memmap vectors.o rp_exclusive.o
	$(ARMGNU)-ld vectors.o rp_exclusive.o -T memmap -o rp_exclusive.elf
	$(ARMGNU)-objdump -D rp_exclusive.elf > rp_exclusive.list

kernel7.img : rp_exclusive.elf
	$(ARMGNU)-objcopy rp_exclusive.elf -O binary kernel7.img

rp_exclusive.hex : rp_exclusive.elf
	$(ARMGNU)-objcopy rp_exclusive.elf -O ihex rp_exclusive.hex

rp_exclusive.syms : rp_exclusive.elf
	$(ARMGNU)-nm -n rp_exclusive.elf > rp_exclusive.syms

rp_exclusive.srec : rp_exclusive.elf
	$(ARMGNU)-objcopy rp_exclusive.elf -O srec rp_exclusive.srec

