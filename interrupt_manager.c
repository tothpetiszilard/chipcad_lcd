/**
  Generated Interrupt Manager Source File

  @Company:
    Microchip Technology Inc.

  @File Name:
    interrupt_manager.c

  @Summary:
    This is the Interrupt Manager file generated using MPLAB� Code Configurator

  @Description:
    This header file provides implementations for global interrupt handling.
    For individual peripheral handlers please see the peripheral driver for
    all modules selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB� Code Configurator - v2.25.2
        Device            :  PIC16F1719
        Driver Version    :  1.02
    The generated drivers are tested against the following:
        Compiler          :  XC8 v1.34
        MPLAB             :  MPLAB X v2.35 or v3.00
 */

/*
Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 */

#include <xc.h> // include processor files - each processor file is guarded.  
#include "interrupt_manager.h"
#include "eusart.h"
#include "TMR0.h"
#include "TMR1.h"
#include "SoftSer.h"

void __interrupt() INTERRUPT_InterruptManager(void) {
    // interrupt handler
    if ((PIE1bits.CCP1IE == 1) && (PIR1bits.CCP1IF != 0))
    {
        TMR1_Interrupt();
    }
    else if ((INTCONbits.INTE == 1) && (0 != INTCONbits.INTF))
    {
        SoftSer_StartDetect();
    }
    /*else if (PIE1bits.RCIE == 1 && PIR1bits.RCIF == 1) 
    {
        EUSART_Receive_ISR();
    } 
    else if (PIE1bits.TXIE == 1 && PIR1bits.TXIF == 1)
    {
        EUSART_Transmit_ISR();
    }*/
    /*else if (INTCONbits.T0IF != 0)
    {
        TMR0_Interrupt();
    }*/
    else if (0 != PIR2bits.OSFIF)
    {
        // Clock problems
        PIR2bits.OSFIF = 0;
    }
    else 
    {
        //Unhandled Interrupt
    }
}
/**
 End of File
 */