/**
 *  @brief:   NRF24L01+ radio module library registers
 *  @author:  luk6xff
 *  @email:   lukasz.uszko@gmail.com
 *  @date:    2019-12-01
 *  @license: MIT
 */

#ifndef __NRF24L01_REGISTERS_H_
#define __NRF24L01_REGISTERS_H_

#include <stdint.h>

typedef uint8_t BYTE;
#define DATA_PIPE_LENGTH 5

/* COMMANDS */
#define R_REGISTER 0x00  		 // nrf24l01_read command and status register (0b000AAAAAA) where AAAAA = 5 bit Register Map Address
#define W_REGISTER 0x20          // nrf24l01_write command and status registers.LSB AAAAA = 5 bit Register Map Address
#define R_RX_PAYLOAD 0x61        // nrf24l01_read RX-payload: 1-32 bytes. A nrf24l01_read operation always starts at byte 0. Payload is deleted from FIFO after it is nrf24l01_read. Used in RX mode
#define W_TX_PAYLOAD 0xA0 	     // nrf24l01_write TX-payload: 1-32 bytes. A nrf24l01_write operation always starts at byte 0 used in TX payload.
#define FLUSH_TX 0xE1     		 // Flush TX FIFO, used in TX mode
#define FLUSH_RX 0xE2    		 // Flush RX FIFO, used in RX mode
#define REUSE_TX_PL 0xE3 		 // Used for a PTX deviceReuse last transmitted payload.TX payload reuse is active until W_TX_PAYLOAD or FLUSH TX is executed.
#define R_RX_PL_WID 0x60 		 // nrf24l01_read RX payload length for the top R_RX_PAYLOAD in the RX FIFO. Note: Flush RX FIFO if the nrf24l01_read value is larger than 32 bytes.
#define W_ACK_PAYLOAD 0xA8 		 // (0b1010 1PPP)Used in RX mode. nrf24l01_write Payload to be transmitted together with ACK packet on PIPE PPP. (PPP valid in the range from 000 to 101).
#define W_TX_PAYLOAD_NO_ACK 0xB0 // Used in TX mode. Disables AUTOACK on this specific packet.
#define NOP 0xFF                 // No Operation. Might be used to nrf24l01_read the STATUS register


typedef enum
{
	READ = R_REGISTER, //nrf24l01_read command and status register (0b000AAAAAA) where AAAAA = 5 bit Register Map Address
	WRITE = W_REGISTER, //nrf24l01_write command and status registers.LSB AAAAA = 5 bit Register Map Address
	READ_PAYLOAD = R_RX_PAYLOAD, //nrf24l01_read RX-payload: 1 � 32 bytes. A nrf24l01_read operation always starts at byte 0. Payload is deleted from FIFO after it is nrf24l01_read. Used in RX mode
	WRITE_PAYLOAD = W_TX_PAYLOAD, //nrf24l01_write TX-payload: 1 � 32 bytes. A nrf24l01_write operation always starts at byte 0 used in TX payload.
	FLUSH__TX = FLUSH_TX, //Flush TX FIFO, used in TX mode
	FLUSH__RX = FLUSH_RX, //Flush RX FIFO, used in RX mode
	REUSE_LAST_TX_PAYLOAD = REUSE_TX_PL, //Used for a PTX deviceReuse last transmitted payload.TX payload reuse is active until W_TX_PAYLOAD or FLUSH TX is executed.
	READ_RX_PAYLOAD_WIDTH = R_RX_PL_WID, //nrf24l01_read RX payload length for the top R_RX_PAYLOAD in the RX FIFO. Note: Flush RX FIFO if the nrf24l01_read value is larger than 32 bytes.
	WRITE_PAYLOAD_WITH_ACK = W_ACK_PAYLOAD, //(0b1010 1PPP)Used in RX mode. nrf24l01_write Payload to be transmitted together with ACK packet on PIPE PPP. (PPP valid in the range from 000 to 101).
	DISABLE_AUTO_ACK = W_TX_PAYLOAD_NO_ACK, //Used in TX mode. Disables AUTOACK on this specific packet.
	DO_NOTHING = NOP
} _SPI_COMMAND_Reg;

