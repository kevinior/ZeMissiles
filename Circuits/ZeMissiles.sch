EESchema Schematic File Version 2  date 28/10/2013 17:29:59
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Mini server launcher wiring"
Date "28 oct 2013"
Rev "1"
Comp "Opera Software"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L CONN_8 J1
U 1 1 526E890B
P 7600 3900
F 0 "J1" V 7550 3900 60  0000 C CNN
F 1 "CONN_8" V 7650 3900 60  0000 C CNN
F 2 "" H 7600 3900 60  0000 C CNN
F 3 "" H 7600 3900 60  0000 C CNN
	1    7600 3900
	1    0    0    -1  
$EndComp
$Comp
L CONN_8 J3
U 1 1 526E893A
P 8100 3900
F 0 "J3" V 8050 3900 60  0000 C CNN
F 1 "CONN_8" V 8150 3900 60  0000 C CNN
F 2 "" H 8100 3900 60  0000 C CNN
F 3 "" H 8100 3900 60  0000 C CNN
	1    8100 3900
	-1   0    0    -1  
$EndComp
$Comp
L SW_PUSH SW3
U 1 1 526E89EF
P 3100 5200
F 0 "SW3" V 3550 5550 70  0000 C CNN
F 1 "Pushbutton" V 3400 5750 70  0000 C CNN
F 2 "~" H 3100 5200 60  0000 C CNN
F 3 "~" H 3100 5200 60  0000 C CNN
	1    3100 5200
	0    -1   -1   0   
$EndComp
$Comp
L SPST SW1
U 1 1 526E8A57
P 3100 1950
F 0 "SW1" V 3400 2200 70  0000 C CNN
F 1 "Keyswitch" V 3250 2350 70  0000 C CNN
F 2 "~" H 3100 1950 60  0000 C CNN
F 3 "~" H 3100 1950 60  0000 C CNN
	1    3100 1950
	0    -1   -1   0   
$EndComp
$Comp
L SPST SW2
U 1 1 526E8AD0
P 3100 3600
F 0 "SW2" V 3750 3950 70  0000 C CNN
F 1 "Toggle" V 3600 4000 70  0000 C CNN
F 2 "~" H 3100 3600 60  0000 C CNN
F 3 "~" H 3100 3600 60  0000 C CNN
	1    3100 3600
	0    -1   -1   0   
$EndComp
$Comp
L LED D?
U 1 1 526E8AE4
P 3100 5900
F 0 "D?" H 3100 6000 50  0001 C CNN
F 1 "LED" H 3100 5800 50  0001 C CNN
F 2 "~" H 3100 5900 60  0000 C CNN
F 3 "~" H 3100 5900 60  0000 C CNN
	1    3100 5900
	0    1    1    0   
$EndComp
$Comp
L LED D?
U 1 1 526E8AF6
P 3500 3600
F 0 "D?" H 3500 3700 50  0001 C CNN
F 1 "LED" H 3500 3500 50  0001 C CNN
F 2 "~" H 3500 3600 60  0000 C CNN
F 3 "~" H 3500 3600 60  0000 C CNN
	1    3500 3600
	0    1    1    0   
$EndComp
Wire Wire Line
	3100 3100 7100 3100
Wire Wire Line
	3500 3100 3500 3400
Wire Wire Line
	3100 1450 8700 1450
Wire Wire Line
	8700 1450 8700 4050
Wire Wire Line
	8700 4050 8450 4050
Wire Wire Line
	3100 2450 8600 2450
Wire Wire Line
	8600 2450 8600 3550
Wire Wire Line
	8600 3550 8450 3550
Wire Wire Line
	7100 3100 7100 5700
Wire Wire Line
	7100 3550 7250 3550
Wire Wire Line
	3100 4100 4400 4100
Wire Wire Line
	4400 4100 4400 1550
Wire Wire Line
	4400 1550 8800 1550
Wire Wire Line
	8800 1550 8800 4150
Wire Wire Line
	8800 4150 8450 4150
Wire Wire Line
	3500 3800 4500 3800
Wire Wire Line
	4500 3800 4500 1650
