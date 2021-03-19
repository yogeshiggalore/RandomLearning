EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "LowPower"
Date "2020-11-24"
Rev "1"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Timer:TPL5111 U2
U 1 1 5FCD4EAF
P 1900 5450
F 0 "U2" H 2150 5900 50  0000 C CNN
F 1 "TPL5111" H 1550 5900 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-6" H 1900 5450 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tpl5111.pdf" H 250 5600 50  0001 C CNN
	1    1900 5450
	1    0    0    -1  
$EndComp
Text Label 1900 4750 0    50   ~ 0
VBAT
$Comp
L Device:C C6
U 1 1 5FCD7712
P 1650 4750
F 0 "C6" V 1600 4850 50  0000 C CNN
F 1 "100n" V 1600 4600 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 1688 4600 50  0001 C CNN
F 3 "~" H 1650 4750 50  0001 C CNN
	1    1650 4750
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5FCD8215
P 1250 4750
F 0 "#PWR02" H 1250 4500 50  0001 C CNN
F 1 "GND" H 1255 4577 50  0000 C CNN
F 2 "" H 1250 4750 50  0001 C CNN
F 3 "" H 1250 4750 50  0001 C CNN
	1    1250 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 4750 1800 4750
Wire Wire Line
	1900 4750 1900 4850
Wire Wire Line
	1250 4750 1500 4750
Wire Wire Line
	1300 5450 1150 5450
Text Label 1150 5450 2    50   ~ 0
DELAY
$Comp
L power:GND #PWR04
U 1 1 5FCDDA9D
P 1900 5950
F 0 "#PWR04" H 1900 5700 50  0001 C CNN
F 1 "GND" H 1905 5777 50  0000 C CNN
F 2 "" H 1900 5950 50  0001 C CNN
F 3 "" H 1900 5950 50  0001 C CNN
	1    1900 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 5850 1900 5950
$Comp
L Device:R R17
U 1 1 5FCDF2C7
P 3450 5600
F 0 "R17" V 3550 5500 50  0000 L CNN
F 1 "10K" V 3350 5500 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 3380 5600 50  0001 C CNN
F 3 "~" H 3450 5600 50  0001 C CNN
	1    3450 5600
	1    0    0    -1  
$EndComp
$Comp
L Device:C C8
U 1 1 5FCDF85D
P 2650 5600
F 0 "C8" H 2450 5600 50  0000 L CNN
F 1 "10u" H 2750 5600 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 2688 5450 50  0001 C CNN
F 3 "~" H 2650 5600 50  0001 C CNN
	1    2650 5600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R14
U 1 1 5FCDFD29
P 3050 5450
F 0 "R14" V 2950 5450 50  0000 C CNN
F 1 "10K" V 3150 5450 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 2980 5450 50  0001 C CNN
F 3 "~" H 3050 5450 50  0001 C CNN
	1    3050 5450
	0    1    1    0   
$EndComp
Wire Wire Line
	2400 5450 2650 5450
Wire Wire Line
	2650 5450 2900 5450
Connection ~ 2650 5450
Wire Wire Line
	3200 5450 3450 5450
$Comp
L power:GND #PWR06
U 1 1 5FCE54F9
P 3050 5750
F 0 "#PWR06" H 3050 5500 50  0001 C CNN
F 1 "GND" H 3055 5577 50  0000 C CNN
F 2 "" H 3050 5750 50  0001 C CNN
F 3 "" H 3050 5750 50  0001 C CNN
	1    3050 5750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 5750 3050 5750
Wire Wire Line
	2650 5750 3050 5750
Connection ~ 3050 5750
Wire Wire Line
	3450 5450 3650 5450
Connection ~ 3450 5450
Text Label 3650 5450 0    50   ~ 0
DONE
Wire Wire Line
	2400 5350 2600 5350
Text Label 3400 4850 0    50   ~ 0
DRIVE
Text Notes 2850 4500 0    50   ~ 0
timer with 1hour wakeup
$Comp
L LowPowerV1-rescue:TPS73733DCQ-TPS U3
U 1 1 5FCF3A9E
P 2400 7050
F 0 "U3" H 2400 7620 50  0000 C CNN
F 1 "TPS73733DCQ" H 2400 7529 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-6" H 2400 7050 50  0001 L BNN
F 3 "" H 2400 7050 50  0001 L BNN
	1    2400 7050
	1    0    0    -1  
$EndComp
$Comp
L Device:C C7
U 1 1 5FCF4040
P 950 7000
F 0 "C7" H 1050 7000 50  0000 L CNN
F 1 "100n" V 800 6950 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 988 6850 50  0001 C CNN
F 3 "~" H 950 7000 50  0001 C CNN
	1    950  7000
	1    0    0    -1  
$EndComp
$Comp
L Device:C C10
U 1 1 5FCF4582
P 1400 7000
F 0 "C10" H 1500 7000 50  0000 L CNN
F 1 "10u" V 1250 6950 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 1438 6850 50  0001 C CNN
F 3 "~" H 1400 7000 50  0001 C CNN
	1    1400 7000
	1    0    0    -1  
$EndComp
Wire Wire Line
	1400 6850 950  6850
$Comp
L power:GND #PWR01
U 1 1 5FCF8D87
P 1150 7250
F 0 "#PWR01" H 1150 7000 50  0001 C CNN
F 1 "GND" H 1155 7077 50  0000 C CNN
F 2 "" H 1150 7250 50  0001 C CNN
F 3 "" H 1150 7250 50  0001 C CNN
	1    1150 7250
	1    0    0    -1  
$EndComp
Wire Wire Line
	950  7150 950  7250
