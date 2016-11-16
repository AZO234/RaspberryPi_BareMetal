PCM(I2S) sample
===============
<br />
This is PCM/I2S sample code.<br />
(You connot hear 3.5 phono jack.)<br />
You need any external I2S DAC board.<br />
<br />
Connect I2S DAC board to below pins.<br />
pin12 - PCM_CLK  (GPIO18/ALT0), BCK/BCLK/BITCLK<br />
pin35 - PCM_FR   (GPIO19/ALT0), LRCK<br />
pin40 - PCM_DOUT (GPIO21/ALT0), Data<br />
You can use VCC and GND.<br />
<br />
You can run this program,<br />
you'll hear 1kHz(44.1kHz/16bit/stereo) sin-curve sound.<br />
<br />
------------------------------<br />
For Raspberry Pi Zero/1/2/3 Bare Metal code by AZO.<br />
<br />
How to make:<br />
1.install "arm-none-eabi-gcc".<br />
2.Open "Makefile " with texteditor, and change options.<br />
  BOARD=RPZERO/RP1/RP2/RP3<br />
3.make<br />
<br />
How to run:<br />
Put follow file to FAT SD card.<br />
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
I²S - Wikipedia https://en.wikipedia.org/wiki/I%C2%B2S<br />
04 « 4月 « 2008 « PCオーディオブログ http://blog.ratocsystems.com/wp/2008/04/04/19/<br />


