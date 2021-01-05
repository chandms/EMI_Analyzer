EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "EMI Analyzer"
Date "2021-01-04"
Rev "A"
Comp "Purdue SMART Lab"
Comment1 "Author: Henry Silva"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L symbols:AD5933 U2
U 1 1 5FF3829F
P 4150 2400
F 0 "U2" H 4400 2800 50  0000 C CNN
F 1 "AD5933" H 4000 2800 50  0000 C CNN
F 2 "Package_SO:SSOP-16_5.3x6.2mm_P0.65mm" H 4150 1950 50  0001 C CNN
F 3 "https://www.analog.com/media/en/technical-documentation/data-sheets/AD5933.pdf" H 4550 3250 50  0001 C CNN
	1    4150 2400
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C2
U 1 1 5FF39E41
P 4850 2300
F 0 "C2" H 4942 2346 50  0000 L CNN
F 1 "10uF" H 4942 2255 50  0000 L CNN
F 2 "" H 4850 2300 50  0001 C CNN
F 3 "~" H 4850 2300 50  0001 C CNN
	1    4850 2300
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C3
U 1 1 5FF3A2DE
P 5300 2300
F 0 "C3" H 5392 2346 50  0000 L CNN
F 1 "0.1uF" H 5392 2255 50  0000 L CNN
F 2 "" H 5300 2300 50  0001 C CNN
F 3 "~" H 5300 2300 50  0001 C CNN
	1    5300 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 2050 5300 2150
Wire Wire Line
	5300 2150 4850 2150
Wire Wire Line
	4850 2150 4850 2200
Connection ~ 5300 2150
Wire Wire Line
	5300 2150 5300 2200
Wire Wire Line
	4600 2150 4600 2250
Wire Wire Line
	4600 2250 4550 2250
Wire Wire Line
	4600 2150 4550 2150
Wire Wire Line
	4600 2250 4600 2350
Wire Wire Line
	4600 2350 4550 2350
Connection ~ 4600 2250
$Comp
L power:GNDA #PWR0101
U 1 1 5FF3CDC7
P 5300 2550
F 0 "#PWR0101" H 5300 2300 50  0001 C CNN
F 1 "GNDA" H 5305 2377 50  0000 C CNN
F 2 "" H 5300 2550 50  0001 C CNN
F 3 "" H 5300 2550 50  0001 C CNN
	1    5300 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 2400 4850 2450
Wire Wire Line
	4850 2450 5300 2450
Wire Wire Line
	5300 2450 5300 2400
Wire Wire Line
	5300 2450 5300 2550
Connection ~ 5300 2450
Wire Wire Line
	4600 2450 4600 2550
Wire Wire Line
	4600 2550 4550 2550
Wire Wire Line
	4600 2450 4550 2450
Wire Wire Line
	4600 2550 4600 2650
Wire Wire Line
	4600 2650 4550 2650
Connection ~ 4600 2550
Wire Wire Line
	4600 2150 4850 2150
Connection ~ 4600 2150
Connection ~ 4850 2150
Wire Wire Line
	4600 2450 4850 2450
Connection ~ 4600 2450
Connection ~ 4850 2450
$Comp
L power:+3V3 #PWR0102
U 1 1 5FF41C41
P 5300 2050
F 0 "#PWR0102" H 5300 1900 50  0001 C CNN
F 1 "+3V3" H 5315 2223 50  0000 C CNN
F 2 "" H 5300 2050 50  0001 C CNN
F 3 "" H 5300 2050 50  0001 C CNN
	1    5300 2050
	1    0    0    -1  
$EndComp
$Comp
L symbols:AD8606 U1
U 1 1 5FF45C43
P 2450 3050
F 0 "U1" H 2550 2900 50  0000 C CNN
F 1 "AD8606" H 2500 3250 50  0000 C CNN
F 2 "Package_SO:MSOP-8_3x3mm_P0.65mm" H 2350 2700 50  0001 C CNN
F 3 "https://www.analog.com/media/en/technical-documentation/data-sheets/AD8605_8606_8608.pdf" H 2450 3050 50  0001 C CNN
	1    2450 3050
	-1   0    0    1   
