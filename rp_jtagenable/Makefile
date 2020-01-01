AZO_BOARD = OLDRP1
#AZO_BOARD = RPZERO
#AZO_BOARD = RP1
#AZO_BOARD = RP2
#AZO_BOARD = RP2_64
#AZO_BOARD = RP3
#AZO_BOARD = RP3_64
#AZO_BOARD = RP4
#AZO_BOARD = RP4_64

ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),OLDRP1 RPZERO RP1 RP2 RP3 RP4))
ARMGNU ?= arm-none-eabi
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2_64 RP3_64 RP4_64))
ARMGNU ?= aarch64-linux-gnu
endif

ASMOPS = -g3

ifeq ($(AZO_BOARD),RPZERO)
ASMOPS := $(ASMOPS) -DAZO_CHIP_BCM2835 -DAZO_BOARD_RPZERO
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP1 OLDRP1))
ASMOPS := $(ASMOPS) -DAZO_CHIP_BCM2835 -DAZO_BOARD_RP1
else ifeq ($(AZO_BOARD),RP2)
ASMOPS := $(ASMOPS) -DAZO_CHIP_BCM2836 -DAZO_BOARD_RP2
else ifeq ($(AZO_BOARD),RP3)
ASMOPS := $(ASMOPS) -DAZO_CHIP_BCM2837 -DAZO_BOARD_RP3
else ifeq ($(AZO_BOARD),RP3_64)
ASMOPS := $(ASMOPS) -DAZO_CHIP_BCM2837 -DAZO_BOARD_RP3_64
else ifeq ($(AZO_BOARD),RP4)
ASMOPS := $(ASMOPS) -DAZO_CHIP_BCM2711 -DAZO_BOARD_RP4
else ifeq ($(AZO_BOARD),RP4_64)
ASMOPS := $(ASMOPS) -DAZO_CHIP_BCM2711 -DAZO_BOARD_RP4_64
endif

ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),OLDRP1 RPZERO RP1))
gcc : kernel.img rp_jtagenable.hex rp_jtagenable.syms rp_jtagenable.srec
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
gcc : kernel7.img rp_jtagenable.hex rp_jtagenable.syms rp_jtagenable.srec
else ifeq ($(AZO_BOARD),RP4)
gcc : kernel7l.img rp_jtagenable.hex rp_jtagenable.syms rp_jtagenable.srec
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2_64 RP3_64 RP4_64))
gcc : kernel8.img rp_jtagenable.hex rp_jtagenable.syms rp_jtagenable.srec
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

rp_jtagenable_32.o : rp_jtagenable_32.S
	$(ARMGNU)-gcc $(ASMOPS) -c rp_jtagenable_32.S -o rp_jtagenable_32.o
rp_jtagenable_32_oldrp1.o : rp_jtagenable_32_oldrp1.S
	$(ARMGNU)-gcc $(ASMOPS) -c rp_jtagenable_32_oldrp1.S -o rp_jtagenable_32_oldrp1.o
rp_jtagenable_64.o : rp_jtagenable_64.S
	$(ARMGNU)-gcc $(ASMOPS) -c rp_jtagenable_64.S -o rp_jtagenable_64.o

ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1))
rp_jtagenable.elf : memmap rp_jtagenable_32.o
else ifeq ($(AZO_BOARD),OLDRP1)
rp_jtagenable.elf : memmap rp_jtagenable_32_oldrp1.o
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3 RP4))
rp_jtagenable.elf : memmap rp_jtagenable_32.o
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2_64 RP3_64 RP4_64))
rp_jtagenable.elf : memmap rp_jtagenable_64.o
endif
ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1))
	$(ARMGNU)-ld rp_jtagenable_32.o -T memmap -o rp_jtagenable.elf
else ifeq ($(AZO_BOARD),OLDRP1)
	$(ARMGNU)-ld rp_jtagenable_32_oldrp1.o -T memmap -o rp_jtagenable.elf
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3 RP4))
	$(ARMGNU)-ld rp_jtagenable_32.o -T memmap -o rp_jtagenable.elf
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2_64 RP3_64 RP4_64))
	$(ARMGNU)-ld rp_jtagenable_64.o -T memmap -o rp_jtagenable.elf
endif
	$(ARMGNU)-objdump -D rp_jtagenable.elf > rp_jtagenable.list

ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1 OLDRP1))
kernel.img : rp_jtagenable.elf
	$(ARMGNU)-objcopy rp_jtagenable.elf -O binary kernel.img
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
kernel7.img : rp_jtagenable.elf
	$(ARMGNU)-objcopy rp_jtagenable.elf -O binary kernel7.img
else ifeq ($(AZO_BOARD),RP4)
kernel7l.img : rp_jtagenable.elf
	$(ARMGNU)-objcopy rp_jtagenable.elf -O binary kernel7l.img
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2_64 RP3_64 RP4_64))
kernel8.img : rp_jtagenable.elf
	$(ARMGNU)-objcopy rp_jtagenable.elf -O binary kernel8.img
endif

rp_jtagenable.hex : rp_jtagenable.elf
	$(ARMGNU)-objcopy rp_jtagenable.elf -O ihex rp_jtagenable.hex

rp_jtagenable.syms : rp_jtagenable.elf
	$(ARMGNU)-nm -n rp_jtagenable.elf > rp_jtagenable.syms

rp_jtagenable.srec : rp_jtagenable.elf
	$(ARMGNU)-objcopy rp_jtagenable.elf -O srec rp_jtagenable.srec