// Memory map of the NRF24L01+ module
typedef enum
{
	CONFIG_regAdr = 0x00,
	EN_AA_ADR_regAdr,
	EN_RXADDR_regAdr,
	SETUP_AW_regAdr,
	SETUP_RETR_regAdr,
	RF_CH_regAdr,
	RF_SETUP_regAdr,
	STATUS_regAdr,
	OBSERVE_TX_regAdr,
	RPD_regAdr,
	RX_ADDR_P0_regAdr,
	RX_ADDR_P1_regAdr,
	RX_ADDR_P2_regAdr,
	RX_ADDR_P3_regAdr,
	RX_ADDR_P4_regAdr,
	RX_ADDR_P5_regAdr,
	TX_ADDR_regAdr,
	RX_PW_P0_regAdr,
	RX_PW_P1_regAdr,
	RX_PW_P2_regAdr,
	RX_PW_P3_regAdr,
	RX_PW_P4_regAdr,
	RX_PW_P5_regAdr,
	FIFO_STATUS_regAdr,
	DYNPD_regAdr = 0x1C,
	FEATURE_regAdr
} _SPI_REGISTER_ADRESS_Reg;

//------------------------------------------------------------------------------
//  CONTENT OF EACH NRF24L01+ REGISTER
//------------------------------------------------------------------------------
typedef struct
{
	BYTE PRIM_RX :1;
	BYTE PWR_UP :1;
	BYTE CRCO :1;
	BYTE EN_CRC :1;
	BYTE MASK_MAX_RT :1;
	BYTE MASK_TX_DS :1;
	BYTE MASK_RX_DR :1;
	const BYTE RESERVED :1;
} CONFIG_Reg; //0x00

//------------------------------------------------------------------------------
typedef struct
{
	BYTE ENAA_P0 :1;
	BYTE ENAA_P1 :1;
	BYTE ENAA_P2 :1;
	BYTE ENAA_P3 :1;
	BYTE ENAA_P4 :1;
	BYTE ENAA_P5 :1;
	const BYTE RESERVED :2;
} EN_AA_Reg; //0x01

//------------------------------------------------------------------------------
typedef struct
{
	BYTE ERX_P0 :1;
	BYTE ERX__P1 :1;
	BYTE ERX__P2 :1;
	BYTE ERX__P3 :1;
	BYTE ERX__P4 :1;
	BYTE ERX_ :1;
	const BYTE RESERVED :2;
} EN_RXADDR_Reg; //0x02

//------------------------------------------------------------------------------
typedef enum
{
	_3BYTES = 0x01, _4BYTES, _5BYTES
} _AW_Reg;

typedef struct
{
	_AW_Reg AW :2;
	const BYTE RESERVED :6;
} SETUP_AW_Reg; //0x03

//------------------------------------------------------------------------------
typedef enum
{
	_DISABLED,
	_1_RETRANSMIT,
	_2_RETRANSMIT,
	_3_RETRANSMIT,
	_4_RETRANSMIT,
	_5_RETRANSMIT,
	_6_RETRANSMIT,
	_7_RETRANSMIT,
	_8_RETRANSMIT,
	_9_RETRANSMIT,
	_10_RETRANSMIT,
	_11_RETRANSMIT,
	_12_RETRANSMIT,
	_13_RETRANSMIT,
	_14_RETRANSMIT,
	_15_RETRANSMIT
} _ARC_Reg;

typedef enum
{
	_250us,
	_500us,
	_750us,
	_1000us,
	_1250us,
	_1500us,
	_1750us,
	_2000us,
	_2250us,
	_2500us,
	_2750us,
	_3000us,
	_3250us,
	_3500us,
	_3750us,
	_4000us
} _ARD_Reg;

