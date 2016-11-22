
AZO_BOARD = RPZERO
#AZO_BOARD = RP1
#AZO_BOARD = RP1OLD
#AZO_BOARD = RP2
#AZO_BOARD = RP3
#AZO_BOARD = RP3_64

ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1 RP1OLD RP2 RP3))
ARMGNU ?= arm-none-eabi
else ifeq ($(AZO_BOARD),RP3_64)
ARMGNU ?= aarch64-linux-gnu
endif

ASMOPS = -g3

ifeq ($(AZO_BOARD),RPZERO)
ASMOPS := $(ASMOPS) -DAZO_CHIP_BCM2835 -DAZO_BOARD_RPZERO
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP1 RP1OLD))
ASMOPS := $(ASMOPS) -DAZO_CHIP_BCM2835 -DAZO_BOARD_RP1
else ifeq ($(AZO_BOARD),RP2)
ASMOPS := $(ASMOPS) -DAZO_CHIP_BCM2836 -DAZO_BOARD_RP2
else ifeq ($(AZO_BOARD),RP3)
ASMOPS := $(ASMOPS) -DAZO_CHIP_BCM2837 -DAZO_BOARD_RP3
else ifeq ($(AZO_BOARD),RP3_64)
ASMOPS := $(ASMOPS) -DAZO_CHIP_BCM2837 -DAZO_BOARD_RP3_64
endif

ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1 RP1OLD))
gcc : rp_jtagenable.hex kernel.img rp_jtagenable.hex rp_jtagenable.syms rp_jtagenable.srec
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
gcc : rp_jtagenable.hex kernel7.img rp_jtagenable.hex rp_jtagenable.syms rp_jtagenable.srec
else ifeq ($(AZO_BOARD),RP3_64)
gcc : rp_jtagenable.hex kernel8.img rp_jtagenable.hex rp_jtagenable.syms rp_jtagenable.srec
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

rp_jtagenable.o : rp_jtagenable.S
	$(ARMGNU)-gcc $(ASMOPS) -c rp_jtagenable.S -o rp_jtagenable.o
rp_jtagenable_old.o : rp_jtagenable_old.S
	$(ARMGNU)-gcc $(ASMOPS) -c rp_jtagenable_old.S -o rp_jtagenable_old.o
rp_jtagenable_64.o : rp_jtagenable_64.S
	$(ARMGNU)-gcc $(ASMOPS) -c rp_jtagenable_64.S -o rp_jtagenable_64.o

ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1))
rp_jtagenable.elf : memmap_rp01 rp_jtagenable.o
else ifeq ($(AZO_BOARD),RP1OLD)
rp_jtagenable.elf : memmap_rp01 rp_jtagenable_old.o
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
rp_jtagenable.elf : memmap_rp23 rp_jtagenable.o
else ifeq ($(AZO_BOARD),RP3_64)
rp_jtagenable.elf : memmap_rp23 rp_jtagenable_64.o
endif
ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1))
	$(ARMGNU)-ld rp_jtagenable.o -T memmap_rp01 -o rp_jtagenable.elf
else ifeq ($(AZO_BOARD),RP1OLD)
	$(ARMGNU)-ld rp_jtagenable_old.o -T memmap_rp01 -o rp_jtagenable.elf
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
	$(ARMGNU)-ld rp_jtagenable.o -T memmap_rp23 -o rp_jtagenable.elf
else ifeq ($(AZO_BOARD),RP3_64)
	$(ARMGNU)-ld rp_jtagenable_64.o -T memmap_rp23 -o rp_jtagenable.elf
endif
	$(ARMGNU)-objdump -D rp_jtagenable.elf > rp_jtagenable.list

ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1 RP1OLD))
kernel.img : rp_jtagenable.elf
	$(ARMGNU)-objcopy rp_jtagenable.elf -O binary kernel.img
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
kernel7.img : rp_jtagenable.elf
	$(ARMGNU)-objcopy rp_jtagenable.elf -O binary kernel7.img
else ifeq ($(AZO_BOARD),RP3_64)
kernel8.img : rp_jtagenable.elf
	$(ARMGNU)-objcopy rp_jtagenable.elf -O binary kernel8.img
endif

rp_jtagenable.hex : rp_jtagenable.elf
	$(ARMGNU)-objcopy rp_jtagenable.elf -O ihex rp_jtagenable.hex

rp_jtagenable.syms : rp_jtagenable.elf
	$(ARMGNU)-nm -n rp_jtagenable.elf > rp_jtagenable.syms

rp_jtagenable.srec : rp_jtagenable.elf
	$(ARMGNU)-objcopy rp_jtagenable.elf -O srec rp_jtagenable.srec

