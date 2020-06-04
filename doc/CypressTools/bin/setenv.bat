@echo off

REM to create a shortcut to this environment use the command line:
REM COMMAND.COM /e:4096 /k C:\cypress\usb\bin\setenv.bat
REM
REM If you install the Keil or Cypress tools in a non-default location 
REM you must update this file

path=c:\cypress\usb\bin;c:\keil\c51\bin;%path%
set CYUSB=c:\cypress\usb
set C51INC=%CYUSB%\Target\inc;C:\keil\c51\inc

@echo on