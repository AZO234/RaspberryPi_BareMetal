Raspberry Pi Zero/1/2/3 Bare Metal sample by AZO
================================================

For Raspberry Pi Zero/1/2/3 Bare Metal code by AZO(Tomohiro Yoshidomi).  
Twitter:@AZO234 E-Mail:typesylph@gmail.com  

How to make:  
1. install "arm-none-eabi-gcc".  
2. Open "Makefile " with texteditor, and change options.  
BOARD=RPZERO/RP1/RP2/RP3  
3. make  
4. If successfully, output "kernel.img"(RPZero/1) or "kernel7.img"(RP2/3).  

How to run:  
Put follow files to FAT SD card.  
* "bootcode.bin"  
* "start.elf"  
* "config.txt" &lt;- config_rp01.txt or config_rp23.txt  
* "kernel.img"(RPZero/1) or "kernel7.img"(RP2/3)  

Firmware:  
Grub "bootcode.bin" and "start.elf" at  
https://github.com/raspberrypi/firmware/tree/master/boot  

Reference
---------
- dwelch67/raspberrypi: Raspberry Pi ARM based bare metal examples  
https://github.com/dwelch67/raspberrypi  
- PeterLemon/RaspberryPi: Raspberry Pi Bare Metal Assembly Programming  
https://github.com/PeterLemon/RaspberryPi  

- CONFIG.TXT  
https://www.raspberrypi.org/documentation/configuration/config-txt.md  

- Peripheral specification  
https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2835/BCM2835-ARM-Peripherals.pdf  
- BCM2835 datasheet errata - eLinux.org  
http://elinux.org/BCM2835_datasheet_errata  
- ARM11 MPCore Processor Technical Reference Manual  
http://infocenter.arm.com/help/topic/com.arm.doc.ddi0360f/DDI0360F_arm11_mpcore_r2p0_trm.pdf  
- ARM1176JZF-S Technical Reference Manual  
http://infocenter.arm.com/help/topic/com.arm.doc.ddi0301h/ddi0301h_arm1176jzfs_r0p7_trm.pdf  
- SCHEMATICS RASPBERRY PI MODEL B+  
https://www.raspberrypi.org/documentation/hardware/raspberrypi/schematics/Raspberry-Pi-B-Plus-V1.2-Schematics.pdf  
- SCHEMATICS RASPBERRY PI 1 MODEL A AND B  
https://www.raspberrypi.org/documentation/hardware/raspberrypi/schematics/Raspberry-Pi-Rev-2.1-Model-AB-Schematics.pdf  
- MECHANICAL DRAWINGS RASPBERRY PI 1 MODEL B+  
https://www.raspberrypi.org/documentation/hardware/raspberrypi/mechanical/Raspberry-Pi-B-Plus-V1.2-Mechanical-Drawing.pdf  
- DPI (PARALLEL DISPLAY INTERFACE)  
https://www.raspberrypi.org/documentation/hardware/raspberrypi/dpi/README.md  
- CAMERA MODULE  
https://www.raspberrypi.org/documentation/hardware/camera/README.md  
- SMSC LAN9512  
http://ww1.microchip.com/downloads/en/DeviceDoc/9512.pdf  
- Raspberry Pi VideoCore APIs  
http://elinux.org/Raspberry_Pi_VideoCore_APIs  
- Github raspberrypi/firmware  
https://github.com/raspberrypi/firmware  
- Official Broadcom VideoCore IV 3D Architecture Reference  
http://www.broadcom.com/docs/support/videocore/VideoCoreIV-AG100-R.pdf  

- BCM2836 ARM-local peripherals  
https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2836/QA7_rev3.4.pdf  
- Cortex™-A7 MPCore™ Technical Reference Manual  
http://infocenter.arm.com/help/topic/com.arm.doc.ddi0464f/DDI0464F_cortex_a7_mpcore_r0p5_trm.pdf  
- Cortex™-A7 Floating-Point Unit Technical Reference Manual  
http://infocenter.arm.com/help/topic/com.arm.doc.ddi0463f/DDI0463F_cortex_a7_fpu_r0p5_trm.pdf  
- Cortex™-A7 NEON™ Media Processing Engine Technical Reference Manual  
http://infocenter.arm.com/help/topic/com.arm.doc.ddi0462f/DDI0462F_cortex_a7_neon_mpe_r0p5_trm.pdf  

- ARM® Cortex®-A53 MPCore Processor Technical Reference Manual  
http://infocenter.arm.com/help/topic/com.arm.doc.ddi0500g/DDI0500G_cortex_a53_trm.pdf  
- ARM® Cortex®-A53 MPCore Processor Advanced SIMD and Floating-point Extension Technical Reference Manual  
http://infocenter.arm.com/help/topic/com.arm.doc.ddi0502g/DDI0502G_cortex_a53_fpu_trm.pdf  
- ARM® Cortex®-A53 MPCore Processor Cryptography Extension Technical Reference Manual  
http://infocenter.arm.com/help/topic/com.arm.doc.ddi0501f/DDI0501F_cortex_a53_cryptography_trm.pdf  
- SCHEMATICS RASPBERRY PI 3 MODEL B  
https://www.raspberrypi.org/documentation/hardware/raspberrypi/schematics/RPI-3B-V1_2-SCHEMATIC-REDUCED.pdf  
- MECHANICAL DRAWINGS RASPBERRY PI 3  
https://www.raspberrypi.org/documentation/hardware/raspberrypi/mechanical/RPI-3B-V1_2.pdf  

- MECHANICAL DRAWINGS RASPBERRY PI ZERO (V1.2)  
https://www.raspberrypi.org/documentation/hardware/raspberrypi/mechanical/rpi-zero-v1_2_dimensions.pdf  

