# robot-arm

CSCI 255 - Intro to Embedded Systems

Code for a robot arm controlled by an MSP430 microcontroller

Using WASD and IJKL all servos can be controlled using one microcontroller.

By sending two duty cycles on separate pins during one period it is possible to
control 4 servos using TA1.0 as the PWM period and TA1.1 and 1.2 for the duty
cylces.
