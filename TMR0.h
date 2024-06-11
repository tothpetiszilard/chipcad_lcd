/* 
 * File:   TMR0.h
 * Author: tothpetiszilard
 *
 * Created on June 11, 2024, 11:37 AM
 */

#ifndef TMR0_H
#define	TMR0_H

#ifdef	__cplusplus
extern "C" {
#endif

    extern void TMR0_Init(void);
    extern void TMR0_Interrupt(void);
    extern void TMR0_Disable(void);
    extern void TMR0_Enable(void);

#ifdef	__cplusplus
}
#endif

#endif	/* TMR0_H */

