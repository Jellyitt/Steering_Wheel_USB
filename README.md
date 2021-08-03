# Steering Wheel USB

This repository holds the code and electronics for a USB based steering wheel rim with a dual clutch system. These are used in both this [Formula style rim](https://www.thingiverse.com/) and this [G29 button box mod](https://www.thingiverse.com/).

This is intended to be used with the Arduino Pro Mirco Board. Using the breakout PCB isn't necessary but steering wheels with larger button/switch counts will get messy very quick using the button matix method that the program utilises. If using a different wiring configuration to that of the PCB, insure that all the input pin assignments are changed acordingly. The current wiring is displayed in the figure below. Com1-6 lines are treaded as signal lines to one of the switches/buttons in each group and then each group of switches/buttons has a single common line that connects them all together. This makes up a matrix of buttons/switches, theres a number of guides online that explain it better and have pretty pictures and I would advise you go have a look for yourself. 

![Arduino pinout for electronics](https://github.com/Jellyitt/Steering_Wheel_USB/blob/main/Steering%20Wheel%20PCB/PinOutwithEG.png)

The dual clutch system allows for on the fly adjustment of the slave clutch. The clutch adjustment mode can be actived by two buttons held simultaneously or a long hold of a single button. The clutch adjustment can be fine tuned with an adjustable increment (i.e. adjust at ±1% or ±0.1 etc). The buttons used to perform the clutch adjustment can be customised. The clutches will need to have their ranges calculated to be accuate but a program is include with instructions in the code.   

No further devolopment will be done on this version.
