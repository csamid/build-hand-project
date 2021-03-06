# CSULB BUILD Hand Project
this repo contains:
1) Grip Load Device
2) Brace
3) Knob Device
4) Hapkit

# GRIP LOAD DEVICE
The grip-load device (GLD) runs on a arduino uno which reads data from 3 sensors. A MPU6050 for acceleration which is used to obtain load force during the dynamic test. A LCFD-50 load cell to acquire grip force. Finally, a LSB302 Futek load cell to get load force during the static tests. The purpose of this device is to assess the differences in grip-load force coordination between healthy and hand impaired individuals. We also hope this device can be used for rehabilitative training of individuals with hand impairment. This repo contains the code, documentation, and parts for this device (Last Update: 6/27/2022 from csamid). The data collection is done on LabVIEW.
Videos explaining how to download code, how arduino & LabVIEW code works, and how run the code can be found here: https://www.youtube.com/playlist?list=PLc3eWZpUKAzyQBUwwpyzYRb3LxGoiJ5aO 

A planned user study will involve a:

Dynamic Test
  - the user performs a lift-hold-lower task based on a metronome
  ![image](https://user-images.githubusercontent.com/85626643/176099929-5e86ee02-7bb1-499f-91a9-f4ee0fa400f1.png)
  
Static Ramp Test
  - the user tracks a ramp force trajectory by pulling on the device
  ![image](https://user-images.githubusercontent.com/85626643/176100437-8bb12e90-0106-477e-bd89-19ee52723d55.png)
  
Static Sine Test
  - user performs a pull and push task based on a metronome and tries to reach min and max force as a beat occurs
  ![image](https://user-images.githubusercontent.com/85626643/176100964-ee5dc6c3-88e7-4444-a6d7-b5ff76effa38.png)
# 
Futek Load Cell Wire Connection:
![IMG-0377](https://user-images.githubusercontent.com/85626643/176093601-97614e21-ff5e-4d98-8d21-9f3b018974cd.jpg)

Feel free to modify and improve the device :)

# Brace
A 2 piece 3d printed adjustable brace to explore the effects of wrist position on grip-load force coordination. It can be set at -30, 0, and 30 degrees at the wrist.

# Knob Device
The knob device (KD) is centered arduino uno as it sends data to a graphical user interface (GUI) and controls a haptic interface. The goal of this device is to assess the coordination b/w grasping force and turning torque and also provide rehabilitative training. The GUI and data acquisition is on LabVIEW. The haptic interface inculdes a HEDS encoder, maxon A-max motor, force sensing resistors (FSR), and a torque sensor. The code from this device has not been updated since mid 2021 so it can use some improvements. See the GLD LabVIEW code for improving the LabVIEW code. Improvements can be made on the arduino code to add/change the haptic environment (ie Spring and Damper). Hardware/Sensor improvements are also welcome, since FSR's are not really reliable quantitatively. 

# Hapkit
Some design changes were made to the open-source Hapkit 10.01.14 from Stanford to fit with available hardware at the RISE Lab @CSULB 
