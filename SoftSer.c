
#include <xc.h>
#include "TMR1.h"
#include "LcdApp.h"

#define SOFTSER_USE_PARITY (0)

static volatile uint8_t rxByte = 0;
static volatile uint8_t bitCnt = 0;

void SoftSer_Init(void)
{
    OPTION_REGbits.INTEDG = 0; // falling edge
    TRISBbits.TRISB0 = 1; // RX is input
    INTCONbits.INTF = 0;
    INTCONbits.INTE = 1; // Enable Start signal detection
    TRISCbits.TRISC5 = 0;
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
    PORTCbits.RC5 = 1;
    if (bitCnt < 9)
    {
        if (0 != bitCnt)
        {
            rxByte |= (PORTBbits.RB0 << (bitCnt - 1));
            //rxByte = rxByte << 1;
        }
        else if (0 != PORTBbits.RB0)
        {
            // must by low
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
        // must by high
        if (1 != PORTBbits.RB0)
        {
            // ERROR
        }
        else
        {
            // Notify higher layer
            LCD_RxIndication(rxByte);
        }
        TMR1_Disable(); // Wait for next frame
        INTCONbits.INTF = 0; // Clear pending flag
        INTCONbits.INTE = 1; // Enable Start signal detection
    }
    bitCnt++;
    PORTCbits.RC5 = 0;
}