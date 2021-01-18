# Solanum-Public - Rangefinder and motor controls
Arduino movement control using rangefinders and L298N motor controller with task input, use serial communication program to communicate with the Arduino

Main code should be readable enough, some changes to variable names might take place soon. Rangefinder measurement results is used as coordinate for the machine to follow, changes can be made to make it use 1/2/3 dimensional coordinate, depends on the needs.

|Serial Command | Function
|--- | --- |
|8 | Start task input mode |
|9 | End task input and execute |
|10, 11 , 20, 21, 30, 31 ,40 ,41 | Motor controls |
|99 | Cancel all command (excludes task) |
|1, 2 | Measure distance |
|4 | Show current task (useful in task inout mode) |

![alt text](https://files.catbox.moe/et810s.png)
Code can be used on system needing to locate things precisely (precision depends on measurement units and rangefinder strength), removing the needs of stepper motor and its controller
