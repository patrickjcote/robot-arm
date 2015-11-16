#include  <msp430.h>

void initialize();
void testInput(char inChar);
void messageOut(char messageIn[], int length);

volatile int servoPosition;

int main(void)
{
	volatile int i;

	initialize();

	__bis_SR_register(GIE);

	while(1){

	}//

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

	//---------------------- Servo PWM Setup ----------------------------

	P2DIR |= 0x0F;							// P2.2/TA1.1 is used for PWM, thus also an output -> servo 2
	P2OUT = 0; 								// Clear all outputs P2
	P2SEL |= BIT1 + BIT2;                          // P2.2 select TA1.1 option
	TA1CCR0 = 22222;                        // PWM Period TA1.1 of 20ms
	TA1CCR1 = 1500;                         // CCR1 PWM duty cycle
	TA1CCTL1 = OUTMOD_7;                    // CCR1 reset/set
	TA1CTL   = TASSEL_2 + MC_1;             // SMCLK, up mode

}//setRegisters()

#pragma vector=USCIAB0RX_VECTOR
//  Echo back RXed character, confirm TX buffer is ready first
__interrupt void USCI0RX_ISR(void)
{
	volatile char inChar;
	inChar = UCA0RXBUF;
	testInput(inChar);
}

void testInput(char inChar){

	switch(inChar){

	case 's':
		servoPosition += 50;
		break;
	case 'w':
		servoPosition -= 50;
		break;
	default:
		messageOut("Invalid Key",11);
		break;
	}//switch

	if(servoPosition < 900){
		servoPosition = 900;
		messageOut("Hit High Edge",13);
	}
	if(servoPosition > 2000){
		servoPosition=2000;
		messageOut("Hit Low Edge",12);
	}
	TA1CCR1 = servoPosition;
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

}//outputMessage