Wire Wire Line
	950  7250 1150 7250
Wire Wire Line
	1400 7150 1400 7250
Wire Wire Line
	1400 7250 1150 7250
Connection ~ 1150 7250
NoConn ~ 1700 6950
Wire Wire Line
	1700 6850 1400 6850
Connection ~ 1400 6850
$Comp
L Device:R R16
U 1 1 5FD05DE7
P 2850 4850
F 0 "R16" V 2750 4850 50  0000 C CNN
F 1 "10K" V 2950 4850 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 2780 4850 50  0001 C CNN
F 3 "~" H 2850 4850 50  0001 C CNN
	1    2850 4850
	0    1    1    0   
$EndComp
$Comp
L Device:C C12
U 1 1 5FD06220
P 3150 5000
F 0 "C12" H 2950 4900 50  0000 L CNN
F 1 "10u" H 3250 5000 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 3188 4850 50  0001 C CNN
F 3 "~" H 3150 5000 50  0001 C CNN
	1    3150 5000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR07
U 1 1 5FD08410
P 3300 5200
F 0 "#PWR07" H 3300 4950 50  0001 C CNN
F 1 "GND" V 3305 5072 50  0000 R CNN
F 2 "" H 3300 5200 50  0001 C CNN
F 3 "" H 3300 5200 50  0001 C CNN
	1    3300 5200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2600 4850 2700 4850
Wire Wire Line
	2600 4850 2600 5350
Wire Wire Line
	3150 5150 3150 5200
Wire Wire Line
	3150 5200 3300 5200
Wire Wire Line
	3000 4850 3150 4850
Wire Wire Line
	3150 4850 3400 4850
Connection ~ 3150 4850
Wire Wire Line
	1700 6750 1550 6750
Text Label 1550 6750 2    50   ~ 0
DRIVE
$Comp
L power:GND #PWR08
U 1 1 5FD11817
P 3300 7350
F 0 "#PWR08" H 3300 7100 50  0001 C CNN
F 1 "GND" H 3305 7177 50  0000 C CNN
F 2 "" H 3300 7350 50  0001 C CNN
F 3 "" H 3300 7350 50  0001 C CNN
	1    3300 7350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 7250 3300 7250
Wire Wire Line
	3300 7250 3300 7350
Wire Wire Line
	3100 7350 3300 7350
Connection ~ 3300 7350
$Comp
L Device:C C13
U 1 1 5FD15823
P 3350 6900
F 0 "C13" V 3500 6850 50  0000 L CNN
F 1 "100n" V 3200 6800 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 3388 6750 50  0001 C CNN
F 3 "~" H 3350 6900 50  0001 C CNN
	1    3350 6900
	1    0    0    -1  
$EndComp
$Comp
L Device:C C14
U 1 1 5FD15D5A
P 3800 6900
F 0 "C14" V 3950 6850 50  0000 L CNN
F 1 "10u" V 3650 6850 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 3838 6750 50  0001 C CNN
F 3 "~" H 3800 6900 50  0001 C CNN
	1    3800 6900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 6750 3350 6750
Wire Wire Line
	3350 6750 3800 6750
Connection ~ 3350 6750
Wire Wire Line
	3800 6750 4150 6750
Connection ~ 3800 6750
$Comp
L power:GND #PWR09
U 1 1 5FD1C83C
P 3600 7350
F 0 "#PWR09" H 3600 7100 50  0001 C CNN
F 1 "GND" H 3605 7177 50  0000 C CNN
F 2 "" H 3600 7350 50  0001 C CNN
F 3 "" H 3600 7350 50  0001 C CNN
	1    3600 7350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3350 7050 3600 7050
Wire Wire Line
	3600 7350 3600 7050
Connection ~ 3600 7050
Wire Wire Line
	3600 7050 3800 7050
$Comp
L power:+3.3V #PWR010
U 1 1 5FD21D33
P 4150 6750
F 0 "#PWR010" H 4150 6600 50  0001 C CNN
F 1 "+3.3V" H 4165 6923 50  0000 C CNN
F 2 "" H 4150 6750 50  0001 C CNN
F 3 "" H 4150 6750 50  0001 C CNN
	1    4150 6750
	1    0    0    -1  
$EndComp
Wire Notes Line
	650  6350 650  7700
Wire Notes Line
	650  7700 4400 7700
Wire Notes Line
	4400 7700 4400 6350
Wire Notes Line
	4400 6350 650  6350
Text Notes 1350 6500 2    50   ~ 0
Voltage regulator
$Comp
L Device:R R18
U 1 1 5FD3739F
P 4850 5700
F 0 "R18" V 4950 5700 50  0000 C CNN
F 1 "10K" V 4734 5700 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 4780 5700 50  0001 C CNN
F 3 "~" H 4850 5700 50  0001 C CNN
	1    4850 5700
	-1   0    0    1   
$EndComp
$Comp
L Device:R R19
U 1 1 5FD37B9E
P 5300 4850
F 0 "R19" V 5400 4850 50  0000 C CNN
F 1 "10K" V 5184 4850 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 5230 4850 50  0001 C CNN
F 3 "~" H 5300 4850 50  0001 C CNN
	1    5300 4850
	-1   0    0    1   
$EndComp
$Comp
L Device:R R21
U 1 1 5FD37FDF
P 5800 5900
F 0 "R21" V 5900 5900 50  0000 C CNN
F 1 "10K" V 5684 5900 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 5730 5900 50  0001 C CNN
F 3 "~" H 5800 5900 50  0001 C CNN
	1    5800 5900
	-1   0    0    1   
