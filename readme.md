Automated Wire Cutter
========

PSoC4200 based wire cutter with remote console command interface.

![alt text][pic1]  

------

#### Project file tree:
* hardware: KiCad project, BOM, PCB assembly guide, gerber files
* software: PSoC Creator 4.2 project, compiled hex file
* pics  


#### Hardware:  
* PSoC4200 CY8C4245AXI-483 - main MCU
* ESP8266 (ESP12 module) - used as serial/Wifi bridge, pin header for CP2102 based programmer, auto bootloader mode via single DTR line
* Cypress PROC EZ-BLE module  - optional
* IR wire presence detector
* 128x128 SPI ST7735 display  
* 4x4 Keypad
* Trinamic TMC2130 Silent Step Stick stepper controller
* A4988 stepper driver (optional)
* PCB designed as development platform with prototyping area, LEDs, etc.

![alt text][pic2]  

Demo video:  

[![AutomatedWireCutter](http://img.youtube.com/vi/adL4IQJYwPY/0.jpg)](http://www.youtube.com/watch?v=adL4IQJYwPY)

------
#### Software:  
- Firmware for the PSoC has been written in C using the PSoC Creator 4.2.  
- ESP8266 runs [ESP-link](https://github.com/jeelabs/esp-link) firmware and serves as Wifi/Serial bridge over telnet or via built in web server.  

------
#### Serial/telnet console:

![alt text][pic5]

------

![alt text][pic3]
![alt text][pic4]

------
(c) 05.2018 by Piotr Zapart  
www.hexeguitar.com

[pic1]: pics/WrCutter_info.jpg "Automated Wire Cutter"

[pic2]: pics/WrCutter_Kicad.png "Kicad project"
[pic3]: pics/WrCutter_PCB1.jpg "Wire Cutter PCB"
[pic4]: pics/WrCutter_PCB2.jpg "Wire Cutter PCB"
[pic5]: pics/Cutter_console.png "Console"
[pic6]: pics/PulseAnim2.gif "UART as stepper driver"
