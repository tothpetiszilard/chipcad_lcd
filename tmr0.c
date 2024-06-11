
#include <xc.h>



void TMR0_Init(void)
{
    OPTION_REGbits.T0CS = 0; // Timer mode
    OPTION_REGbits.PSA = 0; // Prescaler to TMR0
    OPTION_REGbits.PS = 2; // Prescaler 1:8
}

void TMR0_Enable(void)
{
    INTCONbits.T0IF = 0;
    TMR0 = 0;
    INTCONbits.T0IE = 1;
}

void TMR0_Disable(void)
{
    INTCONbits.T0IE = 0;
}

void TMR0_Interrupt(void)
{
    INTCONbits.T0IF = 0;
}
