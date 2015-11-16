#include "robotarm.h"

// ---------- UART Interrupt --------------------------
//
// Reads in character from UART serial port and sends it
// to testInput() to be processed.
//
// ----------------------------------------------------
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
	volatile char inChar;
	inChar = UCA0RXBUF;
	testInput(inChar);
}


// ------- Timer A1.0 ISR ------------------------------
//
// Timer_A 1.0 is the period for the PWM
// This ISR
//
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMERA1_ISR(void)
{
	TA1CCTL1 = OUTMOD_7 + CCIE;
	P2SEL = BIT1;
	TA1CCR1 = servoPosition[0];

}//timerA1.0 interrupt()



// ------- Timer_A 1.1 + 1.2 ISR ------------------------------
#pragma vector=TIMER1_A1_VECTOR
__interrupt void TIMERA11_ISR(void)
{
	switch(TA1IV){
	case 2:
		TA1CCTL1 = OUTMOD_1;
		P2SEL = BIT2;
		TA1CCR1 = PWM_PERIOD - servoPosition[1];
		break;
	default:
		break;
	}

}//timerA1.1 interrupt()
