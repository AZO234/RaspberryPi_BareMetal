
ARMGNU ?= arm-none-eabi

COMMONOPS = -Wall -nostdlib -nostartfiles -ffreestanding

#AZO_MEM_VFP = 1
ifeq ($(AZO_MEM_VFP),1)
COMMONOPS := $(COMMONOPS) -mfpu=vfp -DAZO_MEM_VFP
endif

#AZO_BOARD = RP2
AZO_BOARD = RP3
ifeq ($(AZO_BOARD),RP2)
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
	$(ARMGNU)-gcc $(ASMOPS) -c vectors.S -o vectors.o

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

