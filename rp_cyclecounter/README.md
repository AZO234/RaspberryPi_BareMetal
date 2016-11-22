ARM Cycle Counter sample
========================

This is ARM Cycle Counter sample code.  

This sample use UART 115200/8/N/1.  
Connect follow pins to PC.  
* pin 6 GND  
* Pin 8 TXD(GPIO14/ALT5)  
* Pin10 RXD(GPIO15/ALT5)  

If you can run this program,  
output CPU clock count per 1us(=MHz).  

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
- ARM TRM  

