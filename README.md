Raspberry Pi Zero/1/2/3 Bare Metal sample by AZO
================================================
For Raspberry Pi Zero/1/2/3 Bare Metal code by AZO(Tomohiro Yoshidomi).<br />
Twitter:@AZO234 E-Mail:typesylph@gmail.com<br />
<br />
How to make:<br />
1.install "arm-none-eabi-gcc".<br />
2.Open "Makefile " with texteditor, and change options.<br />
  BOARD=RPZERO/RP1/RP2/RP3<br />
3.make<br />
4.If successfully, output "kernel.img"(RPZero/1) or "kernel7.img"(RP2/3).<br />
<br />
How to run:<br />
Put follow files to FAT SD card.<br />
"bootcode.bin"<br />
"start.elf"<br />
"config.txt" &lt;- config_01.txt or config_23.txt<br />
"kernel.img"(RPZero/1) or "kernel7.img"(RP2/3)<br />
<br />
Firmware:<br />
Grub "bootcode.bin" and "start.elf" at<br />
https://github.com/raspberrypi/firmware/tree/master/boot<br />
<br />
------------------------------<br />
I referred to there infomation links:<br />
dwelch67/raspberrypi: Raspberry Pi ARM based bare metal examples https://github.com/dwelch67/raspberrypi<br />
PeterLemon/RaspberryPi: Raspberry Pi Bare Metal Assembly Programming https://github.com/PeterLemon/RaspberryPi<br />

