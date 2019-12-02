#include <mbed.h>
#include "AT24C64/platform/mbed/at24c64-mbed.h"



#define AT24C64_ADDRESS 0xA0   /* |1|0|1|0|A2|A1|A0|R/W| */
#define AT24C64_PAGE_SIZE 32 
#define AT24C64_NR_OF_PAGES 256
#define AT24C64_CHIP_SIZE (AT24C64_PAGE_SIZE*AT24C64_NR_OF_PAGES)


int main()
{
    Serial dbg(PB6, PB7); // tx, rx
    dbg.baud(9600);

    dbg.printf("HELLO from GarageDoorKeypad app!");
    DigitalOut ledGreen(PA11);

    while(1)
    {
      ledGreen = 1;
      wait_us(3000*1000);
      ledGreen = 0;
      //wait_us(1000*1000);
      //dbg.printf("HELLO");
    }
}