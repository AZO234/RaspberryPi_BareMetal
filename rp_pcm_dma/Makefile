
ARMGNU ?= arm-none-eabi

COPS = -Wall -O0 -g3 -nostdlib -nostartfiles -ffreestanding
#COPS = -Wall -O3 -nostdlib -nostartfiles -ffreestanding
ASMOPS = -g3

#AZO_MEM_VFP = 1
ifeq ($(AZO_MEM_VFP),1)
COPS := $(COPS) -mfpu=vfp -DAZO_MEM_VFP
ASMOPS := $(ASMOPS) -mfpu=vfp --defsym AZO_MEM_VFP=1
endif

AZO_BOARD = RPZERO
#AZO_BOARD = RP1
#AZO_BOARD = RP2
#AZO_BOARD = RP3
ifeq ($(AZO_BOARD),RPZERO)
COPS := $(COPS) -DAZO_CHIP_BCM2835 -DAZO_BOARD_RPZERO
ASMOPS := $(ASMOPS) --defsym AZO_CHIP_BCM2835=1 --defsym AZO_BOARD_RPZERO=1
else ifeq ($(AZO_BOARD),RP1)
COPS := $(COPS) -DAZO_CHIP_BCM2835 -DAZO_BOARD_RP1
ASMOPS := $(ASMOPS) --defsym AZO_CHIP_BCM2835=1 --defsym AZO_BOARD_RP1=1
else ifeq ($(AZO_BOARD),RP2)
COPS := $(COPS) -DAZO_CHIP_BCM2836 -DAZO_BOARD_RP2
ASMOPS := $(ASMOPS) --defsym AZO_CHIP_BCM2836=1 --defsym AZO_BOARD_RP2=1
else ifeq ($(AZO_BOARD),RP3)
COPS := $(COPS) -DAZO_CHIP_BCM2837 -DAZO_BOARD_RP3
ASMOPS := $(ASMOPS) --defsym AZO_CHIP_BCM2837=1 --defsym AZO_BOARD_RP3=1
endif

ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
AZO_CPU_SINGLE = 1
ifeq ($(AZO_CPU_SINGLE),1)
COPS := $(COPS) -DAZO_CPU_SINGLE
ASMOPS := $(ASMOPS) --defsym AZO_CPU_SINGLE=1
endif
endif

ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1))
gcc : rp_pcm_dma.hex kernel.img rp_pcm_dma.hex rp_pcm_dma.syms rp_pcm_dma.srec
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
gcc : rp_pcm_dma.hex kernel7.img rp_pcm_dma.hex rp_pcm_dma.syms rp_pcm_dma.srec
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

rp_pcm_dma.o : rp_pcm_dma.c
	$(ARMGNU)-gcc $(COPS) -c rp_pcm_dma.c -o rp_pcm_dma.o

ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1))
rp_pcm_dma.elf : memmap_rp01 vectors.o rp_pcm_dma.o
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
rp_pcm_dma.elf : memmap_rp23 vectors.o rp_pcm_dma.o
endif
ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1))
	$(ARMGNU)-ld vectors.o rp_pcm_dma.o -T memmap_rp01 -o rp_pcm_dma.elf
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
	$(ARMGNU)-ld vectors.o rp_pcm_dma.o -T memmap_rp23 -o rp_pcm_dma.elf
endif
	$(ARMGNU)-objdump -D rp_pcm_dma.elf > rp_pcm_dma.list

ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1))
kernel.img : rp_pcm_dma.elf
	$(ARMGNU)-objcopy rp_pcm_dma.elf -O binary kernel.img
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
kernel7.img : rp_pcm_dma.elf
	$(ARMGNU)-objcopy rp_pcm_dma.elf -O binary kernel7.img
endif

rp_pcm_dma.hex : rp_pcm_dma.elf
	$(ARMGNU)-objcopy rp_pcm_dma.elf -O ihex rp_pcm_dma.hex

rp_pcm_dma.syms : rp_pcm_dma.elf
	$(ARMGNU)-nm -n rp_pcm_dma.elf > rp_pcm_dma.syms

rp_pcm_dma.srec : rp_pcm_dma.elf
	$(ARMGNU)-objcopy rp_pcm_dma.elf -O srec rp_pcm_dma.srec

