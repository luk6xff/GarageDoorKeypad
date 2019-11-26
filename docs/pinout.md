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
| PA11 |GREEN|
| PA10 |RED|
|  | **_DEBUG_UART_** |
| PB6(USART1_TX) |GOLDPIN_1| 
| PB7(USART1_RX) |GOLDPIN_2|
| GND | GOLDPIN_3 |
|  | **_ST_LINK_** |
| PA13(SWDIO) |GOLDPIN_1|
| PA14(SWCLK)  |GOLDPIN_2| 
| NRST(ACTIVE_LOW,PullUp-3V) |GOLDPIN_3| 
| PB3 (SWO) |NC|
| GND  |GOLDPIN_5|
|  | **_POWER_SUPPLY_** |
| 13,1,32,64 (VDDA,VBAT,VDD,VDD) | VCC(3,3)|
| 12,31,63 (VSSA,VSS,VSS) |GND |
|  | **_TRANSISTORS_** |
|PC8 | TR1 |
|PC9 – FOR RECEIVER ONLY! | _TR2_ |
|  |   |
|   |   |   |   |
