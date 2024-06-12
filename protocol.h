/* 
 * File:   protocol.h
 * Author: tothpetiszilard
 *
 * Created on June 11, 2024, 5:10 PM
 */

#ifndef PROTOCOL_H
#define	PROTOCOL_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <xc.h>

extern void Proto_RxIndication(uint8_t data);
extern uint8_t Proto_Available(void);
extern uint8_t * Proto_Read(void);
extern void Proto_Clear(void);

#ifdef	__cplusplus
}
#endif

#endif	/* PROTOCOL_H */