$EndComp
$Comp
L Device:R R20
U 1 1 5FD3828A
P 5800 5550
F 0 "R20" V 5900 5550 50  0000 C CNN
F 1 "49.9K" V 5684 5550 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 5730 5550 50  0001 C CNN
F 3 "~" H 5800 5550 50  0001 C CNN
	1    5800 5550
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR015
U 1 1 5FD38507
P 5800 6100
F 0 "#PWR015" H 5800 5850 50  0001 C CNN
F 1 "GND" H 5805 5927 50  0000 C CNN
F 2 "" H 5800 6100 50  0001 C CNN
F 3 "" H 5800 6100 50  0001 C CNN
	1    5800 6100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 5FD38F29
P 5150 5900
F 0 "#PWR012" H 5150 5650 50  0001 C CNN
F 1 "GND" H 5155 5727 50  0000 C CNN
F 2 "" H 5150 5900 50  0001 C CNN
F 3 "" H 5150 5900 50  0001 C CNN
	1    5150 5900
	1    0    0    -1  
$EndComp
$Comp
L Device:Q_PMOS_GSD Q7
U 1 1 5FD3C55A
P 5700 5150
F 0 "Q7" H 5550 5050 50  0000 L CNN
F 1 "RZM001P02T2L" V 5950 4800 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-723" H 5900 5250 50  0001 C CNN
F 3 "~" H 5700 5150 50  0001 C CNN
	1    5700 5150
	1    0    0    1   
$EndComp
$Comp
L Device:Q_NMOS_GSD Q6
U 1 1 5FD3DCB1
P 5200 5450
F 0 "Q6" H 5050 5550 50  0000 L CNN
F 1 "RUM001L02T2CL" V 5450 5050 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-723" H 5400 5550 50  0001 C CNN
F 3 "~" H 5200 5450 50  0001 C CNN
	1    5200 5450
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR011
U 1 1 5FD3E8FE
P 4850 5350
F 0 "#PWR011" H 4850 5200 50  0001 C CNN
F 1 "+3.3V" H 4850 5500 50  0000 C CNN
F 2 "" H 4850 5350 50  0001 C CNN
F 3 "" H 4850 5350 50  0001 C CNN
	1    4850 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 5850 4850 5900
Wire Wire Line
	4850 5900 5150 5900
Wire Wire Line
	5300 5650 5300 5900
Wire Wire Line
	5300 5900 5150 5900
Connection ~ 5150 5900
Wire Wire Line
	4850 5550 4850 5450
Wire Wire Line
	4850 5450 5000 5450
Wire Wire Line
	4850 5350 4850 5450
Connection ~ 4850 5450
Wire Wire Line
	5300 5150 5300 5250
Wire Wire Line
	5300 5000 5300 5150
Connection ~ 5300 5150
Wire Wire Line
	5800 4950 5800 4600
Wire Wire Line
	5800 4600 5550 4600
Wire Wire Line
	5300 4600 5300 4700
Wire Wire Line
	5550 4600 5550 4400
Wire Wire Line
	5300 5150 5500 5150
Connection ~ 5550 4600
Wire Wire Line
	5550 4600 5300 4600
Text Label 5550 4400 0    50   ~ 0
VBAT
Wire Wire Line
	5800 5400 5800 5350
Wire Wire Line
	5800 5750 5800 5700
Wire Wire Line
	5800 6100 5800 6050
Wire Wire Line
	8250 1400 8250 1550
Wire Wire Line
	8000 1400 8250 1400
Text Label 8250 1150 0    50   ~ 0
VBAT
Wire Wire Line
	8250 1300 8250 1150
Wire Wire Line
	8000 1300 8250 1300
$Comp
L Connector:Conn_01x02_Male J2
U 1 1 5FCC9ECB
P 7800 1300
F 0 "J2" H 7908 1481 50  0000 C CNN
F 1 "BatInput" H 7908 1390 50  0000 C CNN
F 2 "Connector_JST:JST_EH_B2B-EH-A_1x02_P2.50mm_Vertical" H 7800 1300 50  0001 C CNN
F 3 "~" H 7800 1300 50  0001 C CNN
	1    7800 1300
	1    0    0    -1  
$EndComp
Text Notes 6400 2400 0    50   ~ 0
Set for 1hour
Text Label 5850 1700 0    50   ~ 0
DELAY
Connection ~ 5850 2000
Wire Wire Line
	5850 2000 5850 1700
Text Label 7000 2000 0    50   ~ 0
VBAT
Wire Wire Line
	6900 2000 7000 2000
Connection ~ 6350 2000
Wire Wire Line
	5850 2000 5850 2450
Wire Wire Line
	6350 2000 5850 2000
Wire Wire Line
	6350 2000 6500 2000
Wire Wire Line
	6350 2300 6350 2450
$Comp
L power:GND #PWR016
U 1 1 5FCD04C5
P 6350 2450
F 0 "#PWR016" H 6350 2200 50  0001 C CNN
F 1 "GND" H 6355 2277 50  0000 C CNN
F 2 "" H 6350 2450 50  0001 C CNN
F 3 "" H 6350 2450 50  0001 C CNN
	1    6350 2450
	1    0    0    -1  
$EndComp
$Comp
L Device:R R9
U 1 1 5FCCEEA3
P 6350 2150
F 0 "R9" V 6450 2150 50  0000 C CNN
F 1 "124K" V 6250 2150 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 6280 2150 50  0001 C CNN
F 3 "~" H 6350 2150 50  0001 C CNN
	1    6350 2150
	-1   0    0    1   
