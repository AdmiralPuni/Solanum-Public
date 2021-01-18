# Solanum-Public
Arduino movement control using rangefinders and L298N motor controller with task input, use serial communication program to communicate with the Arduino

Main code should be readable enough, some changes to variable names might take place soon

|Serial Command | Function
|--- | --- |
|8 | Start task input mode |
|9 | End task input and execute |
|10, 11 , 20, 21, 30, 31 ,40 ,41 | Motor controls |
|99 | Cancel all command (excludes task) |
|1, 2 | Measure distance |
|4 | Show current task (useful in task inout mode) |