typedef struct
{
	_ARC_Reg ARC :4;
	_ARD_Reg ARD :4;
} SETUP_RETR_Reg; //0x04

//------------------------------------------------------------------------------
typedef struct
{
	BYTE RF_CH :7;
	const BYTE RESERVED :1;
} RF_CH_Reg; //0x05

//------------------------------------------------------------------------------
/*[RF_DR_LOW, RF_DR_HIGH]:
 �00� � 1Mbps
 �01� � 2Mbps
 �10� � 250kbps
 �11� � Reserved
 */
typedef enum
{
	_18dBm, _12dBm, _6dBm, _0dBm
} _RF_PWR_Reg;

typedef struct {
	const BYTE RESERVED0 :1;
	_RF_PWR_Reg RF_PWR :2;
	BYTE RF_DR_HIGH :1;
	BYTE PLL_LOCK :1;
	BYTE RF_DR_LOW :1;
	const BYTE RESERVED1 :1;
	BYTE CONT_WAVE :1;
} RF_SETUP_Reg; //0x06

//------------------------------------------------------------------------------
typedef struct {
	BYTE TX_FULL :1;
	BYTE RX_P_NO :3;
	BYTE MAX_RT :1;
	BYTE TX_DS :1;
	BYTE RX_DR :1;
	const BYTE RESERVED1 :1;
} STATUS_Reg; //0x07

//------------------------------------------------------------------------------
typedef struct {
	BYTE ARC_CNT :4; //page 75 datasheet
	BYTE PLOS_CNT :4;
} OBSERVE_TX_Reg; //0x08

//------------------------------------------------------------------------------
typedef struct {
	BYTE RPD :1;
	const BYTE RESERVED1 :7;
} RPD_Reg; //0x09

//------------------------------------------------------------------------------
typedef enum
{
	PIPE_NOT_USED,
	_1_Byte,
	_2_Bytes,
	_3_Bytes,
	_4_Bytes,
	_5_Bytes,
	_6_Bytes,
	_7_Bytes,
	_8_Bytes,
	_9_Bytes,
	_10_Bytes,
	_11_Bytes,
	_12_Bytes,
	_13_Bytes,
	_14_Bytes,
	_15_Bytes,
	_16_Bytes,
	_17_Bytes,
	_18_Bytes,
	_19_Bytes,
	_20_Bytes,
	_21_Bytes,
	_22_Bytes,
	_23_Bytes,
	_24_Bytes,
	_25_Bytes,
	_26_Bytes,
	_27_Bytes,
	_28_Bytes,
	_29_Bytes,
	_30_Bytes,
	_31_Bytes,
	_32_Bytes,
} _RX_PW_Reg;

typedef struct
{
	_RX_PW_Reg RX_PW_P0 :6;
	const BYTE RESERVED :2;
} RX_PW_P0_Reg; //0x11

typedef struct
{
	_RX_PW_Reg RX_PW_P1 :6;
	const BYTE RESERVED :2;
} RX_PW_P1_Reg; //0x12

typedef struct
{
	_RX_PW_Reg RX_PW_P2 :6;
	const BYTE RESERVED :2;
} RX_PW_P2_Reg; //0x13

typedef struct
{
	_RX_PW_Reg RX_PW_P3 :6;
	const BYTE RESERVED :2;
} RX_PW_P3_Reg; //0x14

typedef struct
{
	_RX_PW_Reg RX_PW_P4 :6;
	const BYTE RESERVED :2;
} RX_PW_P4_Reg; //0x15

typedef struct
{
	_RX_PW_Reg RX_PW_P5 :6;
	const BYTE RESERVED :2;
} RX_PW_P5_Reg; //0x16

//------------------------------------------------------------------------------
typedef struct
{
	BYTE RX_EMPTY :1;
	BYTE RX_FULL :1;
	const BYTE RESERVED0 :2;
	BYTE TX_EMPTY :1;
	BYTE TX_FULL :1;
	BYTE TX_REUSE :1;
	const BYTE RESERVED1 :1;
} FIFO_STATUS_Reg; //0x17