$EndComp
$Comp
L symbols:AD8606 U1
U 2 1 5FF46321
P 2450 1550
F 0 "U1" H 2400 1300 50  0000 C CNN
F 1 "AD8606" H 2450 1800 50  0000 C CNN
F 2 "Package_SO:MSOP-8_3x3mm_P0.65mm" H 2350 1200 50  0001 C CNN
F 3 "https://www.analog.com/media/en/technical-documentation/data-sheets/AD8605_8606_8608.pdf" H 2450 1550 50  0001 C CNN
	2    2450 1550
	1    0    0    1   
$EndComp
$Comp
L symbols:AD8606 U1
U 3 1 5FF469E3
P 6700 1350
F 0 "U1" H 6658 1396 50  0000 L CNN
F 1 "AD8606" H 6658 1305 50  0000 L CNN
F 2 "Package_SO:MSOP-8_3x3mm_P0.65mm" H 6600 1000 50  0001 C CNN
F 3 "https://www.analog.com/media/en/technical-documentation/data-sheets/AD8605_8606_8608.pdf" H 6700 1350 50  0001 C CNN
	3    6700 1350
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R7
U 1 1 5FF4A655
P 3300 2150
F 0 "R7" V 3095 2150 50  0000 C CNN
F 1 "20k" V 3186 2150 50  0000 C CNN
F 2 "" H 3300 2150 50  0001 C CNN
F 3 "~" H 3300 2150 50  0001 C CNN
	1    3300 2150
	0    1    1    0   
$EndComp
Wire Wire Line
	3400 2150 3750 2150
$Comp
L Device:R_Small_US R4
U 1 1 5FF4B4FE
P 2900 1800
F 0 "R4" H 2832 1754 50  0000 R CNN
F 1 "20k" H 2832 1845 50  0000 R CNN
F 2 "" H 2900 1800 50  0001 C CNN
F 3 "~" H 2900 1800 50  0001 C CNN
	1    2900 1800
	-1   0    0    1   
$EndComp
Wire Wire Line
	3200 2150 2900 2150
Wire Wire Line
	2900 2150 2900 1900
Wire Wire Line
	3750 2250 2900 2250
Wire Wire Line
	2900 2250 2900 2150
Connection ~ 2900 2150
Wire Wire Line
	2900 1700 2900 1550
Wire Wire Line
	2900 1550 2750 1550
$Comp
L Device:R_Small_US R3
U 1 1 5FF4DC84
P 2400 1050
F 0 "R3" V 2605 1050 50  0000 C CNN
F 1 "RFB" V 2514 1050 50  0000 C CNN
F 2 "" H 2400 1050 50  0001 C CNN
F 3 "~" H 2400 1050 50  0001 C CNN
	1    2400 1050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2900 1550 2900 1050
Wire Wire Line
	2900 1050 2500 1050
Connection ~ 2900 1550
Wire Wire Line
	2300 1050 2050 1050
Wire Wire Line
	2050 1050 2050 1450
Wire Wire Line
	2050 1450 2150 1450
Wire Wire Line
	2150 1650 2050 1650
$Comp
L Device:R_Small_US R1
U 1 1 5FF50727
P 1700 1600
F 0 "R1" H 1632 1554 50  0000 R CNN
F 1 "49.9k" H 1632 1645 50  0000 R CNN
F 2 "" H 1700 1600 50  0001 C CNN
F 3 "~" H 1700 1600 50  0001 C CNN
	1    1700 1600
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small_US R2
U 1 1 5FF5117E
P 1700 2200
F 0 "R2" H 1632 2154 50  0000 R CNN
F 1 "49.9k" H 1632 2245 50  0000 R CNN
F 2 "" H 1700 2200 50  0001 C CNN
F 3 "~" H 1700 2200 50  0001 C CNN
	1    1700 2200
	-1   0    0    1   
