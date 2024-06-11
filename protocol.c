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
static volatile uint8_t rxBuffer[40];
static volatile uint8_t rxLen = 0;
static volatile uint8_t rxIdx = 0;

uint8_t Proto_Available(void)
{
    uint8_t retVal = 0;
    if (PROCESS == state)
    {
        retVal = rxLen;
    }
    return retVal;
}
uint8_t * Proto_Read(void)
{
    uint8_t * retVal = NULL;
    if (PROCESS == state)
    {
        retVal = (uint8_t *)rxBuffer;
    }
    return retVal;
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
        if (40 > data)
        {
            rxLen = data;
            rxIdx = 0;
        }
        else
        {
            state = IDLE;
        }
    }
    else if (DATA == state)
    {
        rxBuffer[rxIdx] = data;
        rxIdx++;
        if (rxIdx >= rxLen)
        {
            state = CRC;
            crc8_init(0xFFu);
        }
    }
    else if (CRC == state)
    {
        if (data == crc8_stream((uint8_t *)rxBuffer,rxLen))
        {
            // CRC OK
            state = PROCESS;
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
