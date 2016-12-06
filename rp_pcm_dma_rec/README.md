PCM(I2S) record sample
======================

This is PCM/I2S record sample code.  
You need any external I2S DAC board or Raspberry Pi.  

Connect I2S DAC board to below pins.  
* pin12 - PCM_CLK  (GPIO18/ALT0), BCK/BCLK/BITCLK  
* pin35 - PCM_FR   (GPIO19/ALT0), LRCK  
* pin38 - PCM_DIN  (GPIO20/ALT0), Data  
You can use VCC and GND.  

This sample use UART 115200/8/N/1.  
Connect follow pins to PC.  
* pin 6 GND  
* Pin 8 TXD(GPIO14/ALT5)  
* Pin10 RXD(GPIO15/ALT5)  

If you can run this program,  
First ready prompt to record to terminal.
You push space key, start recording.
Recording is done, then, prompt to playback to terminal.
You push space key, start playback to PWM speaker.

This sample can recode 768kHz/32bit/stereo sound.  

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
- I²S - Wikipedia https://en.wikipedia.org/wiki/I%C2%B2S  
- 04 « 4月 « 2008 « PCオーディオブログ http://blog.ratocsystems.com/wp/2008/04/04/19/  


