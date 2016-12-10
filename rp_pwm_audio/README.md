PWM audio sample
================

This is PWM audio sample code.  

Plug phone jack 3.5.  

If use Raspberry Pi Zero, connect forrow pins to phone.
- pin30 - GND
- pin32 - PWM0(GPIO12/ALT0)
- pin33 - PWM1(GPIO13/ALT0)

If you can run this program,  
you'll hear 1kHz(48kHz/13bit/stereo) sin-curve sound.  
More than 14bit, sound is strangely.  

-----

For Raspberry Pi 1/2/3 Bare Metal code by AZO.  

How to make:  
1. install "arm-none-eabi-gcc".  
2. Open "Makefile " with texteditor, and change options.  
BOARD=RP1/RP2/RP3  
3. make  
4. If successfully, output "kernel.img"(RP1) or "kernel7.img"(RP2/3).  

How to run:  
Put follow files to FAT SD card.  
* "bootcode.bin"  
* "start.elf"  
* "config.txt" &lt;- config_rp1.txt or config_rp23.txt  
* "kernel.img"(RP1) or "kernel7.img"(RP2/3)  

Firmware:  
Grub "bootcode.bin" and "start.elf" at  
https://github.com/raspberrypi/firmware/tree/master/boot  

Reference
---------
- PeterLemon/RaspberryPi: Raspberry Pi Bare Metal Assembly Programming  
https://github.com/PeterLemon/RaspberryPi  

