
ARMGNU ?= arm-none-eabi

COMMONOPS = -Wall -nostdlib -nostartfiles -ffreestanding

#AZO_MEM_VFP = 1

#AZO_BOARD = RPZERO
#AZO_BOARD = RP1
#AZO_BOARD = RP2
AZO_BOARD = RP3
#AZO_BOARD = RP4
ifeq ($(AZO_BOARD),RPZERO)
COMMONOPS := $(COMMONOPS) -march=armv6zk -mcpu=arm1176jzf-s -mtune=arm1176jzf-s -fpic -ffreestanding -DAZO_CHIP_BCM2835 -DAZO_BOARD_RPZERO
ifeq ($(AZO_MEM_VFP),1)
COMMONOPS := $(COMMONOPS) -mfloat-abi=hard -mfpu=vfp -DAZO_MEM_VFP
endif
else ifeq ($(AZO_BOARD),RP1)
COMMONOPS := $(COMMONOPS) -march=armv6zk -mcpu=arm1176jzf-s -mtune=arm1176jzf-s -fpic -ffreestanding -DAZO_CHIP_BCM2835 -DAZO_BOARD_RP1
ifeq ($(AZO_MEM_VFP),1)
COMMONOPS := $(COMMONOPS) -mfloat-abi=hard -mfpu=vfp -DAZO_MEM_VFP
endif
else ifeq ($(AZO_BOARD),RP2)
COMMONOPS := $(COMMONOPS) -mcpu=cortex-a7 -fpic -ffreestanding -DAZO_CHIP_BCM2836 -DAZO_BOARD_RP2
ifeq ($(AZO_MEM_VFP),1)
COMMONOPS := $(COMMONOPS) -mfpu=neon-vfpv4 -mfloat-abi=hard -DAZO_MEM_VFP
endif
else ifeq ($(AZO_BOARD),RP3)
COMMONOPS := $(COMMONOPS) -march=armv8-a -mtune=cortex-a53 -DAZO_CHIP_BCM2837 -DAZO_BOARD_RP3
ifeq ($(AZO_MEM_VFP),1)
COMMONOPS := $(COMMONOPS) -mfpu=neon-fp-armv8 -mfloat-abi=hard -DAZO_MEM_VFP
endif
else ifeq ($(AZO_BOARD),RP4)
COMMONOPS := $(COMMONOPS) -mcpu=cortex-a72 -mtune=cortex-a72 -DAZO_CHIP_BCM2711 -DAZO_BOARD_RP4
ifeq ($(AZO_MEM_VFP),1)
COMMONOPS := $(COMMONOPS) -mfpu=neon-fp-armv8 -mfloat-abi=hard -DAZO_MEM_VFP
endif
endif

ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3 RP4))
AZO_CPU_SINGLE = 1
ifeq ($(AZO_CPU_SINGLE),1)
COMMONOPS := $(COMMONOPS) -DAZO_CPU_SINGLE
endif
endif

COPS := -O0 -g3 $(COMMONOPS)
#COPS = -O3 $(COMMONOPS)
ASMOPS := -g3 $(COMMONOPS)

ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1))
gcc : rp_framebuffer.hex kernel.img rp_framebuffer.hex rp_framebuffer.syms rp_framebuffer.srec
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3 RP4))
gcc : rp_framebuffer.hex kernel7.img rp_framebuffer.hex rp_framebuffer.syms rp_framebuffer.srec
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

rp_framebuffer.o : rp_framebuffer.c
	$(ARMGNU)-gcc $(COPS) -c rp_framebuffer.c -o rp_framebuffer.o

ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1))
rp_framebuffer.elf : memmap_rp01 vectors.o rp_framebuffer.o
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3 RP4))
rp_framebuffer.elf : memmap_rp23 vectors.o rp_framebuffer.o
endif
ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1))
	$(ARMGNU)-ld vectors.o rp_framebuffer.o -T memmap_rp01 -o rp_framebuffer.elf
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3 RP4))
	$(ARMGNU)-ld vectors.o rp_framebuffer.o -T memmap_rp23 -o rp_framebuffer.elf
endif
	$(ARMGNU)-objdump -D rp_framebuffer.elf > rp_framebuffer.list

ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1))
kernel.img : rp_framebuffer.elf
	$(ARMGNU)-objcopy rp_framebuffer.elf -O binary kernel.img
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3 RP4))
kernel7.img : rp_framebuffer.elf
	$(ARMGNU)-objcopy rp_framebuffer.elf -O binary kernel7.img
endif

rp_framebuffer.hex : rp_framebuffer.elf
	$(ARMGNU)-objcopy rp_framebuffer.elf -O ihex rp_framebuffer.hex

rp_framebuffer.syms : rp_framebuffer.elf
	$(ARMGNU)-nm -n rp_framebuffer.elf > rp_framebuffer.syms

rp_framebuffer.srec : rp_framebuffer.elf
	$(ARMGNU)-objcopy rp_framebuffer.elf -O srec rp_framebuffer.srec

