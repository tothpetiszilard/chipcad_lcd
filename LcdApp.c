#include "LcdApp.h"
#include "hd44780.h"
#include "protocol.h"

typedef enum
{
    IDLE,
    BACKLIGHT,
    CONTRAST,
    SETPOS,
    ERROR
}LcdApp_State;

void LCD_Cyclic(void)
{
    uint8_t rxLen = 0;
    uint8_t *dataPtr = NULL;
    rxLen = Proto_Available();
    if (0 < rxLen)
    {
        dataPtr = Proto_Read();
        if (NULL != dataPtr)
        {
            for (uint8_t i = 0; i < rxLen; i++)
            {
                LCD_RxIndication(dataPtr[i]);
            }
        }
        Proto_Clear();
    }
}

void LCD_RxIndication(uint8_t data)
{
    static LcdApp_State state = IDLE;
    if (IDLE == state)
    {
        switch (data)
        {
            case 0x0C:
                hd44780_clear();
                break;
            case 0x0D:
                hd44780_set_address(0x0u);
                break;
            case 0x1B:
                state = SETPOS;
                break;
            case 0x09:
                state = BACKLIGHT;
                break;
            case 0x0B:
                state = CONTRAST;
                break;
            default:
                hd44780_put_char(data);
                break;
        }
    }
    else if (SETPOS == state)
    {
        if ((data >= 20) && (data < 40))
        {
            hd44780_set_address(data + 20);
        }
        else if (data < 20)
        {
            hd44780_set_address(data);
        }
        else  if ((data >= 40) && (data < 60))
        {
            hd44780_set_address(data - 20);
        }
        else
        {
            // data >= 60
            hd44780_set_address(data + 24);
        }
        
        state = IDLE;
    }
    else if (BACKLIGHT == state)
    {
        if (0 != data)
        {
            PORTCbits.RC6 = 1; // Backlight ON
        }
        else
        {
            PORTCbits.RC6 = 0; // Backlight OFF
        }
        state = IDLE;
    }
    else if (CONTRAST == state)
    {
        // Not implemented yet
        (void)(data);
        state = IDLE;
    }
    else
    {
        state = IDLE;
    }
}
