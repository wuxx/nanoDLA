# nanoDLA v1.2 User manual
* [nanoDLA Introduction](#nanodla-introduction) 
* [Features](#features)
* [How to Use](#how-to-use)
    * [Install PulseView](#install-pulseview)
    * [Install Driver](#install-driver)
    * [Run PulseView](#run-pulseview)
    * [Protocol Decode](#protocol-decode)
* [How to Buy](#how-to-buy)
* [FAQ](#faq)
# nanoDLA Introduction
nanoDLA is a hardware and software open source logic analyzer developed and produced by Muse Lab. It is exquisite and compact in appearance with rich functions. It supports up to 24Mhz sampling rate and more than 100 kinds of protocol analysis. 8 channels can sample data simultaneously. Efficiency could be greatly improved in scenarios such as problem location, complex systems with multiple components, timing analysis and performance analysis. currently there are two version with same function (USB-A version & TYPE-C version)  
<div align=center>
<img src="https://github.com/wuxx/nanoDLA/blob/master/doc/nanoDLA.jpg" width = "700" alt="" align=center />
<img src="https://github.com/wuxx/nanoDLA/blob/master/doc/nanoDLA-TYPE-C.jpg" width = "450" alt="" align=center />
</div>


# Features
- Open source software
- provide source code to compile firmware by yourself
- use open source PulseView as PC software to analyze sampling data, which is rich in functions and easy to use, supports more than 100 kinds of protocol analysis
- Support up to 24Mhz sampling rate and 8 channels can sample data simultaneously
- Support input logic level [-0.5v,5.25v]. low level is [-0.5v, 0.8v], high level is[2v, 5.25v]
- Support operating system like Windows, Linux, Mac, Android
- Size: 7.2cm * 2.4cm * 1.4cm

# How To Use

## Install PulseView 
Download installation package from software directory and double click the installation package to install
![pulseview](https://github.com/wuxx/nanoDLA/blob/master/doc/pulseview.png)

## Install Driver
Plug nanoDLA to PC USB port. nanoDLA will be recognized as unknown device in device manager
![usb_device_unknown](https://github.com/wuxx/nanoDLA/blob/master/doc/usb_device_unknown_v1.2.png)  
  
Search "Zadig" in the the start menu of PC, then open it
![zadig_search](https://github.com/wuxx/nanoDLA/blob/master/doc/zadig_search.png)  
 
Click "Options" and toggle on "List All Devices". Choose device named "fx2lafw", whose USB ID is 1D50:608C and select "WinUSB" in the driver blanket. Then click "Install Driver" button.   
![zadig_install1](https://github.com/wuxx/nanoDLA/blob/master/doc/zadig_install1_v1.2.png)  
  
After finishing installation of driver, nanoDLA would be recognized as a USB device in device manager
![usb_device_default](https://github.com/wuxx/nanoDLA/blob/master/doc/usb_device_default_v1.2.png)

## Run PulseView
Plug nanoDLA to PC USB port and open PulseView software
![usb_device_default](https://github.com/wuxx/nanoDLA/blob/master/doc/usb_device_fx2lafw.png)
Configure channels and sampling rate, the click “Run” button in the top left corner to sample data
![pulseview2](https://github.com/wuxx/nanoDLA/blob/master/doc/pulseview2.png)
  
![pulseview3](https://github.com/wuxx/nanoDLA/blob/master/doc/pulseview3.png)

## Protocol Decode
After signals are sampled, you can use decoder to decode them. Let’s take UART sampled data as an example. Click the decoder menu and select UART protocol, then configure UART parameters such as channels, baud rate, data bits, parity type. Afterwards, the data sampled will be decoded.
![pulseview_decode](https://github.com/wuxx/nanoDLA/blob/master/doc/pulseview_decode.png)


# How To Buy
you can buy nanoDLA from our offcial aliexpress shop [Muse Lab Factory Store](https://muselab-tech.aliexpress.com/)


# FAQ
### Q: how many protocols did the nanoDLA support?
support protocols are list [here](https://github.com/wuxx/nanoDLA/blob/master/decoder_list.md)
### Q: why sometimes sample will fail?
the nanoDLA use USB 2.0 High Speed to sample, if you connect to a low speed USB Hub or your PC speed is not very fast, sample may fail.