$EndComp
$Comp
L Switch:SW_SPST SW1
U 1 1 5FCCE40E
P 6700 2000
F 0 "SW1" H 6750 2150 50  0000 C CNN
F 1 "SW" H 6700 1900 50  0000 C CNN
F 2 "Button_Switch_SMD:SW_Push_1P1T_NO_6x6mm_H9.5mm" H 6700 2000 50  0001 C CNN
F 3 "~" H 6700 2000 50  0001 C CNN
	1    6700 2000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR020
U 1 1 5FCCBB57
P 8250 1550
F 0 "#PWR020" H 8250 1300 50  0001 C CNN
F 1 "GND" H 8255 1377 50  0000 C CNN
F 2 "" H 8250 1550 50  0001 C CNN
F 3 "" H 8250 1550 50  0001 C CNN
	1    8250 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 2550 5700 2550
Wire Wire Line
	5700 2600 5700 2550
$Comp
L power:+3.3V #PWR014
U 1 1 5FCB8452
P 5800 2550
F 0 "#PWR014" H 5800 2400 50  0001 C CNN
F 1 "+3.3V" V 5815 2678 50  0000 L CNN
F 2 "" H 5800 2550 50  0001 C CNN
F 3 "" H 5800 2550 50  0001 C CNN
	1    5800 2550
	0    1    1    0   
$EndComp
Text Label 6350 2950 0    50   ~ 0
EXT_WAKE
Wire Wire Line
	6200 2950 6350 2950
Wire Wire Line
	5700 2950 5700 2900
Connection ~ 5700 2950
Wire Wire Line
	5900 2950 5700 2950
Wire Wire Line
	5700 3000 5700 2950
Wire Wire Line
	5700 3500 5700 3400
$Comp
L Device:R R10
U 1 1 5FCB2E20
P 6050 2950
F 0 "R10" V 6150 2950 50  0000 C CNN
F 1 "1K" V 5934 2950 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 5980 2950 50  0001 C CNN
F 3 "~" H 6050 2950 50  0001 C CNN
	1    6050 2950
	0    1    1    0   
$EndComp
$Comp
L Device:R R8
U 1 1 5FCB1950
P 5700 2750
F 0 "R8" V 5800 2750 50  0000 C CNN
F 1 "10K" V 5584 2750 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 5630 2750 50  0001 C CNN
F 3 "~" H 5700 2750 50  0001 C CNN
	1    5700 2750
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR013
U 1 1 5FCB09A3
P 5700 3500
F 0 "#PWR013" H 5700 3250 50  0001 C CNN
F 1 "GND" H 5705 3327 50  0000 C CNN
F 2 "" H 5700 3500 50  0001 C CNN
F 3 "" H 5700 3500 50  0001 C CNN
	1    5700 3500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 5FC140F7
P 2700 3350
F 0 "#PWR05" H 2700 3100 50  0001 C CNN
F 1 "GND" H 2705 3177 50  0000 C CNN
F 2 "" H 2700 3350 50  0001 C CNN
F 3 "" H 2700 3350 50  0001 C CNN
	1    2700 3350
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J1
U 1 1 5FBEAB2A
P 800 1000
F 0 "J1" H 1050 1050 50  0000 C CNN
F 1 "CN1" H 700 950 50  0000 C CNN
F 2 "Connector_JST:JST_EH_B2B-EH-A_1x02_P2.50mm_Vertical" H 800 1000 50  0001 C CNN
F 3 "~" H 800 1000 50  0001 C CNN
	1    800  1000
	1    0    0    -1  
$EndComp
$Comp
L Diode:BAT54W D1
U 1 1 5FBEB55F
P 1400 1100
F 0 "D1" H 1400 1000 50  0000 C CNN
F 1 "BAT54W" H 1400 1250 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-323_SC-70" H 1400 925 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/BAT54W_SER.pdf" H 1400 1100 50  0001 C CNN
	1    1400 1100
	-1   0    0    1   
$EndComp
Wire Wire Line
	1000 1100 1250 1100
Wire Wire Line
	1000 1000 1200 1000
Wire Wire Line
	1200 1000 1200 850 
Text Label 1200 850  0    50   ~ 0
VBAT
$Comp
L Power_Supervisor:TPS3839DBZ U1
U 1 1 5FBECEE4
P 1800 1400
F 0 "U1" H 1670 1446 50  0000 R CNN
F 1 "TPS3839DBZ" H 1650 1300 50  0000 R CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 1800 1400 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/sbvs193d/sbvs193d.pdf" H 1800 1400 50  0001 C CNN
	1    1800 1400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 5FBEDECD
P 1800 1800
F 0 "#PWR03" H 1800 1550 50  0001 C CNN
F 1 "GND" H 1805 1627 50  0000 C CNN
F 2 "" H 1800 1800 50  0001 C CNN
F 3 "" H 1800 1800 50  0001 C CNN
	1    1800 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 1800 1800 1700
$Comp
L Device:R R2
U 1 1 5FC00F82
P 3200 1850
F 0 "R2" H 3250 1850 50  0000 L CNN
F 1 "100" V 3100 1800 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 3130 1850 50  0001 C CNN
F 3 "~" H 3200 1850 50  0001 C CNN
	1    3200 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 1700 3200 1600
Text Label 3200 1600 0    50   ~ 0
VBAT
Wire Wire Line
	3200 2000 3200 2200
Wire Wire Line
	3200 2750 3200 2650
$Comp
L Device:C C2
U 1 1 5FC0F727
P 2700 3100
F 0 "C2" H 2500 3100 50  0000 L CNN
F 1 "100nf" H 2800 3100 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 2738 2950 50  0001 C CNN
F 3 "~" H 2700 3100 50  0001 C CNN
	1    2700 3100
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5FC1307A
P 2350 2950
F 0 "R1" V 2250 2950 50  0000 C CNN
F 1 "10K" V 2450 2950 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 2280 2950 50  0001 C CNN
F 3 "~" H 2350 2950 50  0001 C CNN
	1    2350 2950
	0    1    1    0   
