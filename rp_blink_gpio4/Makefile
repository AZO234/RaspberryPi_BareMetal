
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
else ifeq ($(AZO_BOARD),RPZEROW)
COMMONOPS := $(COMMONOPS) -DAZO_CHIP_BCM2835 -DAZO_BOARD_RPZEROW
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
gcc : rp_blink_gpio4.hex kernel.img rp_blink_gpio4.hex rp_blink_gpio4.syms rp_blink_gpio4.srec
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
gcc : rp_blink_gpio4.hex kernel7.img rp_blink_gpio4.hex rp_blink_gpio4.syms rp_blink_gpio4.srec
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

rp_blink_gpio4.o : rp_blink_gpio4.c
	$(ARMGNU)-gcc $(COPS) -c rp_blink_gpio4.c -o rp_blink_gpio4.o

ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1))
rp_blink_gpio4.elf : memmap_rp01 vectors.o rp_blink_gpio4.o
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
rp_blink_gpio4.elf : memmap_rp23 vectors.o rp_blink_gpio4.o
endif
ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1))
	$(ARMGNU)-ld vectors.o rp_blink_gpio4.o -T memmap_rp01 -o rp_blink_gpio4.elf
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
	$(ARMGNU)-ld vectors.o rp_blink_gpio4.o -T memmap_rp23 -o rp_blink_gpio4.elf
endif
	$(ARMGNU)-objdump -D rp_blink_gpio4.elf > rp_blink_gpio4.list

ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1))
kernel.img : rp_blink_gpio4.elf
	$(ARMGNU)-objcopy rp_blink_gpio4.elf -O binary kernel.img
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
kernel7.img : rp_blink_gpio4.elf
	$(ARMGNU)-objcopy rp_blink_gpio4.elf -O binary kernel7.img
endif

rp_blink_gpio4.hex : rp_blink_gpio4.elf
	$(ARMGNU)-objcopy rp_blink_gpio4.elf -O ihex rp_blink_gpio4.hex

rp_blink_gpio4.syms : rp_blink_gpio4.elf
	$(ARMGNU)-nm -n rp_blink_gpio4.elf > rp_blink_gpio4.syms

rp_blink_gpio4.srec : rp_blink_gpio4.elf
	$(ARMGNU)-objcopy rp_blink_gpio4.elf -O srec rp_blink_gpio4.srec

