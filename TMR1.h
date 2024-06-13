/* 
 * File:   TMR1.h
 * Author: tothpetiszilard
 *
 * Created on June 11, 2024, 12:08 PM
 */

#ifndef TMR1_H
#define	TMR1_H

#ifdef	__cplusplus
extern "C" {
#endif

    extern void TMR1_Init(void);
    inline void TMR1_Enable(void);
    inline void TMR1_Disable(void);
    extern void TMR1_Interrupt(void);


#ifdef	__cplusplus
}
#endif

#endif	/* TMR1_H */