$EndComp
Wire Wire Line
	2500 2950 2700 2950
Wire Wire Line
	2700 2950 2900 2950
Connection ~ 2700 2950
Wire Wire Line
	2200 2950 2200 3250
Wire Wire Line
	2200 3250 2700 3250
Wire Wire Line
	2700 3250 3200 3250
Wire Wire Line
	3200 3250 3200 3150
Connection ~ 2700 3250
Wire Wire Line
	2700 3250 2700 3350
$Comp
L Device:Q_PMOS_GSD Q1
U 1 1 5FC95028
P 3100 2400
F 0 "Q1" H 2950 2300 50  0000 L CNN
F 1 "RZM001P02T2L" V 3350 2050 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-723" H 3300 2500 50  0001 C CNN
F 3 "~" H 3100 2400 50  0001 C CNN
	1    3100 2400
	1    0    0    1   
$EndComp
$Comp
L Device:Q_NMOS_GSD Q2
U 1 1 5FC968A1
P 3100 2950
F 0 "Q2" H 2950 3050 50  0000 L CNN
F 1 "RUM001L02T2CL" V 3350 2550 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-723" H 3300 3050 50  0001 C CNN
F 3 "~" H 3100 2950 50  0001 C CNN
	1    3100 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 2400 2900 2950
Connection ~ 2900 2950
Wire Wire Line
	2200 1400 2200 2400
Connection ~ 2200 2950
Wire Wire Line
	2200 1400 2750 1400
Connection ~ 2200 1400
Connection ~ 3200 2650
Wire Wire Line
	3200 2650 3200 2600
$Comp
L Device:R R6
U 1 1 5FCA4DF7
P 4400 2650
F 0 "R6" V 4300 2650 50  0000 C CNN
F 1 "100" V 4500 2650 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 4330 2650 50  0001 C CNN
F 3 "~" H 4400 2650 50  0001 C CNN
	1    4400 2650
	0    1    1    0   
$EndComp
$Comp
L Device:C C5
U 1 1 5FCA57FC
P 4850 2650
F 0 "C5" V 4800 2550 50  0000 C CNN
F 1 "10u" V 4800 2800 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 4888 2500 50  0001 C CNN
F 3 "~" H 4850 2650 50  0001 C CNN
	1    4850 2650
	0    1    1    0   
$EndComp
Wire Wire Line
	4550 2650 4700 2650
$Comp
L Diode:BAT54W D3
U 1 1 5FCA6C6E
P 4900 1900
F 0 "D3" V 4700 1900 50  0000 C CNN
F 1 "BAT54W" H 4950 1750 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-323_SC-70" H 4900 1725 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/BAT54W_SER.pdf" H 4900 1900 50  0001 C CNN
	1    4900 1900
	0    1    1    0   
$EndComp
$Comp
L Device:R R7
U 1 1 5FCA7A32
P 5100 1900
F 0 "R7" H 5050 1900 50  0000 R CNN
F 1 "100K" V 5200 2000 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 5030 1900 50  0001 C CNN
F 3 "~" H 5100 1900 50  0001 C CNN
	1    5100 1900
	-1   0    0    1   
$EndComp
Text Label 5400 1650 0    50   ~ 0
VBAT
Wire Wire Line
	4900 1750 5100 1750
Wire Wire Line
	5100 1750 5400 1750
Connection ~ 5100 1750
Wire Wire Line
	5400 1750 5400 1650
Connection ~ 5400 1750
Wire Wire Line
	5100 2650 5000 2650
Connection ~ 4100 2650
Wire Wire Line
	4100 2650 4250 2650
Wire Wire Line
	4100 2650 3750 2650
Wire Wire Line
	5400 2000 5400 1750
Wire Wire Line
	5100 2200 5100 2650
Connection ~ 5100 2200
Wire Wire Line
	5100 2050 5100 2200
Wire Wire Line
	4900 2200 5100 2200
Wire Wire Line
	4900 2050 4900 2200
$Comp
L Device:Q_PMOS_GSD Q4
U 1 1 5FCA631C
P 5300 2200
F 0 "Q4" H 5150 2100 50  0000 L CNN
F 1 "RZM001P02T2L" V 5550 1800 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-723" H 5500 2300 50  0001 C CNN
F 3 "~" H 5300 2200 50  0001 C CNN
	1    5300 2200
	1    0    0    1   
$EndComp
Wire Wire Line
	4100 1400 4100 2650
Wire Wire Line
	5400 2400 5400 2450
Wire Wire Line
	5850 2450 5400 2450
Connection ~ 5400 2450
Wire Wire Line
	5400 2450 5400 3200
$Comp
L Device:Q_NMOS_GSD Q5
U 1 1 5FCAEA70
P 5600 3200
F 0 "Q5" H 5450 3300 50  0000 L CNN
F 1 "RUM001L02T2CL" V 5850 2700 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-723" H 5800 3300 50  0001 C CNN
F 3 "~" H 5600 3200 50  0001 C CNN
	1    5600 3200
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint JP2
U 1 1 5FFCA432
P 2200 2400
F 0 "JP2" V 2300 2450 50  0000 L CNN
F 1 "TP" V 2100 2450 50  0000 L CNN
F 2 "TestPoint:TestPoint_Bridge_Pitch2.54mm_Drill1.0mm" H 2400 2400 50  0001 C CNN
F 3 "~" H 2400 2400 50  0001 C CNN
	1    2200 2400
	0    -1   -1   0   
