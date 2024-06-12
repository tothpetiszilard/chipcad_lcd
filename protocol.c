#include "crc8.h"
#include "protocol.h"


#define STARTCHAR   ('#')

typedef enum
{
    IDLE,
    LEN,
    DATA,
    CRC,
    PROCESS
            
}LcdApp_State;

static volatile LcdApp_State state = IDLE;
static volatile uint8_t rxBuffer1[30];
static volatile uint8_t rxBuffer2[30];
static volatile uint8_t rxLen = 0;
static volatile uint8_t rxIdx = 0;
static volatile uint8_t *buffSel = rxBuffer1;
static volatile uint8_t rxReady = 0;

uint8_t Proto_Available(void)
{
    return rxReady;
}
uint8_t * Proto_Read(void)
{
    uint8_t * retVal = NULL;
    if (0 != rxReady)
    {
        if (rxBuffer1 == buffSel)
        {
            retVal = (uint8_t *)rxBuffer2;
        }
        else
        {
            retVal = (uint8_t *)rxBuffer1;
        }
    }
    return retVal;
}

void Proto_Clear(void)
{
    rxReady = 0;
}

void Proto_RxIndication(uint8_t data)
{
    if ((IDLE == state) && (data == STARTCHAR))
    {
        rxLen = 0;
        state = LEN;
    }
    else if (LEN == state)
    {
        if (sizeof(rxBuffer1) > data)
        {
            rxLen = data;
            rxIdx = 0;
            state = DATA;
        }
        else
        {
            state = IDLE;
        }
    }
    else if (DATA == state)
    {
        buffSel[rxIdx] = data;
        rxIdx++;
        if (rxIdx >= rxLen)
        {
            state = CRC;
            crc8_init(0xFFu);
        }
    }
    else if (CRC == state)
    {
        if (data == crc8_stream((uint8_t *)buffSel,rxLen))
        {
            // CRC OK
            if (0 == rxReady) // else data loss
            {
                if (rxBuffer1 == buffSel)
                {
                    buffSel = rxBuffer2;
                }
                else
                {
                    buffSel = rxBuffer1;
                }
                rxReady = rxLen;
            }
            state = IDLE;
        }
        else
        {
            state = IDLE;
        }
    }
    else
    {
        state = IDLE;
    }
    
}
