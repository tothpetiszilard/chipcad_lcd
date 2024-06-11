
#include "hd44780.h"
#include "eusart.h"
#include "interrupt_manager.h"
#include "crc8.h"
#include "SoftSer.h"
#include "TMR1.h"
#include "LcdApp.h"

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
/*
                         Main application
 */
static volatile uint8_t portCheck = 0xFF;

void main(void)
{    
    OSCCONbits.IRCF = 7u; // 8Mhz INTOSC
    while(OSCCONbits.HTS != 1)
    {
        //Wait for the oscillator
    }
    OSCCONbits.OSTS = 0u;
    OSCCONbits.SCS = 0u;

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:
    TMR1_Init();
    SoftSer_Init();
    TRISCbits.TRISC6 = 0; // Backlight output
    __delay_ms(200);
    //TRISCbits.TRISC2 = 0; // Contrast output
    
    hd44780_init();
    //EUSART_Initialize();
    // Disable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();
    __delay_ms(100);
    //
    hd44780_set_address(0x0u); // Cursor to 0
    __delay_ms(4);
    
    while (1)
    {
        LCD_Cyclic();
    }

}
/**
 End of File
*/