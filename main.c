#include  <msp430.h>

int main(void)
{
	volatile int i;

	WDTCTL = WDTPW + WDTHOLD;

	P1DIR |= BIT0; 							// Internal LEDs P1.0 of Launchpad is output

	P1DIR |= BIT6;							// P1.6/TA0.1 is used for PWM, thus also an output -> servo 1
	P2DIR |= BIT2;							// P2.2/TA1.1 is used for PWM, thus also an output -> servo 2

	P1OUT = 0; 								// Clear all outputs P1
	P2OUT = 0; 								// Clear all outputs P2

	P1SEL |= BIT6;                          // P1.6 select TA0.1 option
	P2SEL |= BIT2;                          // P2.2 select TA1.1 option

	// if SMCLK is about 1MHz (or 1000000Hz),
	// and 1000ms are the equivalent of 1 Hz,
	// then, by setting CCR0 to 20000 (1000000 / 1000 * 20)
	// we get a period of 20ms
	TA0CCR0 = 22222;                           // PWM Period TA0.1
	TA1CCR0 = 22222;                           // PWM Period TA1.1

	// setting 1500 is 1.5ms is 0deg. servo pos
	TA0CCR1 = 1500;                            // CCR1 PWM duty cycle
	TA1CCR1 = 1500;                            // CCR1 PWM duty cycle

	TA0CCTL1 = OUTMOD_7;                       // CCR1 reset/set
	TA0CTL   = TASSEL_2 + MC_1;                // SMCLK, up mode
	TA1CCTL1 = OUTMOD_7;                       // CCR1 reset/set
	TA1CTL   = TASSEL_2 + MC_1;                // SMCLK, up mode

	while(1){
		for (i = 900; i < 2000; i++)
		{

			TA0CCR1 = i;
			TA1CCR1 = i;
			__delay_cycles(2000);

		}
		for (i = 2000; i > 900; i--)
		{

			TA0CCR1 = i;
			TA1CCR1 = i;
			__delay_cycles(2000);

		}
	}
}
