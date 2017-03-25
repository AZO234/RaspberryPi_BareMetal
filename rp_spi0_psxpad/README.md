PSX(PS1/PS2) pad for SPI0 sample
================================

This is PSX(PS1/PS2) pad for SPI0 sample code.  

PSX(PS1/2) controler pins (not PSX's socket, pad side)  
 123 456 789  
(...|...|...)  

Connect follow pad pins to RPi.  
* Pin1 DAT -> Pin19 MOSI(GPIO10/ALT0)  
* Pin2 CMD -> Pin21 MOSI(GPIO 9/ALT0)  
* Pin3 9V (for motor, If you not necessary NC)  
* Pin4 GND -> Pin 9 GND  
* Pin5 3.3V -> Pin 1 3.3V  
* Pin6 Attention -> Pin24 CE0_N(GPIO 8/ALT0)  
* Pin7 SCK -> Pin23 SCLK(GPIO11/ALT0)  
* Pin8 N.C.  
* Pin9 ACK -> N.C.  

This sample use UART 115200/8/N/1.  
Connect follow pins to PC.  
* Pin 6 GND  
* Pin 8 TXD(GPIO14/ALT5)  
* Pin10 RXD(GPIO15/ALT5)  

If you can run this program,  
display pressed pad buttons.  

To Do
----
- Get analog value button pressure
- Rumble motors

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
- dwelch67/raspberrypi: Raspberry Pi ARM based bare metal examples https://github.com/dwelch67/raspberrypi  
- Interfacing a PS2 (PlayStation 2) Controller - CuriousInventor Tutorials http://store.curiousinventor.com/guides/PS2/  
