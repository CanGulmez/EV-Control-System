# EV Control System

This project was created for Teknofest - Efficiency Challenge Competition

EV Control System is a linux based control unit that works with BeagleBone
Black Rev C. In KiCAD design, there is a cape for BBB and includes two
important components. First is the LoRa transmitter that is responsible 
for sending the telemetry data to ground station. Second component is the
CAN transciever. It will listen the CAN bus that connects the EV control 
system with subsystems. It can be expanded according to requirements. 

In firmware design, the POSIX shared memory was created to provides 
communication between the programs. Many programs was designed for each
specific task so that if new programs are required, can be imported
easily.