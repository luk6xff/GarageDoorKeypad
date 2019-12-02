/**
 * The Library for nrf24l01+ module written by Lukasz Uszko 07.07.2014
 *
 *
 *
 */


#include "nrf24l01.h"

/**
 * @name
 * @brief writes or reads data to/from the nrf24l01+ device
 * @retval
 *
 */

void nrf24l01_SPI_COMMAND(uint8_t* regData, int nbrOfBytesToReadWrite,
		_SPI_REGISTER_ADRESS_TypeDef regAdress, _SPI_COMMAND_TypeDef command) {
	uint8_t tempBuf[nbrOfBytesToReadWrite + 2]; // +2 more dimension for numberOfBytes to read/write and command+address
	switch (command) {

	case READ:
		tempBuf[0] = nbrOfBytesToReadWrite + 1;
		; // number of bytes to be read , only Status is read , but this amount is used as a dummy data to read
		tempBuf[1] = command | regAdress;
		(*SPI_ReadDataCallback)(tempBuf);
		break;
	case WRITE:
		tempBuf[0] = nbrOfBytesToReadWrite + 1; // number of bytes to be write
		tempBuf[1] = command | regAdress;
		memcpy(&tempBuf[2], regData, nbrOfBytesToReadWrite);
		(*SPI_WriteDataCallback)(tempBuf);
		break;
	case READ_PAYLOAD:
		tempBuf[0] = nbrOfBytesToReadWrite; // number of bytes to be write+commmand
		tempBuf[1] = command;
		(*SPI_ReadDataCallback)(tempBuf);
		break;
	case WRITE_PAYLOAD:
		tempBuf[0] = nbrOfBytesToReadWrite+1; // number of bytes to be write
		tempBuf[1] = command;
		memcpy(&tempBuf[2], regData, nbrOfBytesToReadWrite);
		(*SPI_WriteDataCallback)(tempBuf);
		break;
	case FLUSH__TX:
		tempBuf[0] = 1; // number of bytes to be write
		tempBuf[1] = command;
		(*SPI_WriteDataCallback)(tempBuf);
		break;
	case FLUSH__RX:
		tempBuf[0] = 1; // number of bytes to be write
		tempBuf[1] = command;
		(*SPI_WriteDataCallback)(tempBuf);
		break;
	case REUSE_LAST_TX_PAYLOAD:
		break;
	case READ_RX_PAYLOAD_WIDTH:
		break;
	case WRITE_PAYLOAD_WITH_ACK:
		break;
	case DISABLE_AUTO_ACK:
		break;
	case DO_NOTHING:
		tempBuf[0] = 1;
		tempBuf[1] = command;
		(*SPI_WriteDataCallback)(tempBuf);
		break;

	default:
		break;
	}

}
/**
 * @name void nrf24l01_SetTxMode(void)
 * @brief
 * @retval
 *
 */
void nrf24l01_SetTxMode(void) {
	uint8_t data = /*CONFIG_PWR_UP | */(CONFIG_PRIM_RX & 0);
	nrf24l01_SPI_COMMAND(&data, sizeof(data), CONFIG_regAdr, WRITE);
	nrf24l01_SetCE_PIN();
}
/**
 * @name nrf24l01_SetRxMode(void)
 * @brief
 * @retval
 *
 */
void nrf24l01_SetRxMode(void) {
	uint8_t data = CONFIG_PWR_UP | CONFIG_PRIM_RX;
	nrf24l01_SPI_COMMAND(&data, sizeof(data), CONFIG_regAdr, WRITE);
	nrf24l01_SetCE_PIN();
}

/**
 * @name nrf24l01_PowerUp(void)
 * @brief
 * @retval
 *
 */
void nrf24l01_PowerUp(void) {
	uint8_t data = CONFIG_PWR_UP;
	nrf24l01_SPI_COMMAND(&data, sizeof(data), CONFIG_regAdr, WRITE);

}

/**
 * @name nrf24l01_PowerDown(void)
 * @brief
 * @retval
 *
 */
void nrf24l01_PowerDown(void) {
	uint8_t data = CONFIG_PWR_UP & 0;
	nrf24l01_SPI_COMMAND(&data, sizeof(data), CONFIG_regAdr, WRITE);

}
/**
 * @name nrf24l01_SetCE_PIN(void)
 * @brief
 * @retval
 *
 */
