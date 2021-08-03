# Steering Wheel USB

This repository holds the code and electronics for a USB based steering wheel rim with a dual clutch system. These are used in both this [Formula style rim](https://www.thingiverse.com/) and this [G29 button box mod](https://www.thingiverse.com/).

This is intended to be used with the Arduino Pro Mirco Board. Using the breakout PCB isn't necessary but steering wheels with larger button/switch counts will get messy very quick using the button matix method that the program utilises. If using a different wiring configuration to that of the PCB, insure that all the input pin assignments are changed acordingly. 

![Arduino pinout for electronics](https://github.com/Jellyitt/Steering_Wheel_USB/blob/main/PinOut.png?raw=true)

The dual clutch system allows for on the fly adjustment of the slave clutch. The clutch adjustment mode can be actived by two buttons held simultaneously or a long hold of a single button. The clutch adjustment can be fine tuned with an adjustable increment (i.e. adjust at ±1% or ±0.1 etc). The buttons used to perform the clutch adjustment can be customised. The clutches will need to have their ranges calculated to be accuate but a program is include with instructions in the code.   

No further devolopment will be done on this version.
