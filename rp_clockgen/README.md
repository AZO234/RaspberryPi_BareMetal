Clock generator sample
======================

Clock generator sample code.  

Connect follow pins to external devices.  
* pin 7 GPCLK0(GPIO4/ALT0)  
* pin31 GPCLK2(GPIO6/ALT0)  

If you can run this program,  
RPi can supply clock to external device.  

Don't use GPCLK1 (it's probably used for the Ethernet clock).  

-----

For Raspberry Pi Zero/1/2/3 Bare Metal code by AZO.  

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
- Peripheral specification  
https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2835/BCM2835-ARM-Peripherals.pdf  

