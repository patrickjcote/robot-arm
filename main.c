//Includes
#include "robotarm.h"

//Global Variables
volatile int servoPosition[4] = {1500, 1500, 1500, 1500};

int main(void)
{

	initialize();

	__bis_SR_register(GIE);

}


void initialize(){

	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT


	//--------------------- Serial Comm Setup -------------------------
	if (CALBC1_1MHZ==0xFF)					// If calibration constant erased
	{
		while(1);                               // do not load, trap CPU!!
	}
	DCOCTL = 0;                               // Select lowest DCOx and MODx settings
	BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
	DCOCTL = CALDCO_1MHZ;
	P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
	P1SEL2 = BIT1 + BIT2 ;                    // P1.1 = RXD, P1.2=TXD
	UCA0CTL1 |= UCSSEL_2;                     // SMCLK
	UCA0BR0 = 104;                            // 1MHz 9600
	UCA0BR1 = 0;                              // 1MHz 9600
	UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
	IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt

	//---------------------- Servo PWM Setup -----------------------------
	//	TA1CCR1 Controls duty cycle for motors 1 + 2
	//	TA1CCR2 Controls duty cycle for motors 3 + 4

	P2DIR |= 0xFF;							// P2 all output
	//TODO: UPDATE P2 direction to only pins being used
	P2OUT = 0; 								// Clear all outputs P2
	P2SEL = BIT1 | BIT4;                    // P2 select TA1 option
	TA1CCR0 = PWM_PERIOD;                   // PWM Period on TA1 of 20ms
	TA1CCR1 = 1500;                         // CCR1 PWM duty cycle
	TA1CCTL1 = OUTMOD_7 + CCIE;             // CCR1 reset/set
	TA1CTL   = TASSEL_2 + MC_1;             // SMCLK, up mode
	TA1CCTL0 |= CCIE;						// Compare/Control Interrupt Enable

}//initialize()