$EndComp
Wire Wire Line
	1700 2100 1700 1900
Wire Wire Line
	1700 1900 2050 1900
Connection ~ 1700 1900
Wire Wire Line
	1700 1900 1700 1700
$Comp
L power:+3V3 #PWR0103
U 1 1 5FF52DB6
P 1700 1400
F 0 "#PWR0103" H 1700 1250 50  0001 C CNN
F 1 "+3V3" H 1715 1573 50  0000 C CNN
F 2 "" H 1700 1400 50  0001 C CNN
F 3 "" H 1700 1400 50  0001 C CNN
	1    1700 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 1400 1700 1500
$Comp
L power:GNDA #PWR0104
U 1 1 5FF53E9C
P 1700 2450
F 0 "#PWR0104" H 1700 2200 50  0001 C CNN
F 1 "GNDA" H 1705 2277 50  0000 C CNN
F 2 "" H 1700 2450 50  0001 C CNN
F 3 "" H 1700 2450 50  0001 C CNN
	1    1700 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 2450 1700 2300
Wire Wire Line
	2050 1650 2050 1900
$Comp
L Connector:Conn_Coaxial J1
U 1 1 5FF57137
P 1150 2200
F 0 "J1" H 1250 2082 50  0000 L CNN
F 1 "Conn_Coaxial" H 1250 2173 50  0000 L CNN
F 2 "" H 1150 2200 50  0001 C CNN
F 3 " ~" H 1150 2200 50  0001 C CNN
	1    1150 2200
	-1   0    0    1   
$EndComp
Connection ~ 2050 1050
Wire Wire Line
	1150 1050 2050 1050
$Comp
L Device:C_Small C1
U 1 1 5FF5B85A
P 3300 3150
F 0 "C1" V 3050 3100 50  0000 L CNN
F 1 "47nF" V 3150 3050 50  0000 L CNN
F 2 "" H 3300 3150 50  0001 C CNN
F 3 "~" H 3300 3150 50  0001 C CNN
	1    3300 3150
	0    1    1    0   
$EndComp
Wire Wire Line
	3750 2350 3450 2350
$Comp
L Device:R_Small_US R5
U 1 1 5FF681D4
P 2900 2850
F 0 "R5" H 2832 2804 50  0000 R CNN
F 1 "49.9k" H 2832 2895 50  0000 R CNN
F 2 "" H 2900 2850 50  0001 C CNN
F 3 "~" H 2900 2850 50  0001 C CNN
	1    2900 2850
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small_US R6
U 1 1 5FF681DA
P 2900 3450
F 0 "R6" H 2832 3404 50  0000 R CNN
F 1 "49.9k" H 2832 3495 50  0000 R CNN
F 2 "" H 2900 3450 50  0001 C CNN
F 3 "~" H 2900 3450 50  0001 C CNN
	1    2900 3450
	-1   0    0    1   
$EndComp
$Comp
L power:+3V3 #PWR0105
U 1 1 5FF681E4
P 2900 2650
F 0 "#PWR0105" H 2900 2500 50  0001 C CNN
F 1 "+3V3" H 2915 2823 50  0000 C CNN
F 2 "" H 2900 2650 50  0001 C CNN
F 3 "" H 2900 2650 50  0001 C CNN
	1    2900 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 2650 2900 2750
$Comp
L power:GNDA #PWR0106
U 1 1 5FF681EB
P 2900 3700
F 0 "#PWR0106" H 2900 3450 50  0001 C CNN
F 1 "GNDA" H 2905 3527 50  0000 C CNN
F 2 "" H 2900 3700 50  0001 C CNN
F 3 "" H 2900 3700 50  0001 C CNN
	1    2900 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 3700 2900 3550
