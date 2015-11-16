# xybeamscanner
To upload code to the beam scanner, you need to install the Arduino IDE found in https://www.arduino.cc/en/Main/Software

You also need the makeblock library, found in https://github.com/Makeblock-official/Makeblock-Library

Make sure the selected board in the Arduino IDE is Arduino/Genuino UNO and the serial port is correct. If you are using a Mac, you need to download an extra driver, CH341SER from http://0xcf.com/2015/03/13/chinese-arduinos-with-ch340-ch341-serial-usb-chip-on-os-x-yosemite/

Press p to start, l to stop, o to crash to wall (reset).

# LabJack

Connect FIO6 and GND to the discriminator connected to the PMT. The counter consists of a 32-bit register that accumulates the number of falling edges detected on the pin FIO6.

Source: https://labjack.com/support/datasheets/u3/hardware-description/timers-counters

U3 Python library reference: https://github.com/labjack/LabJackPython/blob/master/src/u3.py
