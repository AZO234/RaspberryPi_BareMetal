
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
gcc : rp_pcm_dma_rec.hex kernel.img rp_pcm_dma_rec.hex rp_pcm_dma_rec.syms rp_pcm_dma_rec.srec
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
gcc : rp_pcm_dma_rec.hex kernel7.img rp_pcm_dma_rec.hex rp_pcm_dma_rec.syms rp_pcm_dma_rec.srec
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

rp_pcm_dma_rec.o : rp_pcm_dma_rec.c
	$(ARMGNU)-gcc $(COPS) -c rp_pcm_dma_rec.c -o rp_pcm_dma_rec.o

ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1))
rp_pcm_dma_rec.elf : memmap_rp01 vectors.o rp_pcm_dma_rec.o
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
rp_pcm_dma_rec.elf : memmap_rp23 vectors.o rp_pcm_dma_rec.o
endif
ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1))
	$(ARMGNU)-ld vectors.o rp_pcm_dma_rec.o -T memmap_rp01 -o rp_pcm_dma_rec.elf
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
	$(ARMGNU)-ld vectors.o rp_pcm_dma_rec.o -T memmap_rp23 -o rp_pcm_dma_rec.elf
endif
	$(ARMGNU)-objdump -D rp_pcm_dma_rec.elf > rp_pcm_dma_rec.list

ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RPZERO RP1))
kernel.img : rp_pcm_dma_rec.elf
	$(ARMGNU)-objcopy rp_pcm_dma_rec.elf -O binary kernel.img
else ifeq ($(AZO_BOARD),$(filter $(AZO_BOARD),RP2 RP3))
kernel7.img : rp_pcm_dma_rec.elf
	$(ARMGNU)-objcopy rp_pcm_dma_rec.elf -O binary kernel7.img
endif

rp_pcm_dma_rec.hex : rp_pcm_dma_rec.elf
	$(ARMGNU)-objcopy rp_pcm_dma_rec.elf -O ihex rp_pcm_dma_rec.hex

rp_pcm_dma_rec.syms : rp_pcm_dma_rec.elf
	$(ARMGNU)-nm -n rp_pcm_dma_rec.elf > rp_pcm_dma_rec.syms

rp_pcm_dma_rec.srec : rp_pcm_dma_rec.elf
	$(ARMGNU)-objcopy rp_pcm_dma_rec.elf -O srec rp_pcm_dma_rec.srec