$EndComp
Connection ~ 2200 2400
Wire Wire Line
	2200 2400 2200 2950
$Comp
L Connector:TestPoint JP3
U 1 1 5FFD36AF
P 2750 1400
F 0 "JP3" H 2800 1450 50  0000 L CNN
F 1 "TP" H 2650 1450 50  0000 L CNN
F 2 "TestPoint:TestPoint_Bridge_Pitch2.54mm_Drill1.0mm" H 2950 1400 50  0001 C CNN
F 3 "~" H 2950 1400 50  0001 C CNN
	1    2750 1400
	1    0    0    -1  
$EndComp
Connection ~ 2750 1400
Wire Wire Line
	2750 1400 4100 1400
$Comp
L Connector:TestPoint JP1
U 1 1 5FFD4B1F
P 2200 3250
F 0 "JP1" V 2300 3300 50  0000 L CNN
F 1 "TP" V 2100 3300 50  0000 L CNN
F 2 "TestPoint:TestPoint_Bridge_Pitch2.54mm_Drill1.0mm" H 2400 3250 50  0001 C CNN
F 3 "~" H 2400 3250 50  0001 C CNN
	1    2200 3250
	0    -1   -1   0   
$EndComp
Connection ~ 2200 3250
$Comp
L Connector:TestPoint JP4
U 1 1 5FFD65CE
P 3750 2650
F 0 "JP4" H 3800 2700 50  0000 L CNN
F 1 "TP" H 3650 2700 50  0000 L CNN
F 2 "TestPoint:TestPoint_Bridge_Pitch2.54mm_Drill1.0mm" H 3950 2650 50  0001 C CNN
F 3 "~" H 3950 2650 50  0001 C CNN
	1    3750 2650
	1    0    0    -1  
$EndComp
Connection ~ 3750 2650
Wire Wire Line
	3750 2650 3200 2650
Wire Notes Line
	550  650  550  3800
Wire Notes Line
	7350 3800 7350 650 
Wire Notes Line
	7350 650  550  650 
Wire Notes Line
	800  6200 800  4300
Wire Notes Line
	800  4300 4100 4300
Wire Notes Line
	4100 4300 4100 6200
Wire Notes Line
	800  6200 4100 6200
Wire Wire Line
	5800 5700 6000 5700
Connection ~ 5800 5700
Text Label 6000 5700 0    50   ~ 0
BAT_DIV
Wire Notes Line
	4600 6350 6300 6350
Wire Notes Line
	6300 6350 6300 4250
Wire Notes Line
	6300 4250 4600 4250
Wire Notes Line
	4600 4250 4600 6350
Wire Notes Line
	550  3800 7350 3800
Wire Wire Line
	7800 5200 7650 5200
Wire Wire Line
	7800 5300 7650 5300
Wire Wire Line
	7800 5400 7650 5400
Wire Wire Line
	7800 5500 7650 5500
Wire Wire Line
	7800 4900 7650 4900
$Comp
L power:+3.3V #PWR019
U 1 1 600B6073
P 7500 4350
F 0 "#PWR019" H 7500 4200 50  0001 C CNN
F 1 "+3.3V" H 7515 4523 50  0000 C CNN
F 2 "" H 7500 4350 50  0001 C CNN
F 3 "" H 7500 4350 50  0001 C CNN
	1    7500 4350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 600B6DDD
P 7350 4600
F 0 "R3" H 7420 4646 50  0000 L CNN
F 1 "10K" H 7420 4555 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 7280 4600 50  0001 C CNN
F 3 "~" H 7350 4600 50  0001 C CNN
	1    7350 4600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 600B75B0
P 7650 4600
F 0 "R4" H 7720 4646 50  0000 L CNN
F 1 "10K" H 7720 4555 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 7580 4600 50  0001 C CNN
F 3 "~" H 7650 4600 50  0001 C CNN
	1    7650 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	7650 4900 7650 4750
Wire Wire Line
	7350 4450 7350 4350
Wire Wire Line
	7350 4350 7500 4350
Wire Wire Line
	7650 4450 7650 4350
Wire Wire Line
	7650 4350 7500 4350
Connection ~ 7500 4350
Wire Wire Line
	7350 5100 7800 5100
Wire Wire Line
	7350 4750 7350 5100
$Comp
L power:GND #PWR022
U 1 1 600E4797
P 9800 5900
F 0 "#PWR022" H 9800 5650 50  0001 C CNN
F 1 "GND" H 9805 5727 50  0000 C CNN
F 2 "" H 9800 5900 50  0001 C CNN
F 3 "" H 9800 5900 50  0001 C CNN
	1    9800 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9800 5900 9800 5600
Text Label 7650 5200 2    50   ~ 0
EXT_WAKE
$Comp
L Device:C C9
U 1 1 600EA3E3
P 7300 5900
F 0 "C9" H 7415 5946 50  0000 L CNN
F 1 "10u" H 7415 5855 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 7338 5750 50  0001 C CNN
F 3 "~" H 7300 5900 50  0001 C CNN
	1    7300 5900
	1    0    0    -1  
$EndComp
$Comp
L Device:C C11
U 1 1 600EAC4E
P 7650 5900
F 0 "C11" H 7765 5946 50  0000 L CNN
F 1 "100n" H 7765 5855 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 7688 5750 50  0001 C CNN
F 3 "~" H 7650 5900 50  0001 C CNN
	1    7650 5900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR018
