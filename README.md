#Build log for model rocket (class D motor ish) altimeter using Arduino Nano and Barometer.
Author: Adam Hu
Log: 23/01/2024

Buy an Arduino Nano with Mini-B USB Cable. Almost nobody has a Mini-B cable on hand.

Using USB cable, plug in Arduino Nano into PC. It should light up.

Install Arduino IDE from official website, then open it.
Top bar, in Tools, select correct Board, Port, Processor.

Try uploading the example blink program.
It may fail. Check the easy fixes first.

Log: 12/02/2024
The problem might be that the clone didn't come with a bootloader, in which case you need to
install one. Need a working Arduino Uno or Mega to burn the bootloader onto the Nano. 
After taking a long time to get my Uno from home, one of the 3 Nano clones I bought 
had been soldered badly with 2 solder touching 2 pins, making it fail in the chip wipe.

Anyways, it is all solved now.
Tried the Nano with LCD screen with I2C module, and buzzer. Works just fine.
Used 9V battery to power Nano.

Log: 15/02/2024
Connected Nano to Micro SD Card reader module. Much disaster. One of the two SD card reader
simply did not work out of the box. On top of that, I bought SDXC (exFAT) which does not work
with the Micro SD Card. It has to be microSDHC (Fat32) or (Fat16). Bought new ones of both.

Log: 16/02/2024
New SD cards from a cheap unknown brand don't read at all for some reason. Luckily found a
Sandisk one I already owned which worked fine.

Log: 19/02/2024
Connected BMP280 sensor: using 5V and GND, SCL in A5, and SDA in A4. First installed BMP280 library
by Adafruit along with dependencies. Ran bmp280_sensortest, gave error which which said:
"did not find a valid BMP280 sensor."
To fix, install I2CScanner library by Luis LLamas, then run the scanner example code.
Serial code says: I2C device found at address 0x76
Open File Explorer and navigate to: Documents\Arduino\libraries\Adafruit_BMP280_Library
Open the file "Adafruit_BMP280.h" with notepad.
Under  *  I2C ADDRESS/BITS/SETTINGS, change the line BMP280_ADDRESS (0x77) to BMP280_ADDRESS (0x76) and save.
Retry bmp280_sensortest code and it worked, outputting temp and pressure.

Log: 20/02/2024
Completed breadboard circuit with SD card reader, BMP280, and buzzer. Used another computer (work laptop) and
it just refused to connect to the Arduino Nano at all, there was no recognised COM.
Later, the code was being annoying, not letting the SD card initialise, but was fixed eventually.

Log: 21/02/2024
Wrote the code to start a new txt file for the data each run, got buzzer to work with tunes.
Completed main code basically. Tested altimeter writing by going on the elevator.

Log: 26/02/2024
Using my other duplicate components, soldered everything using wires together.
Desoldering the pins on the original components was a massive hassle. Took 4 hours to desolder
and wire everything, but the wiring was right first time. Working on 3D printing the Nosecone stuff.
