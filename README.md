# ZeMissiles

THEN FIRE ZE MISSILES! Or at least execute some command by pressing return.

The hardware is a little box with a USB cable, a microcontroller and some
switches.

The software makes the microcontroller pretend to be a USB keyboard and presses
return when the box has been enabled (by turning a keyswitch), armed (by
flipping a toggle switch) and fired (by pressing a button).

The wiring diagram is in "Circuits" and the firmware for the Texas Instruments
Stellaris EK-LM4F120XL development board is in "firmware".

## Parts

* Stellaris EK-LM4F120XL Launchpad
* pushbutton with LED (Kjell & Company 36064)
* keyswitch (Kjell & Company 50563)
* toggle switch with LED (SparkFun COM-11310)
* plastic box (Kjell & Company 89030)
* wire, heatshrink, USB cable

## States to LEDs

toggle switch LED: `!DISABLED`
button LED: `ARMED || FIRING_PRESS || FIRING_RELEASE || FIRED_1`
green LED: `FIRING_PRESS || FIRING_RELEASE || FIRED_1 || FIRED_2`