U 1 1 600EAF66
P 7450 6150
F 0 "#PWR018" H 7450 5900 50  0001 C CNN
F 1 "GND" H 7455 5977 50  0000 C CNN
F 2 "" H 7450 6150 50  0001 C CNN
F 3 "" H 7450 6150 50  0001 C CNN
	1    7450 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7300 6050 7300 6150
Wire Wire Line
	7300 6150 7450 6150
Wire Wire Line
	7650 6050 7650 6150
Wire Wire Line
	7650 6150 7450 6150
Connection ~ 7450 6150
$Comp
L power:+3.3V #PWR017
U 1 1 600F546F
P 7350 5600
F 0 "#PWR017" H 7350 5450 50  0001 C CNN
F 1 "+3.3V" V 7365 5728 50  0000 L CNN
F 2 "" H 7350 5600 50  0001 C CNN
F 3 "" H 7350 5600 50  0001 C CNN
	1    7350 5600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7350 5600 7500 5600
Wire Wire Line
	7300 5750 7500 5750
Wire Wire Line
	7500 5750 7500 5600
Connection ~ 7500 5750
Wire Wire Line
	7500 5750 7650 5750
Connection ~ 7500 5600
Wire Wire Line
	7500 5600 7800 5600
Text Label 9800 5500 0    50   ~ 0
DONE
Text Label 9800 5300 0    50   ~ 0
DTR
Text Label 9800 5400 0    50   ~ 0
DTX
Text Label 9800 4900 0    50   ~ 0
TX
Text Label 9800 5000 0    50   ~ 0
RX
Text Label 9800 5100 0    50   ~ 0
I2C_SCL
Text Label 9800 5200 0    50   ~ 0
I2C_SDA
Text Label 7650 5300 2    50   ~ 0
SPI_SCK
Text Label 7650 5400 2    50   ~ 0
SPI_MISO
Text Label 7650 5500 2    50   ~ 0
SPI_MOSI
Wire Wire Line
	7800 5000 7700 5000
Text Label 7700 5000 2    50   ~ 0
BAT_DIV
NoConn ~ 8450 6100
NoConn ~ 8550 6100
NoConn ~ 8650 6100
NoConn ~ 8750 6100
NoConn ~ 8850 6100
NoConn ~ 8950 6100
Wire Wire Line
	1300 5350 1150 5350
Text Label 1150 5350 2    50   ~ 0
VBAT
Wire Wire Line
	9600 5600 9800 5600
Wire Wire Line
	9600 5500 9800 5500
Wire Wire Line
	9600 5400 9800 5400
Wire Wire Line
	9600 5300 9800 5300
Wire Wire Line
	9600 5200 9800 5200
Wire Wire Line
	9600 5100 9800 5100
Wire Wire Line
	9600 5000 9800 5000
Wire Wire Line
	9600 4900 9800 4900
Connection ~ 10550 5100
Wire Wire Line
	10350 5100 10550 5100
Wire Wire Line
	10350 5200 10350 5100
Wire Wire Line
	10700 5100 10550 5100
Wire Wire Line
	10700 5200 10700 5100
$Comp
L power:+3.3V #PWR023
U 1 1 60114FFF
P 10550 5100
F 0 "#PWR023" H 10550 4950 50  0001 C CNN
F 1 "+3.3V" H 10565 5273 50  0000 C CNN
F 2 "" H 10550 5100 50  0001 C CNN
F 3 "" H 10550 5100 50  0001 C CNN
	1    10550 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	10700 5500 10700 5550
Wire Wire Line
	10350 5500 10350 5550
$Comp
L Device:R R11
U 1 1 6010667F
P 10700 5350
F 0 "R11" H 10630 5304 50  0000 R CNN
F 1 "10K" H 10630 5395 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 10630 5350 50  0001 C CNN
F 3 "~" H 10700 5350 50  0001 C CNN
	1    10700 5350
	-1   0    0    1   
$EndComp
$Comp
L Device:R R5
U 1 1 60105D40
P 10350 5350
F 0 "R5" H 10420 5396 50  0000 L CNN
F 1 "10K" H 10420 5305 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 10280 5350 50  0001 C CNN
F 3 "~" H 10350 5350 50  0001 C CNN
	1    10350 5350
	1    0    0    -1  
$EndComp
$Comp
L ESP8266:ESP-12E U4
U 1 1 6005CE39
P 8700 5200
F 0 "U4" H 8700 5965 50  0000 C CNN
F 1 "ESP-12E" H 8700 5874 50  0000 C CNN
F 2 "RF_Module:ESP-12E" H 8700 5200 50  0001 C CNN
F 3 "http://l0l.org.uk/2014/12/esp8266-modules-hardware-guide-gotta-catch-em-all/" H 8700 5200 50  0001 C CNN
	1    8700 5200
	1    0    0    -1  
$EndComp
Text Label 10350 5550 3    50   ~ 0
DTR
Text Label 10700 5550 3    50   ~ 0
DTX
Wire Notes Line
	6900 6450 11000 6450
Wire Notes Line
	11000 6450 11000 3900
Wire Notes Line
	11000 3900 6900 3900
Wire Notes Line
	6900 3900 6900 6450
Text Notes 10700 4100 2    50   ~ 0
ESP8266-12 WiFi module\n
$Comp
L Switch:SW_SPST SW2
U 1 1 6019137D
P 9100 1350
F 0 "SW2" H 9150 1500 50  0000 C CNN
F 1 "SW" H 9100 1250 50  0000 C CNN
F 2 "Button_Switch_SMD:SW_Push_1P1T_NO_6x6mm_H9.5mm" H 9100 1350 50  0001 C CNN
F 3 "~" H 9100 1350 50  0001 C CNN
	1    9100 1350
	1    0    0    -1  
