
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
gcc : rp_spi0_psxpad.hex kernel.img rp_spi0_psxpad.hex rp_spi0_psxpad.syms rp_spi0_psxpad.srec
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
gcc : rp_spi0_psxpad.hex kernel7.img rp_spi0_psxpad.hex rp_spi0_psxpad.syms rp_spi0_psxpad.srec
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

rp_spi0_psxpad.o : rp_spi0_psxpad.c
	$(ARMGNU)-gcc $(COPS) -c rp_spi0_psxpad.c -o rp_spi0_psxpad.o

ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1))
rp_spi0_psxpad.elf : memmap_rp01 vectors.o rp_spi0_psxpad.o
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
rp_spi0_psxpad.elf : memmap_rp23 vectors.o rp_spi0_psxpad.o
endif
ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1))
	$(ARMGNU)-ld vectors.o rp_spi0_psxpad.o -T memmap_rp01 -o rp_spi0_psxpad.elf
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
	$(ARMGNU)-ld vectors.o rp_spi0_psxpad.o -T memmap_rp23 -o rp_spi0_psxpad.elf
endif
	$(ARMGNU)-objdump -D rp_spi0_psxpad.elf > rp_spi0_psxpad.list

ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1))
kernel.img : rp_spi0_psxpad.elf
	$(ARMGNU)-objcopy rp_spi0_psxpad.elf -O binary kernel.img
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
kernel7.img : rp_spi0_psxpad.elf
	$(ARMGNU)-objcopy rp_spi0_psxpad.elf -O binary kernel7.img
endif

rp_spi0_psxpad.hex : rp_spi0_psxpad.elf
	$(ARMGNU)-objcopy rp_spi0_psxpad.elf -O ihex rp_spi0_psxpad.hex

rp_spi0_psxpad.syms : rp_spi0_psxpad.elf
	$(ARMGNU)-nm -n rp_spi0_psxpad.elf > rp_spi0_psxpad.syms

rp_spi0_psxpad.srec : rp_spi0_psxpad.elf
	$(ARMGNU)-objcopy rp_spi0_psxpad.elf -O srec rp_spi0_psxpad.srec

