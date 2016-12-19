TFM-1010(AN1010) FM radio I2C sample
====================================

This is TFM-1010(AN1010) FM radio I2C sample code.  

Connect follow pins to TFM-1010.  
* pin 1 3.3V - pin 6 VCC  
* pin 3 SDA1(GPIO2/ALT0) - pin 10 Data  
* pin 5 SCL1(GPIO3/ALT0) - pin 9 Data  
* pin 9 GND - pin 5 GND &amp; pin 8 BUSMODE  

Connect TFM-1010 follow pins.  
* pin 1 ANT - long long cable  
* pin 3 L-Out - phone  
* pin 4 R-Out - phone  
* pin 5 GND - phone  

This sample use UART 115200/8/N/1.  
Connect follow pins to PC.  
* pin 6 GND  
* Pin 8 TXD(GPIO14/ALT5)  
* Pin10 RXD(GPIO15/ALT5)  

If you can run this program,  
Display menu to terminal.  
1. Hit '1' to send array values.  
2. Hit '5' to begin seek.  
If tune successful, you can hear radio program.  

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
- SPK-TFM-1010 SPECIFICATION.pdf https://strawberry-linux.com/pub/SPK-TFM-1010%20SPECIFICATION.pdf  
- https://strawberry-linux.com/pub/AR1000ATMega168v11.zip  

