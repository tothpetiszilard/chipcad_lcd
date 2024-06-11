
#include <xc.h>
#include "SoftSer.h"

static volatile uint8_t timeCnt = 0; 

void TMR1_Init(void)
{
    T1CONbits.TMR1ON = 0; // Disable
    T1CONbits.TMR1GE = 0; // No gate
    T1CONbits.TMR1CS = 0; // Fosc/4
    T1CONbits.T1CKPS = 0; // 500 khz
    T1CONbits.T1OSCEN = 0;
    CCP1CONbits.CCP1M = 0b1011; // Compare mode with interrupt + reset TMR1
    CCPR1 = 268; // 3 times 2400 baud per sec = 69.4444
    PIR1bits.CCP1IF = 0;
    PIE1bits.TMR1IE = 0;
    TMR1 = 0;
}

void TMR1_Enable(void)
{
    TMR1 = 0;
    PIR1bits.CCP1IF = 0;
    T1CONbits.TMR1ON = 1; // Enable
    PIE1bits.CCP1IE = 1;
    timeCnt = 0;
}

void TMR1_Disable(void)
{
    PIE1bits.CCP1IE = 0;
    PIR1bits.CCP1IF = 0;
    T1CONbits.TMR1ON = 0; // Enable
}

void TMR1_Interrupt(void)
{
    PIR1bits.CCP1IF = 0;
    if (timeCnt > 0)
    {
        if (timeCnt < 2)
        {
            // Timecount 1 
            SoftSer_CaptureBit();
            timeCnt++;
        }
        else
        {
            // timecount 2 -> 0
            timeCnt = 0;
        }
    }
    else
    {
        // timecount 0
        timeCnt++;
    }
}