Wire Wire Line
	2900 2950 2900 3150
Wire Wire Line
	3450 3150 3400 3150
Wire Wire Line
	3200 3150 2900 3150
Connection ~ 2900 3150
Wire Wire Line
	2900 3150 2900 3350
Wire Wire Line
	3450 2350 3450 3150
Wire Wire Line
	2750 3150 2900 3150
Wire Wire Line
	2750 2950 2800 2950
Wire Wire Line
	2800 2950 2800 2800
Wire Wire Line
	2800 2800 2050 2800
Wire Wire Line
	2050 2800 2050 3050
Wire Wire Line
	2050 3050 2150 3050
Wire Wire Line
	2050 3050 1450 3050
Connection ~ 2050 3050
Wire Wire Line
	1150 1050 1150 2000
Wire Wire Line
	1450 3050 1450 2200
Wire Wire Line
	1450 2200 1350 2200
$Comp
L Device:C_Small C5
U 1 1 5FF84B6F
P 6250 1350
F 0 "C5" H 6100 1250 50  0000 L CNN
F 1 "10uF" H 6000 1450 50  0000 L CNN
F 2 "" H 6250 1350 50  0001 C CNN
F 3 "~" H 6250 1350 50  0001 C CNN
	1    6250 1350
	-1   0    0    1   
$EndComp
$Comp
L Device:C_Small C4
U 1 1 5FF8585A
P 6000 1350
F 0 "C4" H 5850 1450 50  0000 L CNN
F 1 "0.1uF" H 5750 1250 50  0000 L CNN
F 2 "" H 6000 1350 50  0001 C CNN
F 3 "~" H 6000 1350 50  0001 C CNN
	1    6000 1350
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR0107
U 1 1 5FF89B37
P 6600 950
F 0 "#PWR0107" H 6600 800 50  0001 C CNN
F 1 "+3V3" H 6615 1123 50  0000 C CNN
F 2 "" H 6600 950 50  0001 C CNN
F 3 "" H 6600 950 50  0001 C CNN
	1    6600 950 
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR0108
U 1 1 5FF9113A
P 6600 1750
F 0 "#PWR0108" H 6600 1500 50  0001 C CNN
F 1 "GNDA" H 6605 1577 50  0000 C CNN
F 2 "" H 6600 1750 50  0001 C CNN
F 3 "" H 6600 1750 50  0001 C CNN
	1    6600 1750
	1    0    0    -1  
$EndComp
Text GLabel 3750 2650 0    50   Input ~ 0
SCL
Text GLabel 3750 2550 0    50   Input ~ 0
SDA
Wire Wire Line
	6600 950  6600 1000
Wire Wire Line
	6600 1000 6250 1000
Wire Wire Line
	6000 1000 6000 1250
Connection ~ 6600 1000
Wire Wire Line
	6600 1000 6600 1050
Wire Wire Line
	6250 1250 6250 1000
Connection ~ 6250 1000
Wire Wire Line
	6250 1000 6000 1000
Wire Wire Line
	6250 1450 6250 1700
Wire Wire Line
	6250 1700 6600 1700
Wire Wire Line
	6600 1700 6600 1650
Wire Wire Line
	6600 1750 6600 1700
Connection ~ 6600 1700
Wire Wire Line
	6250 1700 6000 1700
Wire Wire Line
	6000 1700 6000 1450
Connection ~ 6250 1700
Text Notes 550  650  0    94   ~ 0
Impedance Analyzer and Analog Front End
Wire Notes Line
	500  3950 5650 3950
Wire Notes Line
	5650 3950 5650 500 
Text Notes 5700 650  0    94   ~ 0
Opamp Power
Wire Notes Line
	5650 2050 7000 2050
Wire Notes Line
	7000 2050 7000 500 
NoConn ~ 3750 2450
$EndSCHEMATC
