#include "robotarm.h"

void testInput(char inChar){

	volatile int i;

	switch(inChar){
	case 's':
		servoPosition[0] += 50;
		break;
	case 'w':
		servoPosition[0] -= 50;
		break;
	case 'd':
		servoPosition[1] += 50;
		break;
	case 'a':
		servoPosition[1] -= 50;
		break;
	default:
		messageOut("Invalid Key",11);
		break;
	}//switch

	for(i = 0; i < 4; i++){
		if(servoPosition[i] < 900){
			servoPosition[i] = 900;
			messageOut("Hit High Edge",13);
		}
		if(servoPosition[i] > 2000){
			servoPosition[i]=2000;
			messageOut("Hit Low Edge",12);
		}
	}
}//testInput()

void messageOut(char messageIn[], int length){

	volatile int i;
	for(i = 0; i < length; i++){
		while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
		UCA0TXBUF = messageIn[i];
	}//for

	while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
	UCA0TXBUF = 0x0A;
	while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
	UCA0TXBUF = 0x0D;

}//outputMessage()

