MIDI OUT sample
===============

This is MIDI OUT sample code.  

This sample use UART 31250/8/N/1.  
Connect follow pins and registances to MIDI sound generator's MIDI IN.  
* pin 2 5V - 220 owm registance - DIN 5P 4 pin  
* pin 6 GND - DIN 5P 2 pin  
* Pin 8 TXD(GPIO14/ALT5) - 220 owm registance - DIN 5P 5 pin  

If you can run this program,  
You can hear loop of C major melody 'C D E F G A B C'.  

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
- ＰＩＣ　ＡＶＲ　工作室　ＵＡＲＴとＭＩＤＩの変換器 http://picavr.uunyan.com/avr_m_uart_midi.html  