Wire Wire Line
	4500 1650 8900 1650
Wire Wire Line
	8900 1650 8900 4250
Wire Wire Line
	8900 4250 8450 4250
Wire Wire Line
	3100 4900 7000 4900
Wire Wire Line
	7000 4900 7000 3950
Wire Wire Line
	7000 3950 7250 3950
Wire Wire Line
	3100 5500 4300 5500
Wire Wire Line
	4300 5500 4300 2450
Connection ~ 4300 2450
Wire Wire Line
	7100 5700 3100 5700
Connection ~ 7100 3550
Wire Wire Line
	3100 6100 6900 6100
Wire Wire Line
	6900 6100 6900 4050
Wire Wire Line
	6900 4050 7250 4050
Wire Notes Line
	2900 2900 2900 4300
Wire Notes Line
	2900 4300 3700 4300
Wire Notes Line
	3700 4300 3700 2900
Wire Notes Line
	3700 2900 2900 2900
Wire Notes Line
	2900 4700 2900 6300
Wire Notes Line
	2900 6300 3700 6300
Wire Notes Line
	3700 6300 3700 4700
Wire Notes Line
	3700 4700 2900 4700
Text Notes 3750 6200 0    60   ~ 0
-
Text Notes 3750 5800 0    60   ~ 0
+
Text Notes 3750 4200 0    60   ~ 0
+
Text Notes 3750 3900 0    60   ~ 0
GND
Text Notes 3750 3200 0    60   ~ 0
<
Text Notes 4000 5700 0    60   ~ 0
Org
Text Notes 4000 6100 0    60   ~ 0
Vio
Text Notes 4000 5500 0    60   ~ 0
Red
Text Notes 4000 4900 0    60   ~ 0
Blu
Text Notes 4000 4100 0    60   ~ 0
Ylw
Text Notes 4000 3800 0    60   ~ 0
Grn
Text Notes 4000 3100 0    60   ~ 0
Org
Text Notes 4000 2450 0    60   ~ 0
Red
Text Notes 4000 1450 0    60   ~ 0
Brn
$Comp
L CONN_1 P?
U 1 1 526E8F9E
P 7450 4350
F 0 "P?" H 7530 4350 40  0001 L CNN
F 1 "CONN_1" H 7450 4405 30  0001 C CNN
F 2 "" H 7450 4350 60  0000 C CNN
F 3 "" H 7450 4350 60  0000 C CNN
	1    7450 4350
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 P?
U 1 1 526E8FB2
P 7450 4450
F 0 "P?" H 7530 4450 40  0001 L CNN
F 1 "CONN_1" H 7450 4505 30  0001 C CNN
F 2 "" H 7450 4450 60  0000 C CNN
F 3 "" H 7450 4450 60  0000 C CNN
	1    7450 4450
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 P?
U 1 1 526E8FF2
P 8250 3450
F 0 "P?" H 8330 3450 40  0001 L CNN
F 1 "CONN_1" H 8250 3505 30  0001 C CNN
F 2 "" H 8250 3450 60  0000 C CNN
F 3 "" H 8250 3450 60  0000 C CNN
	1    8250 3450
	-1   0    0    -1  
$EndComp
$Comp
L CONN_1 P?
U 1 1 526E900C
P 8250 4350
F 0 "P?" H 8330 4350 40  0001 L CNN
F 1 "CONN_1" H 8250 4405 30  0001 C CNN
F 2 "" H 8250 4350 60  0000 C CNN
F 3 "" H 8250 4350 60  0000 C CNN
	1    8250 4350
	-1   0    0    -1  
$EndComp
Text Notes 6700 3600 0    60   ~ 0
3V3
Text Notes 6700 4000 0    60   ~ 0
PE4
Text Notes 6700 4100 0    60   ~ 0
PE5
Text Notes 8950 3600 0    60   ~ 0
GND
Text Notes 8950 4050 0    60   ~ 0
PE1
Text Notes 8950 4200 0    60   ~ 0
PE2
Text Notes 8950 4300 0    60   ~ 0
PE3
$EndSCHEMATC