//------------------------------------------------------------------------------
typedef struct
{
	BYTE DPL_P0 :1;
	BYTE DPL_P1 :1;
	BYTE DPL_P2 :1;
	BYTE DPL_P3 :1;
	BYTE DPL_P4 :1;
	BYTE DPL_P5 :1;
	const BYTE RESERVED1 :2;
} DYNPD_Reg; //0x1C

//------------------------------------------------------------------------------

typedef struct
{
	BYTE EN_DYN_ACK :1;
	BYTE EN_ACK_PAY :1;
	BYTE EN_DPL :1;
	const BYTE RESERVED1 :5;
} FEATURE_Reg; //0x1C

//------------------------------------------------------------------------------
// Memory map of NRF24L01
typedef struct
{
	CONFIG_Reg CONFIG;
	EN_AA_Reg EN_AA;
	EN_RXADDR_Reg EN_RXADDR;
	SETUP_AW_Reg SETUP_AW;
	SETUP_RETR_Reg SETUP_RETR;
	RF_CH_Reg RF_CH;
	RF_SETUP_Reg RF_SETUP;
	STATUS_Reg STATUS;
	OBSERVE_TX_Reg OBSERVE_TX;
	RPD_Reg RPD;
	BYTE RX_ADDR_P0[DATA_PIPE_LENGTH]; //0x0A  // nrf24l01_write the number of bytes defined in SETUP_AW (AW) // LSB written first
	BYTE RX_ADDR_P1[DATA_PIPE_LENGTH]; //0x0B
	BYTE RX_ADDR_P2[1];
	BYTE RX_ADDR_P3[1];
	BYTE RX_ADDR_P4[1];
	BYTE RX_ADDR_P5[1];
	BYTE TX_ADDR[DATA_PIPE_LENGTH]; //0x10 // nrf24l01_write the number of bytes defined in SETUP_AW (AW) // LSB written first
	RX_PW_P0_Reg RX_PW_P0;
	RX_PW_P1_Reg RX_PW_P1;
	RX_PW_P2_Reg RX_PW_P2;
	RX_PW_P3_Reg RX_PW_P3;
	RX_PW_P4_Reg RX_PW_P4;
	RX_PW_P5_Reg RX_PW_P5;
	FIFO_STATUS_Reg FIFO_STATUS;
	DYNPD_Reg DYNPD;
	FEATURE_Reg FEATURE;
} nRF24L01;


/**
 * @brief REGISTERS AND BITFIELDS FOR NRF24L01+
 *		  BIT xxx_MASK can be used to be sure if all bits have been set up correctly (no reserved ones were changed)
 *	      To use xxx_MASK just & (AND) all previously set bits with this xxx_MASK
 */
//CONFIG REGISTER
#define CONFIG_PRIM_RX      0x01
#define CONFIG_PWR_UP       0x02
#define CONFIG_CRCO         0x04
#define CONFIG_EN_CRC		0x08
#define CONFIG_MASK_MAX_RT  0x10
#define CONFIG_MASK_TX_DS   0x20
#define CONFIG_MASK_RX_DR   0x40
#define CONFIG_MASK         0x7F

//EN_AA REGISTER
#define EN_AA_ENAA_P0       0x01
#define EN_AA_ENAA_P1       0x02
#define EN_AA_ENAA_P2       0x04
#define EN_AA_ENAA_P3       0x08
#define EN_AA_ENAA_P4       0x10
#define EN_AA_ENAA_P5       0x20
#define EN_AA_MASK          0x3F

//EN_RXADDR REGISTER
#define EN_RXADDR_ERX_P0    0x01
#define EN_RXADDR_ERX_P1    0x02
#define EN_RXADDR_ERX_P2    0x04
#define EN_RXADDR_ERX_P3    0x08
#define EN_RXADDR_ERX_P4    0x10
#define EN_RXADDR_ERX_P5    0x20
#define EN_RXADDR_MASK      0x3F

