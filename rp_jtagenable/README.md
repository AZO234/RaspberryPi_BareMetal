JTAG enable
===========

This program makes Raspberry Pi's ARM JTAG enable to degub.  
You need any external JTAG interface probe.  

Connect JTAG interface to below pins.  
- pin15 - nTRST(GPIO22/ALT4)
- pin18 - TDO/DI(GPIO24/ALT4)
- pin22 - TCK/SK(GPIO25/ALT4)
- pin37 - TDI/DO(GPIO26/ALT4), or old RP1 pin7 - TDI/DO(GPIO04/ALT5)
- pin13 - TMS/CS(GPIO27/ALT4)
- pin20 - GND(Ground)

Connect JTAG interface, and run this program, then start debugger.

-----

How to
------
For Raspberry Pi Zero/1/2/2_64/3/3_64/4/4_64) Bare Metal code by AZO.  

How to make:  
1. install "arm-none-eabi-gcc" or "gcc-aarch64-linux-gnu"(RP2_64/RP3_64/RP4_64).  
2. Open "Makefile " with texteditor, and change options.  
BOARD=OLDRPI/RPZERO/RP1/RP2/RP3/RP3_64/RP4/RP4_64  
3. make  
4. If successfully, output "kernel.img"(RPold1/Zero/1) or "kernel7.img"(RP2/3) or "kernel7l.img"(RP4) or "kernel8.img"(RP2_64/RP3_64/RP4_64).  

How to run:  
Put follow files to FAT SD card.  
* "bootcode.bin"  
* "start.elf"  
* "fixup.dat"  
* "config.txt" &lt;- config_32bit.txt or config_64bit.txt.  
* "kernel.img"(RPold1/Zero/1) or "kernel7.img"(RP2/3) or "kernel7l.img"(RP4) or "kernel8.img"(RP2_64/RP3_64/RP4_64)  

Firmware:  
Grub "bootcode.bin" and "start.elf" and "fixup.dat" at  
https://github.com/raspberrypi/firmware/tree/master/boot  

-----

OpenOCD
-------
Use newest OpenOCD from repository.  

You can make OpenOCD, include current implemantion for ARMv8.  
```shell
git clone https://git.code.sf.net/p/openocd/code openocd-code  
cd openocd-code  
./bootstrap.sh
./configure --enable-ftdi --enable-bcm2835gpio  
make -j4  
sudo make install
```
and  
```shell  
sudo -s  
cp cfg/bcm*.cfg /usr/local/share/openocd/scripts/board/  
exit
```

When debug, run follow command.
```shell
openocd -f interface/yourinterface.cfg -f board/bcm2835.cfg  
```

You can use GDB, or  
Eclipce + OpenOCD debugging Eclipse plug-in.  
http://gnuarmeclipse.github.io/debug/openocd/  

--- But, I don't success RP4 connection. Please tell me good method :(  

-----

Referrence
----------
- Boot options in config.txt - Raspberry Pi Documentation https://www.raspberrypi.org/documentation/configuration/config-txt/boot.md  
- Peripheral specification https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2835/BCM2835-ARM-Peripherals.pdf  
- dwelch67/raspberrypi: Raspberry Pi ARM based bare metal examples https://github.com/dwelch67/raspberrypi  
- documentation/README.md at master · 96boards/documentation https://github.com/96boards/documentation/blob/master/ConsumerEdition/HiKey/Configuration/JTAG/README.md  
- Olimex ARM-USB-OCD-H / openocd-0.9.0 / libftd2xx1.1.12 / Raspberry Pi 2 / Ubuntu 15.04 https://gist.github.com/tonosaman/62a31e7991a41edb19c5  
- Raspberry Pi 4 Bringup - Raspberry Pi https://xihan94.gitbook.io/raspberry-pi/raspberry-pi-4-bringup  

