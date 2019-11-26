| **uc STM32F051R8T6** | **Peripherals** |
| --- | --- |
| PIN NAME | PIN NAME |
|  | **_Keypad:_** |
| PA0 | _A_(1,2,3,6) |
| PA1 | _B_(9,4,5,8) |
| PA2 | _C_(0,<,7,>) |
| PA3 | _D_(P,M,ESC) |
| PA4 | _E_(1,9,0,P) |
| PA5 | _F_(3,5,7,ESC) |
| PA6 | _G_(2,4,<,M) |
| PA7 | _H_(6,8,>) |
|  | **_nRF24L01+:** |
| TR1 (PNP-EMITER) | _GND
| VCC (3,3V) | VCC |
| PC6 | CE |
| PB12(SPI2_NSS) | CSN |
| PB13(SPI2_SCK) | SCK |
| PB15(SPI2_MOSI) | MOSI|
| PB14(SPI2_MISO) | MISO |
| PC7 |     IRQ |
|  | **_AT24C64D_** |
| GND | A0 |
| GND | A1|
| GND |A2|
| GND |GND |
| PF7(I2C2\_SDA)-PullUp 4,7kOhm |SDA |
| PF6(I2C2\_SCL)-PullUp 4,7kOhm |SCL|
| PA12-(VCC-disable,GND-enable) |WP |
| VCC (3,3V) |VCC | 
|  | **_LEDS_** |
| PA11PA10 | 12 | _GREEN__RED_ |
|  | _UART\_DEBUG\_MODE_ |
| 5859GND | PB6(USART1\_TX)PB7(USART1\_RX)GND | 123 | _1 (GOLDPIN\_1) __2__ 3_ |
|  | _ST\_LINK_ |
| 4649755GND | PA13(SWDIO)PA14(SWCLK)NRST(ACTIVE\_LOW,pullUp-3V)PB3 (SWO)GND | 12345 | _1 (GOLDPIN\_2) __2__ 3 __4__ 5_ |
|  | _POWER\_SUPPLY_ |
| 13,1,32,6412,31,63 | VDDA,VBAT,VDD,VDDVSSA,VSS,VSS | VCC(3,3)GND | _1__2_ |
|  | _TRANSISTORS_ |
| 39 | PC8 | 1-G | _TR1_ |
| 40 | PC9 – FOR RECEIVER ONLY!!! | 1-G | _TR2_ |
|  |   |
|   |   |   |   |