//SETUP_AW REGISTER
#define SETUP_AW_3BYTES    0x01
#define SETUP_AW_4BYTES    0x02
#define SETUP_AW_5BYTES    0x03
#define SETUP_AW_MASK      0x03

//SETUP_RETR REGISTER
#define SETUP_RETR_ARC_RETRANSMIT_DISABLED    0x00
#define SETUP_RETR_ARC_1_RETRANSMIT           0x01
#define SETUP_RETR_ARC_2_RETRANSMIT           0x02
#define SETUP_RETR_ARC_3_RETRANSMIT           0x03
#define SETUP_RETR_ARC_4_RETRANSMIT           0x04
#define SETUP_RETR_ARC_5_RETRANSMIT           0x05
#define SETUP_RETR_ARC_6_RETRANSMIT           0x06
#define SETUP_RETR_ARC_7_RETRANSMIT           0x07
#define SETUP_RETR_ARC_8_RETRANSMIT           0x08
#define SETUP_RETR_ARC_9_RETRANSMIT           0x09
#define SETUP_RETR_ARC_10_RETRANSMIT          0x0A
#define SETUP_RETR_ARC_11_RETRANSMIT          0x0B
#define SETUP_RETR_ARC_12_RETRANSMIT          0x0C
#define SETUP_RETR_ARC_13_RETRANSMIT          0x0D
#define SETUP_RETR_ARC_14_RETRANSMIT          0x0E
#define SETUP_RETR_ARC_15_RETRANSMIT          0x0F
#define SETUP_RETR_ARD_250us                  0x00
#define SETUP_RETR_ARD_500us           		  0x10
#define SETUP_RETR_ARD_750us          		  0x20
#define SETUP_RETR_ARD_1000us          		  0x30
#define SETUP_RETR_ARD_1250us        	      0x40
#define SETUP_RETR_ARD_1500us       	      0x50
#define SETUP_RETR_ARD_1750us         		  0x60
#define SETUP_RETR_ARD_2000us        	      0x70
#define SETUP_RETR_ARD_2250us         		  0x80
#define SETUP_RETR_ARD_2500us         		  0x90
#define SETUP_RETR_ARD_2750us       	      0xA0
#define SETUP_RETR_ARD_3000us         		  0xB0
#define SETUP_RETR_ARD_3250us                 0xC0
#define SETUP_RETR_ARD_3500us                 0xD0
#define SETUP_RETR_ARD_3750us                 0xE0
#define SETUP_RETR_ARD_4000us                 0xF0

//RF_CH REGISTER
#define RF_CH_MASK  0x7F

//RF_SETUP REGISTER
#define RF_SETUP_RF_PWR_18dBm        		  0x00
#define RF_SETUP_RF_PWR_12dBm   	 		  0x02
#define RF_SETUP_RF_PWR_16dBm   	 		  0x04
#define RF_SETUP_RF_PWR_0dBm    	 		  0x06
#define RF_SETUP_RF_DR_HIGH_1Mbps             0x00
#define RF_SETUP_RF_DR_HIGH_2Mbps    		  0x08
#define RF_SETUP_RF_DR_HIGH_250kbps  		  0x20
#define RF_SETUP_RF_DR_LOW					  0x20
#define RF_SETUP_PLL_LOCK                     0x10
#define RF_SETUP_CONT_WAVE                    0x80
#define RF_SETUP_MASK						  0xBE

//STATUS REGISTER
#define STATUS_TX_FULL_MASK   		          0x01//R
#define STATUS_RX_P_NO_0_MASK  		          0x00//R
#define STATUS_RX_P_NO_1_MASK  		          0x02//R
#define STATUS_RX_P_NO_2_MASK  		          0x04//R
#define STATUS_RX_P_NO_3_MASK  		          0x06//R
#define STATUS_RX_P_NO_4_MASK 		          0x08//R
#define STATUS_RX_P_NO_5_MASK 		          0x09//R
#define STATUS_MAX_RT		                  0x10
#define STATUS_TX_DS		                  0x20
#define STATUS_RX_DR                          0x40
#define STATUS_MASK                           0x7F

