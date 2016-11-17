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
"bootcode.bin"  
"start.elf"  
"config.txt" &lt;- config_01.txt or config_23.txt  
"kernel.img"(RPZero/1) or "kernel7.img"(RP2/3)  

Firmware:  
Grub "bootcode.bin" and "start.elf" at  
https://github.com/raspberrypi/firmware/tree/master/boot  

Reference
---------
- dwelch67/raspberrypi: Raspberry Pi ARM based bare metal examples https://github.com/dwelch67/raspberrypi  
- PeterLemon/RaspberryPi: Raspberry Pi Bare Metal Assembly Programming https://github.com/PeterLemon/RaspberryPi  