$EndComp
Text Label 7650 4900 2    50   ~ 0
RST
$Comp
L power:GND #PWR021
U 1 1 60192D8E
P 9450 1450
F 0 "#PWR021" H 9450 1200 50  0001 C CNN
F 1 "GND" H 9455 1277 50  0000 C CNN
F 2 "" H 9450 1450 50  0001 C CNN
F 3 "" H 9450 1450 50  0001 C CNN
	1    9450 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	9300 1350 9450 1350
Wire Wire Line
	9450 1350 9450 1450
Wire Wire Line
	8900 1350 8800 1350
Text Label 8800 1350 2    50   ~ 0
RST
$Comp
L Connector:Conn_01x04_Male J3
U 1 1 6019FD31
P 10050 1450
F 0 "J3" H 10158 1731 50  0000 C CNN
F 1 "Prog" V 9950 1400 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 10050 1450 50  0001 C CNN
F 3 "~" H 10050 1450 50  0001 C CNN
	1    10050 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	10250 1350 10400 1350
Wire Wire Line
	10250 1450 10400 1450
Wire Wire Line
	10250 1550 10400 1550
Wire Wire Line
	10250 1650 10400 1650
Text Label 10400 1350 0    50   ~ 0
RST
Text Label 10400 1450 0    50   ~ 0
DTR
Text Label 10400 1550 0    50   ~ 0
TX
Text Label 10400 1650 0    50   ~ 0
RX
$Comp
L Connector:Conn_01x05_Male J4
U 1 1 600172A3
P 8250 2700
F 0 "J4" H 8358 3081 50  0000 C CNN
F 1 "CONN-1" H 8358 2990 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 8250 2700 50  0001 C CNN
F 3 "~" H 8250 2700 50  0001 C CNN
	1    8250 2700
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x05_Male J5
U 1 1 6001857C
P 9450 2700
F 0 "J5" H 9558 3081 50  0000 C CNN
F 1 "CONN-2" H 9558 2990 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 9450 2700 50  0001 C CNN
F 3 "~" H 9450 2700 50  0001 C CNN
	1    9450 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	8450 2500 8600 2500
Wire Wire Line
	8450 2600 8600 2600
Wire Wire Line
	8450 2700 8600 2700
Wire Wire Line
	8450 2800 8600 2800
Wire Wire Line
	8450 2900 8600 2900
Text Label 8600 2500 0    50   ~ 0
TX
Text Label 8600 2600 0    50   ~ 0
RX
Text Label 8600 2700 0    50   ~ 0
I2C_SCL
Text Label 8600 2800 0    50   ~ 0
I2C_SDA
$Comp
L power:GND #PWR024
U 1 1 6003F583
P 8600 2900
F 0 "#PWR024" H 8600 2650 50  0001 C CNN
F 1 "GND" H 8605 2727 50  0000 C CNN
F 2 "" H 8600 2900 50  0001 C CNN
F 3 "" H 8600 2900 50  0001 C CNN
	1    8600 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9650 2500 9800 2500
Wire Wire Line
	9650 2600 9800 2600
Wire Wire Line
	9650 2700 9800 2700
Wire Wire Line
	9650 2800 9800 2800
Wire Wire Line
	9650 2900 9800 2900
Text Label 9800 2500 0    50   ~ 0
SPI_MISO
Text Label 9800 2600 0    50   ~ 0
SPI_MOSI
Text Label 9800 2700 0    50   ~ 0
SPI_SCK
$Comp
L power:+3.3V #PWR025
U 1 1 600628E9
P 9800 2800
F 0 "#PWR025" H 9800 2650 50  0001 C CNN
F 1 "+3.3V" V 9815 2928 50  0000 L CNN
F 2 "" H 9800 2800 50  0001 C CNN
F 3 "" H 9800 2800 50  0001 C CNN
	1    9800 2800
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR026
U 1 1 60063776
P 9800 2900
F 0 "#PWR026" H 9800 2650 50  0001 C CNN
F 1 "GND" V 9805 2772 50  0000 R CNN
F 2 "" H 9800 2900 50  0001 C CNN
F 3 "" H 9800 2900 50  0001 C CNN
	1    9800 2900
	0    -1   -1   0   
$EndComp
Wire Notes Line
	7600 850  7600 3400
Wire Notes Line
	7600 3400 10850 3400
Wire Notes Line
	10850 3400 10850 850 
Wire Notes Line
	10850 850  7600 850 
Wire Wire Line
	1550 1100 1750 1100
$Comp
L power:PWR_FLAG #FLG01
U 1 1 60083AAC
P 1750 1100
F 0 "#FLG01" H 1750 1175 50  0001 C CNN
F 1 "PWR_FLAG" H 1750 1273 50  0000 C CNN
F 2 "" H 1750 1100 50  0001 C CNN
F 3 "~" H 1750 1100 50  0001 C CNN
	1    1750 1100
	1    0    0    -1  
$EndComp
Connection ~ 1750 1100
Wire Wire Line
	1750 1100 1800 1100
$Comp
L power:PWR_FLAG #FLG02
U 1 1 60084BC6
P 1900 4850
F 0 "#FLG02" H 1900 4925 50  0001 C CNN
F 1 "PWR_FLAG" V 1900 4978 50  0000 L CNN
F 2 "" H 1900 4850 50  0001 C CNN
F 3 "~" H 1900 4850 50  0001 C CNN
	1    1900 4850
	0    1    1    0   
$EndComp
Connection ~ 1900 4850
Wire Wire Line
	1900 4850 1900 4950
$EndSCHEMATC