//OBSERVE_TX REGISTER
#define OBSERVE_TX_PLOS_CNT_MASK 		      0xF0//R
#define OBSERVE_TX_ARC_CNT_MASK 		      0x0F//R
//RPD REGISTER
#define RPD_RPD_MASK 		                  0x01//R
#define RPD_MASK 		                      0xFE

//RX_PW_Px REGISTER
#define RX_PW_Px_PIPE_NOT_USED                0x00
#define RX_PW_Px_1_BYTE                       0x01
#define RX_PW_Px_2_BYTES					  0x02
#define RX_PW_Px_3_BYTES 					  0x03
#define RX_PW_Px_4_BYTES					  0x04
#define RX_PW_Px_5_BYTES					  0x05
#define RX_PW_Px_6_BYTES					  0x06
#define RX_PW_Px_7_BYTES					  0x07
#define RX_PW_Px_8_BYTES					  0x08
#define RX_PW_Px_9_BYTES					  0x09
#define RX_PW_Px_10_BYTES					  0x0A
#define RX_PW_Px_11_BYTES				      0x0B
#define RX_PW_Px_12_BYTES					  0x0C
#define RX_PW_Px_13_BYTES					  0x0D
#define RX_PW_Px_14_BYTES					  0x0E
#define RX_PW_Px_15_BYTES					  0x0F
#define RX_PW_Px_16_BYTES					  0x10
#define RX_PW_Px_17_BYTES					  0x11
#define RX_PW_Px_18_BYTES					  0x12
#define RX_PW_Px_19_BYTES					  0x13
#define RX_PW_Px_20_BYTES					  0x14
#define RX_PW_Px_21_BYTES					  0x15
#define RX_PW_Px_22_BYTES					  0x16
#define RX_PW_Px_23_BYTES					  0x17
#define RX_PW_Px_24_BYTES					  0x18
#define RX_PW_Px_25_BYTES					  0x19
#define RX_PW_Px_26_BYTES				      0x1A
#define RX_PW_Px_27_BYTES					  0x1B
#define RX_PW_Px_28_BYTES					  0x1C
#define RX_PW_Px_29_BYTES					  0x1D
#define RX_PW_Px_30_BYTES					  0x1E
#define RX_PW_Px_31_BYTES					  0x1F
#define RX_PW_Px_32_BYTES					  0x20
#define RX_PW_Px_MASK                         0x3F

//FIFO_STATUS REGISTER
#define FIFO_STATUS_RX_EMPTY_MASK  		      0x01//R
#define FIFO_STATUS_RX_FULL_MASK  		      0x02//R
#define FIFO_STATUS_TX_EMPTY_MASK  		      0x10//R
#define FIFO_STATUS_TX_FULL_MASK  		      0x20//R
#define FIFO_STATUS_TX_REUSE_MASK  		      0x40//R
#define FIFO_STATUS_MASK 		              0x73

//DYNPD REGISTER
#define DYNPD_DPL_P0  		      			  0x01
#define DYNPD_DPL_P1  		      			  0x02
#define DYNPD_DPL_P2  		      			  0x04
#define DYNPD_DPL_P3  		      			  0x08
#define DYNPD_DPL_P4  		      			  0x10
#define DYNPD_DPL_P5 		      			  0x20
#define DYNPD_MASK 		          			  0x3F

//FEATURE REGISTER
#define FEATURE_EN_DYN_ACK  		      	  0x01
#define FEATURE_EN_ACK_PAY  		      	  0x02
#define FEATURE_EN_DPL  		      		  0x04
#define FEATURE_MASK 		          		  0x07


#endif /* __NRF24L01_REGISTERS_H_ */
