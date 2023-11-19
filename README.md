BarePi
======

Overview
--------
bare metal operating system for raspberry pi 4 basic GPIO control and UART for serial bus communication. Fast boot basic embedded development. 

Prerequesites: 

1) Raspberry pi 4 
2) Micro SD card and card reader
3) GNU toolchain find it at: https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads I was building in VSCODE with WSL2 ubuntu 22 in windows so used "AArch64 bare-metal target (aarch64-none-elf)" in the "x86_64 Linux hosted cross toolchains"
4) IDE like VSCODE to customize and compile.
5) Raspberry pi imager 
6) Putty or some other serial terminal software
7) UART to usb adapter

Install:

1) Download the appropriate toolchain for you system and unzip it into your /usr/bin folder in ubuntu.
2) Add the libraries of the toolchain to your path if the bin isn't already in the path ```export PATH=$PATH:/usr/bin/```.
3) Clone the barepi repsository ```git clone https://github.com/lwlagrange/barepi.git```.
4) Enter the directory ```cd barepi/``` Inspect the Makefile and make sure paths to the toolchain are correct and run ```make``` which produces a ```kernel8.img``` file.
5) Using raspberry pi imager ```https://www.raspberrypi.com/news/raspberry-pi-imager-imaging-utility/``` install the Raspian Lite OS 64 on your SD card for initial firmware.
6) From the sd card with the raspbian install and repplace the ```kernel8.img``` file in the sd card with the one you created in barepi project folder and eject the sd card.
7) Install the sd card in a raspberry pi 4 and connect the TX on the UART adapter to RX pin on the raspberry pi and the RX on the UART to the TX on the pi
8) Open putty and open a serial connection to your pi using a baud rate of 115200 look in the device manager to find which com port your device is on mine was ```COM5```
9) Power up the pi and watch for the welcome message on the serial port
10) You can start your development of your bare metal c program by making changes to the ```kernel.c``` in your ide and re-running ```make``` and replacing the ```kernel8.img``` in the sd card





