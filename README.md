# Haptic Blocks Wearable

This project is intended to provide the public with the means to quickly and easily implement their own solutions to providing haptic (tactile to be precise) feedback for their own projects. Wether you want to immerse users in a great game or simply do research on haptics, HBW might be your solution. 

 A variety of 3D printed "blocks" are used to construct a wearable apparatus containing tactile actuators. These actuators are then controlled via an API by WiFi.  The entire system is designed to be open and low cost, and provided under the Unlicense. 

## Why use HBW? 
Many solutions exist to provide tactile feedback in computer programs. Some are professional products and quite expensive, others are free and low cost, but use proprietary protocols for control and are therefore not suitable for all use cases. 

Haptics is a "bodily" field of research - and diverse people have diverse bodies. Therefore, HBW aims to deliver simple costumisation for different sizes of bodies, and allows tactile actuators to be placed in different locations on your users' bodies. 

Furthermore, HBW goes beyond the usual limitations of merely providing vibrotactile feedback and enables you to also use (or experiment with) thermoreceptive feedback by use of fans, and touch the user by implementing small servos. 

# Let's get started

## Materials you'll need 

### A 3D printer
The components for your own wearable will be 3D printed. This repository provides both STL and STEP formats, allowing you to either just print the provided files or alter them according to your needs. STL files are already in the correct orientation and designed to require minimal supports. Please note that the parts are designed for FDM printing, though you are of course free to use any other technology. 

In order to offer the best experience to your users, post-process the printed parts to the best of your ability. Always remember that users might wear your project for extended periods of time. 

**As the HBW is intended to be worn on the skin, please make sure to avoid using toxic materials. 
If you aim to have many people use your wearable, please consider hygiene.** 

### The actuators 
HBW is designed to use common components so you'll spend less time sourcing and more time creating great haptics applications. 

You may directly buy the components using the links provided, or find alternate sources. 
Please note that the provision of links is not an endorsement of the companies named.
 I do not use affiliate links. 

|Actuator| Use |Amazon US Link| AliExpress Link |
|--|--|--|--|
|  Fan Type 5015, 5V version | Provides thermic feedback, controlled via motor driver . |||
|  Servo MG90| The classic RC-Servo can be controlled directly by the ESP32 and provides touch feedback. It is chosen for its relatively low strength in order not to hurt the users. |||
| Coin-shaped vibration motor with control board | This little motor provides weak vibrotactile feedback. It can be controlled directly by the ESP32. The "coin" has a diameter  of 10mm.  |||
| Eccentric Servo Motor  | This motor is able to provide strong vibrotactile feedback and is controlled via a motor driver. It has a diameter of 12mm and is 24mm long.  |||

### Further components
|Component| Use |Amazon US Link| AliExpress Link |
|--|--|--|--|
| ESP32  | The HBW has been initially implemented on an ESP32 Wroom, though any other ESP32 should also work. The ESP32 is the "brains" of the HBW, it provides (or connects to) WiFi and controls the actuators.  ||  |
| Motor Driver L298N | As stated above, some of the actuators can not be controlled directly by the ESP32 - they require this motor driver to run.  ||  |
|A PCB  |Depending on your requirements, you can either solder your own PCB or design one and have it manufactured. During development, a 7x9 through hole plate and Dupont-headers were used.  |None| None |
| Velcro-belt | In order to put on the wearable easily, it is recommended to use velcro. The belt should be 25mm wide.  ||  |
| Flexible belt| In order to allow the wearable to stretch and contract depending on posture, you may use belt made of a flexible material. The belt should also be 25mm wide.  ||  |
| Filament| In order to connect the different components, filament (1.75mm) is used. If you use a printing technology different from FDM and therefore do not have filament, M2 Screws or similar rods may be substituted.  ||  |

## Building the HBW

### 1: Printing and assembling the hardware
Choose which actuators you want to use and print the required blocks. The blocks are held together by connector pieces or the abovementioned belts. 
In order to connect the components, pieces of filament are used in order to minimize friction. Insert the filament through the holes on the blocks and push them through the connectors. Continue in this fashion until all printed components form a belt, ring or bracelet
To be able to wear the HBW, one side of the wearable should be open and connected to the other side either by a flexible belt or by velcro. To hold the belts in place, either sew them together or use a stapler.
If using velcro, use a closed loop instead of separate pieces.

### 2: Assemble the PCB
Connect the different actuators to whatever PCB you are using. Larger motors connect to the motor drivers, smaller ones and servos tie in directly to the ESP32. 


### 3: Preparing and flashing the software
There are several alterations you might have to perform in "HBW.ino" in order to adapt the code to your need. 


| Variable | Explanation  |
|--|--|
| bool isLocal | Set isLocal to "true" if the ESP32 should host its' own network. Then connect the controlling device to that network in order to control the HBW - its' IP will be "192.168.4.1". The fields "ssidLocal" and "passwordLocal" determine the name and password of said network. If isLocal is set to false, the wearable will connect to an existing network determined by the values of "ssid" and "password". The HBW will print the assigned IP to the Serial Console on startup. |
|const int vibmotors[]  | Write the pins connected to all non-servo motors into this array. The motors will be accesible in the API by their position in this array, so make note which one is connected to which pin. |
|const int servopins[]   |  Write the pins connected to all servo motors into this array. The motors will be accesible in the API by their position in this array, so make note which one is connected to which pin.  |

Use the Arduino IDE to write the software to the ESP32. 
**You are now ready to use the HBW.**

## Using the HBW

### The API 
The HBW is controlled by a very simple API, using only two paths. 
|Path|Parameters  |Explanation|Example|
|--|--|--|--|
| ip/vibration?motorNum=X&intensity=Y | motorNum and intensity|motorNum can be either a single int (1) or an array of ints separated by commas in square brackets ([0,1,2,3]). motorNum determines the position of the pin(s) in the vibmotors array which will then receive the PWM-value specified in intensity. The square brackets approach was chosen to minimize the amount of API-calls needed|*/vibration?motorNum=[0,1]&intensity=255* will make motors on position 0 and 1 in vibmotors vibrate at full strength.  |
| ip/servo?motorNum=X&position=Y | motorNum and position|motorNum can be either a single int (1) or an array of ints separated by commas in square brackets ([0,1,2,3]). motorNum determines the position of the pin(s) in the servopins array which will then move to the position specified in position. |*/servo?motorNum=[0,1]&position=170* will make servos on position 0 and 1 in servopins move to the 170 degree position.  |

### Calling the API

#### Python 
In Python, use the requests library to make calls to a URL. 
```  
import requests
#example for a wrapping function to make the motors vibrate 
def vibrate(motor, intensity):
	# replace x.x.x.x with the desired ip 
    url =" http://x.x.x.x/vibration?motorNum=" + str(motor) + "&intensity="+str(intensity)
    response = requests.get(url)

#example call - makes motors 1,2,3 vibrate at full power
vibrate("[1,2,3]", 255)
```
You may use time.sleep to space the commands in order to create patterns. 





