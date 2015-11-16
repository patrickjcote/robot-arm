//Includes
#include  <msp430.h>

//Defines
#define PWM_PERIOD 20000

//Prototypes
void initialize();
void testInput(char inChar);
void messageOut(char messageIn[], int length);

//Global
extern volatile int servoPosition[4];


//robotarm.h
