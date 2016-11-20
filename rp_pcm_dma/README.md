PCM(I2S) + DMA sample
=====================

This is PCM/I2S with DMA sample code.  
(You connot hear 3.5 phone jack.)  
You need any external I2S DAC board.  

Connect I2S DAC board to below pins.  
* pin12 - PCM_CLK  (GPIO18/ALT0), BCK/BCLK/BITCLK  
* pin35 - PCM_FR   (GPIO19/ALT0), LRCK  
* pin40 - PCM_DOUT (GPIO21/ALT0), Data  
You can use VCC and GND.  

If you can run this program,  
you'll hear 1kHz(44.1kHz/16bit/stereo) sin-curve sound.  

This sample can play 384kHz/32bit/stereo sound.  
I used Pimoroni's "pHAT DAC".  
https://shop.pimoroni.com/products/phat-dac

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
* "config.txt" &lt;- config_01.txt or config_23.txt  
* "kernel.img"(RPZero/1) or "kernel7.img"(RP2/3)  

Firmware:  
Grub "bootcode.bin" and "start.elf" at  
https://github.com/raspberrypi/firmware/tree/master/boot  

Reference
---------
* I²S - Wikipedia https://en.wikipedia.org/wiki/I%C2%B2S  
* 04 « 4月 « 2008 « PCオーディオブログ http://blog.ratocsystems.com/wp/2008/04/04/19/  


