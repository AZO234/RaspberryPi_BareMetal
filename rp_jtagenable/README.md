JTAG enable
===========

This program makes Raspberry Pi's ARM JTAG enable to degub.  
You need any external JTAG interface probe.  

Connect JTAG interface to below pins.  
- pin17 - VREF(3.3V DC Power)
- pin15 - TRSTn(GPIO22/ALT4)
- pin16 - RTCK(GPIO23/ALT4)
- pin18 - TDO(GPIO24/ALT4)
- pin22 - TCK(GPIO25/ALT4)
- pin37 - TDI(GPIO26/ALT4), or old RP1 pin7 - TDI(GPIO04/ALT5)
- pin13 - TMS(GPIO27/ALT4)
- pin20 - GND(Ground)

Connect JTAG interface, and run this program, then start debugger.

-----

How to
------
For Raspberry Pi Zero/1/2(/3/3_64) Bare Metal code by AZO.  

How to make:  
1. install "arm-none-eabi-gcc" or "gcc-aarch64-linux-gnu"(RP3_64).  
2. Open "Makefile " with texteditor, and change options.  
BOARD=RPZERO/RP1/RPIOLD/RP2(/RP3/RP3_64)  
3. make  
4. If successfully, output "kernel.img"(RPZero/1/1old) or "kernel7.img"(RP2/3) or "kernel8.img"(RP3_64).  

How to run:  
Put follow files to FAT SD card.  
* "bootcode.bin"  
* "start.elf"  
* "config.txt" &lt;- config_rp23.txt or config_rp3_64.txt, Unnecessary for RPZero/1.  
* "kernel.img"(RPZero/1) or "kernel7.img"(RP2/3) or "kernel8.img"(RP3_64)  

Firmware:  
Grub "bootcode.bin" and "start.elf" at  
https://github.com/raspberrypi/firmware/tree/master/boot  

-----

OpenOCD
-------
You will use OpenOCD ARM, and it will be for ARMv7.  
Now, OpenOCD doesn't have function for ARMv8.  

You can make OpenOCD, include current implemantion for ARMv8.  
```shell
git clone https://git.linaro.org/people/peter.griffin/openocd-code.git  
cd openocd-code  
git checkout armv8  
git submodule update --init --recursive  
autoreconf -iv  
./configure --enable-maintainer-mode --enable-ftdi --enable-legacy-ft2232_libftdi --enable-openjtag_ftdi --enable-bcm2835gpio  
make -j4  
sudo make install
```
and  
```shell  
sudo -s  
cp cfg/bcm2835.cfg /usr/local/share/openocd/scripts/board/  
cp cfg/bcm2836.cfg /usr/local/share/openocd/scripts/board/  
cp cfg/ARMv8/bcm2837.cfg /usr/local/share/openocd/scripts/board/  
cp cfg/ARMv8/bcm2837_64.cfg /usr/local/share/openocd/scripts/board/  
exit
```

When debug, run follow command.
```shell
openocd -f interface/yourinterface.cfg -f board/bcm2835.cfg  
```

You can use GDB, or  
Eclipce + OpenOCD debugging Eclipse plug-in.  
http://gnuarmeclipse.github.io/debug/openocd/  

--- But, I don't success ARMv8 connection. Please tell me good method :(  

-----

Referrence
----------
- Peripheral specification https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2835/BCM2835-ARM-Peripherals.pdf  
- dwelch67/raspberrypi: Raspberry Pi ARM based bare metal examples https://github.com/dwelch67/raspberrypi  
- documentation/README.md at master · 96boards/documentation https://github.com/96boards/documentation/blob/master/ConsumerEdition/HiKey/Configuration/JTAG/README.md  
- Olimex ARM-USB-OCD-H / openocd-0.9.0 / libftd2xx1.1.12 / Raspberry Pi 2 / Ubuntu 15.04 https://gist.github.com/tonosaman/62a31e7991a41edb19c5  

