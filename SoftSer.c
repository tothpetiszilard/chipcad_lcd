
#include <xc.h>
#include "TMR1.h"
#include "protocol.h"

#define SOFTSER_USE_PARITY (0)

static volatile uint8_t rxByte = 0;
static volatile uint8_t bitCnt = 0;
static volatile uint8_t rxReady = 0;

void SoftSer_Init(void)
{
    OPTION_REGbits.INTEDG = 0; // falling edge
    TRISBbits.TRISB0 = 1; // RX is input
    INTCONbits.INTF = 0;
    INTCONbits.INTE = 1; // Enable Start signal detection
}

void SoftSer_Cyclic(void)
{
    if (0 != rxReady)
    {
        Proto_RxIndication(rxByte);
        rxReady = 0;
    }
}

void SoftSer_StartDetect(void)
{
    INTCONbits.INTF = 0;
    INTCONbits.INTE = 0; // Disable Start signal detection
    TMR1_Enable(); // Start timer to catch first bit of packet
    rxByte = 0;
    bitCnt = 0;
}

void SoftSer_CaptureBit(void)
{
    if (bitCnt < 9u)
    {
        if (0 != bitCnt)
        {
            if (0 != PORTBbits.RB0)
            {
                rxByte = 0x80u | (rxByte >>1u);
            }
            else
            {
                rxByte = (rxByte >>1u);
            }
            
        }
        else if (0 != PORTBbits.RB0)
        {
            // must be low
            // ERROR
        }
    }
    /*else if (bitCnt == 8)
    {
        // last bit
        rxByte |= PORTBbits.RB0;
    }*/
#if (SOFTSER_USE_PARITY != 0)
    else if (bitCnt == 9)
    {
        // parity? bit
    }
#endif
    else
    {
        // must be high
        if (1 != PORTBbits.RB0)
        {
            // ERROR
        }
        else
        {
            // Notify higher layer
            rxReady = 1;
        }
        //TMR1_Disable(); // Wait for next frame
        PIE1bits.CCP1IE = 0;
        PIR1bits.CCP1IF = 0;
        T1CONbits.TMR1ON = 0; // Enable
        INTCONbits.INTF = 0; // Clear pending flag
        INTCONbits.INTE = 1; // Enable Start signal detection
    }
    bitCnt++;
}