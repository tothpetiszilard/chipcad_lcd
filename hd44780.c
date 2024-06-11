#include "hd44780.h"
#include "chars.h"

#ifdef HD44780_DUAL
	#define HD44780_SET_E(x) {if (hd44780_device == 0) HD44780_E1 = x; else HD44780_E2 = x;}
#else
	#define HD44780_SET_E(x) {HD44780_E = x;}
#endif

static void hd44780_build(unsigned char location, unsigned char *ptr);

void hd44780_wait()
{
	unsigned char temp;
	HD44780_DATA_TRIS = HD44780_DATA_CAST(0xff);
	HD44780_RS = 0;
	HD44780_RW = 1;
	do
	{
		HD44780_SET_E(1);
		temp = HD44780_DATA;
		HD44780_SET_E(0);
		HD44780_SET_E(1);
		HD44780_SET_E(0);
	} while (HD44780_DATA_R7(temp));
	HD44780_DATA_TRIS = HD44780_DATA_CAST(0x00);
	HD44780_RW = 0;
}

void hd44780_put_char(unsigned char data)
{
	hd44780_wait();
	HD44780_RS = 1;
#ifdef HD44780_4BIT
	HD44780_SET_E(1);
	HD44780_DATA = HD44780_DATA_CAST(data >> 4);
	HD44780_SET_E(0);
	HD44780_SET_E(1);
	HD44780_DATA = HD44780_DATA_CAST(data & 0x0f);
	HD44780_SET_E(0);
#else
	HD44780_SET_E(1);
	HD44780_DATA = HD44780_DATA_CAST(data);
	HD44780_SET_E(0);
#endif
}

void hd44780_put_cmd(unsigned char cmd)
{
	hd44780_wait();
	HD44780_RS = 0;
#ifdef HD44780_4BIT
	HD44780_SET_E(1);
	HD44780_DATA = HD44780_DATA_CAST(cmd >> 4);
	HD44780_SET_E(0);
	HD44780_SET_E(1);
	HD44780_DATA = HD44780_DATA_CAST(cmd & 0x0f);
	HD44780_SET_E(0);
#else
	HD44780_SET_E(1);
	HD44780_DATA = HD44780_DATA_CAST(cmd);
	HD44780_SET_E(0);
#endif
}

inline void hd44780_set_address(unsigned char addr)
{
	hd44780_put_cmd(0x80 | addr);
}

inline void hd44780_clear(void)
{
	hd44780_put_cmd(0x01);
	HD44780_DELAY_1640US();
}

void hd44780_init_ports(void)
{
    ANSEL = 0x00;
    ANSELH = 0x00;
	HD44780_DATA = HD44780_DATA_CAST(0x00);
	HD44780_DATA_TRIS = HD44780_DATA_CAST(0x00);

	HD44780_RS = 0;
	HD44780_RS_TRIS = 0;
	HD44780_RW = 0;
	HD44780_RW_TRIS = 0;

#ifdef HD44780_DUAL
	HD44780_E1 = 0;
	HD44780_E1_TRIS = 0;
	HD44780_E2 = 0;
	HD44780_E2_TRIS = 0;
#else
	HD44780_E = 0;
	HD44780_E_TRIS = 0;
#endif
}

void hd44780_init(void)
{
#ifdef HD44780_DUAL
	unsigned char device_counter;
#endif

	hd44780_init_ports();

	HD44780_DELAY_1640US();

#ifdef HD44780_DUAL
	for (device_counter = 0; device_counter <= 1; device_counter++)
	{
		hd44780_device = device_counter;
#endif
		hd44780_put_cmd(0x28 /* 2 lines, 5x7 font */
#ifndef HD44780_4BIT
				| 0x10
#endif
				);
		hd44780_put_cmd(0x08); /* display off, cursor off, cursor blink off */
		hd44780_clear();
		hd44780_put_cmd(0x0c); /* display on, cursor off, cursor blink off */
		hd44780_put_cmd(0x06); /* increment cursor; don't shift display */
#ifdef HD44780_DUAL
	}
#endif
    hd44780_build(0,&hungarian[0][0]);
    hd44780_build(1,&hungarian[1][0]);
    hd44780_build(2,&hungarian[2][0]);
    hd44780_build(3,&hungarian[3][0]);
    hd44780_build(4,&hungarian[4][0]);
    hd44780_build(5,&hungarian[5][0]);
    hd44780_build(6,&hungarian[6][0]);
}

//Input:
//     location: location where you want to store
//               0,1,2,....7
//     ptr: Pointer to pattern data
//
//Usage:
//     pattern[8]={0x04,0x0E,0x0E,0x0E,0x1F,0x00,0x04,0x00};
//     LCD_build(1,pattern);
//
//LCD Ports are same as discussed in previous sections
 
static void hd44780_build(unsigned char location, unsigned char *ptr)
{
    unsigned char i;
    if(location<8)
    {
        hd44780_put_cmd(0x40+(location*8));
        for(i=0;i<8;i++)
        {
            hd44780_put_char(ptr[ i ]);
        }
   }
}
