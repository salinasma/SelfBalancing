EESchema Schematic File Version 4
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L SelfBalancing:Node_Mcu_Esp32_Dev_Board U0
U 1 1 5F0A868A
P 5750 3900
F 0 "U0" H 5750 5065 50  0000 C CNN
F 1 "Node_Mcu_Esp32_Dev_Board" H 5750 4974 50  0000 C CNN
F 2 "" H 5750 4950 50  0001 C CNN
F 3 "" H 5750 4950 50  0001 C CNN
	1    5750 3900
	1    0    0    -1  
$EndComp
$Comp
L SelfBalancing:3081_Polulu_Motor_Encoder U0
U 1 1 5F0A8BC3
P 9050 3950
F 0 "U0" H 9000 4450 50  0000 L CNN
F 1 "Right 3081_Polulu_Motor_Encoder" H 8550 4350 50  0000 L CNN
F 2 "" H 9050 4350 50  0001 C CNN
F 3 "" H 9050 4350 50  0001 C CNN
	1    9050 3950
	1    0    0    -1  
$EndComp
$Comp
L SelfBalancing:3081_Polulu_Motor_Encoder U1
U 1 1 5F0A9006
P 9050 4950
F 0 "U1" H 9000 5450 50  0000 L CNN
F 1 "Left 3081_Polulu_Motor_Encoder" H 8550 5350 50  0000 L CNN
F 2 "" H 9050 5350 50  0001 C CNN
F 3 "" H 9050 5350 50  0001 C CNN
	1    9050 4950
	1    0    0    -1  
$EndComp
$Comp
L SelfBalancing:2130_Polulu_Motor_Driver U0
U 1 1 5F0A978D
P 8950 2850
F 0 "U0" H 8950 3465 50  0000 C CNN
F 1 "2130_Polulu_Motor_Driver" H 8950 3374 50  0000 C CNN
F 2 "" H 5800 5650 50  0001 C CNN
F 3 "" H 5800 5650 50  0001 C CNN
	1    8950 2850
	1    0    0    -1  
$EndComp
$Comp
L SelfBalancing:2851_Polulu_V_Regulator U0
U 1 1 5F0AA0C7
P 2350 4600
F 0 "U0" H 2300 5200 50  0000 L CNN
F 1 "2851_Polulu_V_Regulator" H 1900 5100 50  0000 L CNN
F 2 "" H 2350 4950 50  0001 C CNN
F 3 "" H 2350 4950 50  0001 C CNN
	1    2350 4600
	-1   0    0    -1  
$EndComp
$Comp
L SelfBalancing:HiLetGo_MPU_6050_IMU U0
U 1 1 5F0AA9BE
P 2300 3350
F 0 "U0" H 2250 3950 50  0000 L CNN
F 1 "HiLetGo_MPU_6050_IMU" H 1850 3850 50  0000 L CNN
F 2 "" H 950 4650 50  0001 C CNN
F 3 "" H 950 4650 50  0001 C CNN
	1    2300 3350
	-1   0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR?
U 1 1 5F0B1E9D
P 5800 6000
F 0 "#PWR?" H 5800 5750 50  0001 C CNN
F 1 "GNDREF" H 5805 5827 50  0000 C CNN
F 2 "" H 5800 6000 50  0001 C CNN
F 3 "" H 5800 6000 50  0001 C CNN
	1    5800 6000
	1    0    0    -1  
$EndComp
Text GLabel 5800 5900 1    50   Input ~ 0
GND
Wire Wire Line
	5800 5900 5800 6000
Text GLabel 8400 4700 0    50   Input ~ 0
GND
Text GLabel 8400 3700 0    50   Input ~ 0
GND
Text GLabel 8400 2500 0    50   Input ~ 0
GND
Wire Wire Line
	8500 3700 8400 3700
Text GLabel 6900 3000 2    50   Input ~ 0
GND
Wire Wire Line
	6900 3000 6800 3000
Text GLabel 4600 4300 0    50   Input ~ 0
GND
Text GLabel 2900 4600 2    50   Input ~ 0
GND
Text GLabel 2900 4700 2    50   Input ~ 0
GND
Text GLabel 2900 3100 2    50   Input ~ 0
GND
Wire Wire Line
	2800 3100 2900 3100
Wire Wire Line
	2800 4600 2900 4600
Wire Wire Line
	2800 4700 2900 4700
Wire Wire Line
	4600 4300 4700 4300
Wire Wire Line
	8400 4700 8500 4700
$Comp
L power:+BATT #PWR?
U 1 1 5F0B301D
P 5300 5800
F 0 "#PWR?" H 5300 5650 50  0001 C CNN
F 1 "+BATT" H 5315 5973 50  0000 C CNN
F 2 "" H 5300 5800 50  0001 C CNN
F 3 "" H 5300 5800 50  0001 C CNN
	1    5300 5800
	1    0    0    -1  
$EndComp
Text GLabel 5300 5900 3    50   Input ~ 0
VBATT
Wire Wire Line
	5300 5900 5300 5800
Text GLabel 2900 4500 2    50   Input ~ 0
VBATT
Wire Wire Line
	2800 4500 2900 4500
Text GLabel 2900 4800 2    50   Input ~ 0
VCC
Text GLabel 6900 3600 2    50   Input ~ 0
NC
Wire Wire Line
	6900 3600 6800 3600
Wire Wire Line
	2900 4800 2800 4800
Text GLabel 4600 4800 0    50   Input ~ 0
VCC
Wire Wire Line
	4600 4800 4700 4800
Text GLabel 9500 2500 2    50   Input ~ 0
GND
Wire Wire Line
	9400 2500 9500 2500
Text GLabel 9500 2600 2    50   Input ~ 0
VBATT
Wire Wire Line
	9400 2600 9500 2600
Text GLabel 4600 3000 0    50   Input ~ 0
3V3
Wire Wire Line
	4600 3000 4700 3000
Text GLabel 8400 2600 0    50   Input ~ 0
3V3
Wire Wire Line
	8400 2600 8500 2600
Text GLabel 8400 2700 0    50   Input ~ 0
RIGHT_MOTOR_IN1
Text GLabel 8400 3800 0    50   Input ~ 0
RIGHT_MOTOR_IN1
Wire Wire Line
	8400 3800 8500 3800
Text GLabel 8400 3900 0    50   Input ~ 0
RIGHT_MOTOR_IN2
Text GLabel 8400 2800 0    50   Input ~ 0
RIGHT_MOTOR_IN2
Text GLabel 8400 2900 0    50   Input ~ 0
LEFT_MOTOR_IN1
Text GLabel 8400 3000 0    50   Input ~ 0
LEFT_MOTOR_IN2
Wire Wire Line
	8400 2700 8500 2700
Wire Wire Line
	8400 2800 8500 2800
Wire Wire Line
	8400 2900 8500 2900
Wire Wire Line
	8400 3000 8500 3000
Wire Wire Line
	8400 3900 8500 3900
Text GLabel 8400 4800 0    50   Input ~ 0
LEFT_MOTOR_IN1
Text GLabel 8400 4900 0    50   Input ~ 0
LEFT_MOTOR_IN2
Wire Wire Line
	8400 4800 8500 4800
Wire Wire Line
	8400 4900 8500 4900
Text GLabel 8400 4000 0    50   Input ~ 0
3V3
Wire Wire Line
	8400 4000 8500 4000
Text GLabel 8400 5000 0    50   Input ~ 0
3V3
Wire Wire Line
	8400 5000 8500 5000
Wire Wire Line
	8400 2500 8500 2500
$EndSCHEMATC