void nrf24l01_SetCE_PIN(void) {
	if (CE_PinSetCallback != NULL)
		(*CE_PinSetCallback)();
}
/**
 * @name nrf24l01_ResetCE_PIN(void)
 * @brief
 * @retval
 *
 */
void nrf24l01_ResetCE_PIN(void) {

	if (CE_PinResetCallback != NULL)
		(*CE_PinResetCallback)();

}

/**
 * @name nrf24l01_NOP(void)
 * @brief
 * @retval
 *
 */
void nrf24l01_NOP(void) {
	nrf24l01_SPI_COMMAND(NULL, 0, 0, DO_NOTHING);

}

/**
 * @name nrf24l01_ReadSTATUS(void)
 * @brief
 * @retval
 *
 */
void nrf24l01_ReadSTATUS(void) {
	nrf24l01_SPI_COMMAND(NULL, 1, STATUS_regAdr, READ);
}

/**
 * @name  nrf24l01_FlushSTATUSReg((void)
 * @brief
 * @retval
 *
 */
void nrf24l01_FlushSTATUSReg(void) {
	uint8_t data = 0xFF;
	nrf24l01_SPI_COMMAND(&data, 1, STATUS_regAdr, WRITE);
}



/**
 * @name nrf24l01_ReadCONFIGReg(void)
 * @brief
 * @retval
 *
 */
void nrf24l01_ReadCONFIGReg(void) {
	nrf24l01_SPI_COMMAND(NULL, 1, CONFIG_regAdr, READ);
}

/**
 * @name  nrf24l01_SendDataToFifo(uint8_t*data)
 * @brief
 * @retval
 *
 */
void nrf24l01_SendDataToFifo(uint8_t*data) {
	//nrf24l01_SetTxMode();   i set up it in the init function

	nrf24l01_SPI_COMMAND(data, NUMBER_OF_BYTES_IN_FIFO, 0, WRITE_PAYLOAD);
	nrf24l01_SetCE_PIN();
	//nrf24l01_ResetCE_PIN();
}

/**
 * @name  void nrf24l01_FlushTx(void)
 * @brief
 * @retval
 *
 */
void nrf24l01_FlushTx(void) {
	nrf24l01_SPI_COMMAND(NULL, 1, 0, FLUSH__TX);
}

/**
 * @name  void nrf24l01_FlushRx(void)
 * @brief
 * @retval
 *
 */
void nrf24l01_FlushRx(void) {
	nrf24l01_SPI_COMMAND(NULL, 1, 0, FLUSH__RX);
}




/**
 * @name  void nrf24l01_Init(void)
 * @brief
 * @retval
 *
 */
void nrf24l01_Init(void)
{
	if (IRQ_PinSetupCallback != NULL) {
		(*IRQ_PinSetupCallback)();
	}
	nrf24l01_ResetCE_PIN(); // setup Tx MODE
	nrf24l01 obj;
	obj.CONFIG.PWR_UP = SET;
	obj.CONFIG.MASK_MAX_RT = SET;
	obj.CONFIG.MASK_RX_DR = SET;
	obj.CONFIG.MASK_TX_DS = SET;

	obj.CONFIG.PRIM_RX=RESET; // Tx Mode

	obj.EN_AA.ENAA_P0 = RESET;
	obj.EN_RXADDR.ERX_P0 = SET;
	//obj.RF_SETUP.RF_PWR = _0dBm;
	//obj.RF_SETUP.RF_DR_HIGH = SET;

	obj.RF_CH.RF_CH=40;
	obj.RX_PW_P0.RX_PW_P0 = _10_Bytes;

	nrf24l01_SPI_COMMAND((uint8_t*) &obj.CONFIG, 1, CONFIG_regAdr, WRITE);
	nrf24l01_SPI_COMMAND((uint8_t*) &obj.EN_AA, 1, EN_AA_ADR_regAdr, WRITE);
	nrf24l01_SPI_COMMAND((uint8_t*) &obj.EN_RXADDR, 1, EN_RXADDR_regAdr, WRITE);
	//nrf24l01_SPI_COMMAND((uint8_t*) &obj.RF_SETUP, 1, RF_SETUP_regAdr, WRITE);
	nrf24l01_SPI_COMMAND((uint8_t*) &obj.RX_PW_P0, 1, RX_PW_P0_regAdr, WRITE);
	nrf24l01_SetCE_PIN(); // setup Tx MODE
}

