#include "wm_sdio.h"

static void SD_Init(SD_TypeDef *SDx, SD_InitTypeDef Init);
static void SDSendCmd(SD_TypeDef *SDx, uint8_t cmdnum, uint32_t cmdarg, uint8_t have_rsp);
static uint32_t SDWaitFlag(SD_TypeDef *SDx, uint32_t flag);
static void SDGetRsp(SD_TypeDef *SDx, uint32_t *rspbuf, uint32_t len);
static int SDCmdAndRsp(SD_TypeDef *SDx, uint32_t cmd, uint32_t arg, uint32_t *rspbuf, uint32_t rsp_len);

#if 0
static void DumpBuffer(uint32_t cmd, uint8_t *buf, uint32_t len)
{
	int i = 0;
	
	printf("CMD%d:\r\n", cmd);
	for (i = 0; i < len; i++)
	{
		if((i % 16) == 0)
		{
			printf("\r\n");
		}
		printf("%02x ", buf[i]);
	}
	printf("\r\n");
}
#endif

/**
  * @brief  Initializes the SD according to the specified parameters in the
            SD_HandleTypeDef and create the associated handle.
  * @param  hsd: Pointer to the SD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_Init(SD_HandleTypeDef *hsd)
{
	uint32_t count = 0, rspbuf[4], devicesize;
	SD_InitTypeDef Init;
	HAL_StatusTypeDef ret;
	uint8_t ready = 0;
	uint16_t rca;
	
	if (hsd == NULL)
	{
		return HAL_ERROR;
	}
	
	assert_param(IS_SD_ALL_INSTANCE(hsd->Instance));
	if (hsd->State == HAL_SD_STATE_RESET)
	{
		HAL_SD_MspInit(hsd);
	}
	hsd->State = HAL_SD_STATE_BUSY;
	
	// init low speed and 1 bus wide
	Init.BusWide = SD_BUS_WIDE_1B;
	Init.ClockMode = SD_CLOCK_MODE_LOW_SPEED;
	Init.ClockDiv = SD_CLOCK_DIV_1_4;
	SD_Init(hsd->Instance, Init);
	
	// CMD0
	SDSendCmd(hsd->Instance, SDCMD_GO_IDLE_STATE, 0, 0);
	ret = SDWaitFlag(hsd->Instance, SDIO_IF_CMDDONE);
	if (ret != HAL_OK)
	{
		goto ERROR;
	}
	
	// CMD8
	if (SDCmdAndRsp(hsd->Instance, SDCMD_HS_SEND_EXT_CSD, 0x1AA, rspbuf, 2) != HAL_OK)
	{
		goto ERROR;
	}
	if ((rspbuf[0] != 0x1AA) || ((rspbuf[1] & 0xFF) != SDCMD_HS_SEND_EXT_CSD))
	{
		hsd->SdCard.CardVersion = CARD_V1_X;
	}
	else
	{
		hsd->SdCard.CardVersion = CARD_V2_X;
	}
	
	while((count < 0x0FFFF) && (ready == 0))
	{
		// CMD55 APP_CMD with RCA as 0
		if (SDCmdAndRsp(hsd->Instance, SDCMD_APP_CMD, 0, rspbuf, 2) != HAL_OK)
		{
			goto ERROR;
		}
		if ((rspbuf[1] & 0xFF) != SDCMD_APP_CMD)
		{
			goto ERROR;
		}
		
		// CMD41 with Argument 0x80100000
		if (SDCmdAndRsp(hsd->Instance, SDCMD_APP_OP_COND, 0xC0100000, rspbuf, 2) != HAL_OK)
		{
			goto ERROR;
		}
		if ((rspbuf[1] & 0xFF) != 0x3F)
		{
			goto ERROR;
		}
		ready = (((rspbuf[0] >> 31) == 1) ? 1 : 0);
		count++;
	}
	
	if (count >= 0x0FFFF)
	{
		goto ERROR;
	}
	if (hsd->SdCard.CardVersion == CARD_V1_X)
	{
		hsd->SdCard.CardType = CARD_V1_SDSC;
	}
	else
	{
		if ((rspbuf[0] & 0x40000000) == 0x40000000)
		{
			hsd->SdCard.CardType = CARD_V2_SDHC;
		}
		else
		{
			hsd->SdCard.CardType = CARD_V2_SDSC;
		}
	}
	
	// CMD2
	if (SDCmdAndRsp(hsd->Instance, SDCMD_ALL_SEND_CID, 0, rspbuf, 4) != HAL_OK)
	{
		goto ERROR;
	}
	if (((rspbuf[3] >> 24) & 0xFF) != 0x3F)
	{
		goto ERROR;
	}
	
	// CMD3
	if (SDCmdAndRsp(hsd->Instance, SDCMD_SET_REL_ADDR, 1, rspbuf, 2) != HAL_OK)
	{
		goto ERROR;
	}
	if ((rspbuf[1] & 0xFF) != SDCMD_SET_REL_ADDR)
	{
		goto ERROR;
	}
	rca = rspbuf[0] >> 16;
	hsd->SdCard.RelCardAdd = rca;
	
	// CMD9
	if (SDCmdAndRsp(hsd->Instance, SDCMD_SEND_CSD, (rca << 16), rspbuf, 4) != HAL_OK)
	{
		goto ERROR;
	}
	if (((rspbuf[3] >> 24) & 0xFF) != 0x3F)
	{
		goto ERROR;
	}
	hsd->SdCard.Class = (rspbuf[1] >> 12) & 0x00FFF;
	if ((hsd->SdCard.CardType ==  CARD_V1_SDSC) || (hsd->SdCard.CardType ==  CARD_V2_SDSC))
	{
		devicesize = ((rspbuf[1] & 0x00000003) << 10) | ((rspbuf[0] & 0xFF000000) >> 22) | ((rspbuf[2] & 0x00C00000) >> 22);
		hsd->SdCard.BlockNbr = devicesize + 1;
		hsd->SdCard.BlockNbr *= (1 << (((rspbuf[2] & 0x00000380) >> 7) + 2));
		hsd->SdCard.BlockSize = (1 << ((rspbuf[1] & 0x00000F00) >> 8));
		hsd->SdCard.LogBlockNbr = (hsd->SdCard.BlockNbr) * ((hsd->SdCard.BlockSize) / 512);
		hsd->SdCard.LogBlockSize = 512;
	}
	else if (hsd->SdCard.CardType ==  CARD_V2_SDHC)
	{
		devicesize = ((rspbuf[0] & 0x3F000000) >> 8) | ((rspbuf[1] & 0x00FFFF00) >> 8);
		hsd->SdCard.BlockNbr = (devicesize + 1) * 1024;
		hsd->SdCard.LogBlockNbr = hsd->SdCard.BlockNbr;
		hsd->SdCard.BlockSize = 512;
		hsd->SdCard.LogBlockSize = hsd->SdCard.BlockSize;
	}
	
	
	// CMD7
	if (SDCmdAndRsp(hsd->Instance, SDCMD_SEL_DESEL_CARD, (rca << 16), rspbuf, 2) != HAL_OK)
	{
		goto ERROR;
	}
	if ((rspbuf[1] & 0xFF) != SDCMD_SEL_DESEL_CARD)
	{
		goto ERROR;
	}
	
	SD_Init(hsd->Instance, hsd->Init);
	if (hsd->Init.BusWide == SD_BUS_WIDE_4B)
	{
		// CMD55 APP_CMD with RCA as 0
		if (SDCmdAndRsp(hsd->Instance, SDCMD_APP_CMD, (rca << 16), rspbuf, 2) != HAL_OK)
		{
			goto ERROR;
		}
		if ((rspbuf[1] & 0xFF) != SDCMD_APP_CMD)
		{
			goto ERROR;
		}
		// CMD6 
		if (SDCmdAndRsp(hsd->Instance, SDCMD_HS_SWITCH, 2, rspbuf, 2) != HAL_OK)
		{
			goto ERROR;
		}
		if ((rspbuf[1] & 0xFF) != SDCMD_HS_SWITCH)
		{
			goto ERROR;
		}
	}
	
	hsd->State = HAL_SD_STATE_READY;
	return HAL_OK;
ERROR:
	hsd->State = HAL_SD_STATE_READY;
	return HAL_ERROR;
}

/**
  * @brief  De-Initializes the SD card.
  * @param  hsd: Pointer to SD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_DeInit(SD_HandleTypeDef *hsd)
{
	if (hsd == NULL)
	{
		return HAL_ERROR;
	}
	assert_param(IS_SDIO_ALL_INSTANCE(hsd->Instance));
	
	hsd->State = HAL_SD_STATE_BUSY;
	
	HAL_SD_MspDeInit(hsd);
	
	hsd->State = HAL_SD_STATE_RESET;
	
	return HAL_OK;
}

/**
 * @brief          Initializes the SD MSP.
 *
 * @param[in]      hsd    Pointer to SD handle.
 *
 * @retval         None
 *
 * @note           None
 */
__attribute__((weak)) void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
	UNUSED(hsd);
}

/**
 * @brief          De-Initializes the SD MSP.
 *
 * @param[in]      hsd    Pointer to SD handle.
 *
 * @retval         None
 *
 * @note           None
 */
__attribute__((weak)) void HAL_SD_MspDeInit(SD_HandleTypeDef *hsd)
{
	UNUSED(hsd);
}

/**
  * @brief  Reads block(s) from a specified address in a card. The Data transfer
  *         is managed by polling mode.
  * @param  hsd: Pointer to SD handle
  * @param  pData: pointer to the buffer that will contain the received data
  * @param  BlockAdd: Block Address from where data is to be read
  * @param  NumberOfBlocks: Number of SD blocks to read
  * @param  Timeout: Specify timeout value
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_ReadBlocks(SD_HandleTypeDef *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks, uint32_t Timeout)
{
	uint32_t add = BlockAdd, i = 0, data = 0;
	uint32_t tickstart = HAL_GetTick();
	uint8_t *p = pData;
	uint32_t rspbuf[4];
	
	if (pData == NULL)
	{
		return HAL_ERROR;
	}
	
	if (hsd->State == HAL_SD_STATE_READY)
	{
		if ((add + NumberOfBlocks) > (hsd->SdCard.LogBlockNbr))
		{
			return HAL_ERROR;
		}
		hsd->State = HAL_SD_STATE_BUSY;
		
		if (hsd->SdCard.CardType != CARD_V2_SDHC)
		{
			add *= BLOCKSIZE;
		}
		// CMD16
		if (SDCmdAndRsp(hsd->Instance, SDCMD_SET_BLOCKLEN, BLOCKSIZE, rspbuf, 2) != HAL_OK)
		{
			goto ERROR;
		}
		if ((rspbuf[1] & 0xFF) != SDCMD_SET_BLOCKLEN)
		{
			goto ERROR;
		}
		if (NumberOfBlocks > 1)
		{
			// CMD18
			if (SDCmdAndRsp(hsd->Instance, SDCMD_RD_MULT_BLOCK, add, rspbuf, 2) != HAL_OK)
			{
				goto ERROR;
			}
			if ((rspbuf[1] & 0xFF) != SDCMD_RD_MULT_BLOCK)
			{
				goto ERROR;
			}
			WRITE_REG(hsd->Instance->BUFCR, (SDIO_BUFCR_DMAEN | (1 << SDIO_BUFCR_FIFOLEVEL_Pos)));
			HAL_DMA_Start(hsd->hdmarx, (uint32_t)hsd->Instance->DATABUF, (uint32_t)pData, (NumberOfBlocks * BLOCKSIZE));
			WRITE_REG(hsd->Instance->BLOCKCNT, NumberOfBlocks);
			__HAL_SD_CLEAR_FLAG(hsd->Instance, 0x7FF);
			WRITE_REG(hsd->Instance->MBCTL, ((3 << SDIO_MBCTL_NACTIMEOUT_Pos) | (3 << SDIO_MBCTL_BUSYTIMEOUT_Pos) | SDIO_MBCTL_BLKTRANSDIR | SDIO_MBCTL_BLKSAUTOTREN));
			
			if(HAL_DMA_PollForTransfer(hsd->hdmarx, HAL_DMA_FULL_TRANSFER, Timeout) != HAL_OK)
			{
				goto ERROR;
			}
			while(READ_BIT(hsd->Instance->MBCTL, SDIO_MBCTL_BLKSAUTOTREN))
			{
				if (((HAL_GetTick() - tickstart) >= Timeout) || (Timeout == 0))
				{
					goto ERROR;
				}
			}
			// CMD12
			if (SDCmdAndRsp(hsd->Instance, SDCMD_STOP_TRANS, 0, rspbuf, 2) != HAL_OK)
			{
				goto ERROR;
			}
			if ((rspbuf[1] & 0xFF) != SDCMD_STOP_TRANS)
			{
				goto ERROR;
			}
		}
		else
		{
			// CMD17
			if (SDCmdAndRsp(hsd->Instance, SDCMD_RD_SINGLE_BLOCK, add, rspbuf, 2) != HAL_OK)
			{
				goto ERROR;
			}
			if ((rspbuf[1] & 0xFF) != SDCMD_RD_SINGLE_BLOCK)
			{
				goto ERROR;
			}
		
			WRITE_REG(hsd->Instance->BUFCR, 0x4020);
			__HAL_SD_CLEAR_FLAG(hsd->Instance, 0x7FF);
			WRITE_REG(hsd->Instance->IO, 0x3);
			
			while(__HAL_SD_GET_FLAG(hsd->Instance, SDIO_IF_DATADONE) == 0)
			{
				if (((HAL_GetTick() - tickstart) >= Timeout) || (Timeout == 0))
				{
					__HAL_SD_CLEAR_FLAG(hsd->Instance, 0x7FF);
					hsd->State = HAL_SD_STATE_READY;
					return HAL_TIMEOUT;
				}
			}
			__HAL_SD_CLEAR_FLAG(hsd->Instance, SDIO_IF_DATADONE);
			for (i = 0; i < 128; i++)
			{
				data = hsd->Instance->DATABUF[0];
				*p = (uint8_t)(data & 0xFF);
				p++;
				*p = (uint8_t)((data >> 8) & 0xFF);
				p++;
				*p = (uint8_t)((data >> 16) & 0xFF);
				p++;
				*p = (uint8_t)((data >> 24) & 0xFF);
				p++;
			}
		}
		hsd->State = HAL_SD_STATE_READY;
		return HAL_OK;
ERROR:
		hsd->State = HAL_SD_STATE_READY;
		return HAL_ERROR;
	}
	else
	{
		return HAL_ERROR;
	}
}

/**
  * @brief  Allows to write block(s) to a specified address in a card. The Data
  *         transfer is managed by polling mode.
  * @param  hsd: Pointer to SD handle
  * @param  pData: pointer to the buffer that will contain the data to transmit
  * @param  BlockAdd: Block Address where data will be written
  * @param  NumberOfBlocks: Number of SD blocks to write
  * @param  Timeout: Specify timeout value
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_WriteBlocks(SD_HandleTypeDef *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks, uint32_t Timeout)
{
	uint32_t add = BlockAdd, i = 0, data = 0;
	uint32_t tickstart = HAL_GetTick();
	uint8_t *p = pData;
	uint32_t rspbuf[4];
	
	if (pData == NULL)
	{
		return HAL_ERROR;
	}
	
	if (hsd->State == HAL_SD_STATE_READY)
	{
		if ((add + NumberOfBlocks) > (hsd->SdCard.LogBlockNbr))
		{
			return HAL_ERROR;
		}
		hsd->State = HAL_SD_STATE_BUSY;
		if (hsd->SdCard.CardType != CARD_V2_SDHC)
		{
			add *= BLOCKSIZE;
		}
		// CMD16
		if (SDCmdAndRsp(hsd->Instance, SDCMD_SET_BLOCKLEN, BLOCKSIZE, rspbuf, 2) != HAL_OK)
		{
			goto ERROR;
		}
		if ((rspbuf[1] & 0xFF) != SDCMD_SET_BLOCKLEN)
		{
			goto ERROR;
		}
		if (NumberOfBlocks > 1)
		{
			// CMD25
			if (SDCmdAndRsp(hsd->Instance, SDCMD_WRITE_MULT_BLOCK, add, rspbuf, 2) != HAL_OK)
			{
				goto ERROR;
			}
			if ((rspbuf[1] & 0xFF) != SDCMD_WRITE_MULT_BLOCK)
			{
				goto ERROR;
			}
			WRITE_REG(hsd->Instance->BUFCR, (SDIO_BUFCR_FIFODIR | SDIO_BUFCR_DMAEN | (8 << SDIO_BUFCR_FIFOLEVEL_Pos)));
			HAL_DMA_Start(hsd->hdmatx, (uint32_t)pData, (uint32_t)hsd->Instance->DATABUF, (NumberOfBlocks * BLOCKSIZE));
			WRITE_REG(hsd->Instance->BLOCKCNT, NumberOfBlocks);
			__HAL_SD_CLEAR_FLAG(hsd->Instance, 0x7FF);
			WRITE_REG(hsd->Instance->MBCTL, ((3 << SDIO_MBCTL_NACTIMEOUT_Pos) | (3 << SDIO_MBCTL_BUSYTIMEOUT_Pos) | SDIO_MBCTL_BLKSAUTOTREN));
			
			if(HAL_DMA_PollForTransfer(hsd->hdmatx, HAL_DMA_FULL_TRANSFER, Timeout) != HAL_OK)
			{
				goto ERROR;
			}
			while(READ_BIT(hsd->Instance->MBCTL, SDIO_MBCTL_BLKSAUTOTREN))
			{
				if (((HAL_GetTick() - tickstart) >= Timeout) || (Timeout == 0))
				{
					goto ERROR;
				}
			}
			// CMD12
			if (SDCmdAndRsp(hsd->Instance, SDCMD_STOP_TRANS, 0, rspbuf, 2) != HAL_OK)
			{
				goto ERROR;
			}
			if ((rspbuf[1] & 0xFF) != SDCMD_STOP_TRANS)
			{
				goto ERROR;
			}
		}
		else
		{
			// CMD24
			if (SDCmdAndRsp(hsd->Instance, SDCMD_WRITE_SINGLE_BLOCK, add, rspbuf, 2) != HAL_OK)
			{
				goto ERROR;
			}
			if ((rspbuf[1] & 0xFF) != SDCMD_WRITE_SINGLE_BLOCK)
			{
				goto ERROR;
			}
			
			WRITE_REG(hsd->Instance->BUFCR, 0x4820);
			for (i = 0; i < 128; i++)
			{
				data = (uint32_t)(*p);
				p++;
				data |= (((uint32_t)(*p)) << 8);
				p++;
				data |= (((uint32_t)(*p)) << 16);
				p++;
				data |= (((uint32_t)(*p)) << 24);
				p++;
				hsd->Instance->DATABUF[i] = data;
			}
			WRITE_REG(hsd->Instance->BYTECNT, BLOCKSIZE);
			__HAL_SD_CLEAR_FLAG(hsd->Instance, 0x7FF);
			WRITE_REG(hsd->Instance->IO, 0x1);
			
			while(__HAL_SD_GET_FLAG(hsd->Instance, SDIO_IF_DATADONE) == 0)
			{
				if (((HAL_GetTick() - tickstart) >= Timeout) || (Timeout == 0))
				{
					__HAL_SD_CLEAR_FLAG(hsd->Instance, 0x7FF);
					hsd->State = HAL_SD_STATE_READY;
					return HAL_TIMEOUT;
				}
			}
			__HAL_SD_CLEAR_FLAG(hsd->Instance, SDIO_IF_DATADONE);
			
		}
		hsd->State = HAL_SD_STATE_READY;
		return HAL_OK;
ERROR:
		hsd->State = HAL_SD_STATE_READY;
		return HAL_ERROR;
	}
	else
	{
		return HAL_ERROR;
	}
}

/**
  * @brief  Gets the SD status info.
  * @param  hsd: Pointer to SD handle
  * @retval the SD card status information
  */
HAL_SD_CardStateTypeDef HAL_SD_GetCardState(SD_HandleTypeDef *hsd)
{
	uint32_t rspbuf[2];
	HAL_SD_CardStateTypeDef state = HAL_SD_CARD_TRANSFER;
	
	if (SDCmdAndRsp(hsd->Instance, SDCMD_SEND_STATUS, (hsd->SdCard.RelCardAdd << 16), rspbuf, 2) != HAL_OK)
	{
		goto ERROR;
	}
	if ((rspbuf[1] & 0xFF) != SDCMD_SEND_STATUS)
	{
		goto ERROR;
	}
	
	state = (HAL_SD_CardStateTypeDef)((rspbuf[0] >> 9) & 0x0F);
ERROR:
	return state;
}

/**
  * @brief  Gets the SD card info.
  * @param  hsd: Pointer to SD handle
  * @param  pCardInfo: Pointer to the HAL_SD_CardInfoTypeDef structure that
  *         will contain the SD card status information
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_GetCardInfo(SD_HandleTypeDef *hsd, HAL_SD_CardInfoTypeDef *pCardInfo)
{
	pCardInfo->CardType     = (uint32_t)(hsd->SdCard.CardType);
	pCardInfo->CardVersion  = (uint32_t)(hsd->SdCard.CardVersion);
	pCardInfo->Class        = (uint32_t)(hsd->SdCard.Class);
	pCardInfo->RelCardAdd   = (uint32_t)(hsd->SdCard.RelCardAdd);
	pCardInfo->BlockNbr     = (uint32_t)(hsd->SdCard.BlockNbr);
	pCardInfo->BlockSize    = (uint32_t)(hsd->SdCard.BlockSize);
	pCardInfo->LogBlockNbr  = (uint32_t)(hsd->SdCard.LogBlockNbr);
	pCardInfo->LogBlockSize = (uint32_t)(hsd->SdCard.LogBlockSize);
	
	return HAL_OK;
}

static void SD_Init(SD_TypeDef *SDx, SD_InitTypeDef Init)
{
	wm_sys_clk sysclk;
	
	assert_param(IS_SD_BUS_WIDE(Init.BusWide));
	assert_param(IS_SD_CLOCK_MODE(Init.ClockMode));
	assert_param(IS_SD_CLOCK_DIV(Init.ClockDiv));
	
	SystemClock_Get(&sysclk);
	SDx->SEL = 0xC0 | (sysclk.cpuclk / 2 - 1);
	SDx->CR = Init.BusWide | Init.ClockMode | Init.ClockDiv | 0x03;
	SDx->CRCCTL = 0xC0;
	SDx->TIMEOUT = 0xFF;
}

static void SDSendCmd(SDIO_TypeDef *SDx, uint8_t cmdnum, uint32_t cmdarg, uint8_t have_rsp)
{
	uint32_t temp = 0;
	SDx->CMDBUF[4] = cmdnum | 0x40;
	SDx->CMDBUF[3] = (cmdarg >> 24) & 0xFF;
	SDx->CMDBUF[2] = (cmdarg >> 16) & 0xFF;
	SDx->CMDBUF[1] = (cmdarg >> 8) & 0xFF;
	SDx->CMDBUF[0] = (cmdarg & 0xFF);
	SDx->IF |= 0x7FF;
	temp = SDIO_IO_8CLKNULLEN;
	if ((cmdnum == SDCMD_SEND_CSD) || (cmdnum == SDCMD_ALL_SEND_CID))
	{
		temp |= SDIO_IO_CIDCSD;
	}
	if (have_rsp)
	{
		temp |= SDIO_IO_AUTORCVRSP;
	}
	SDx->IO = temp;
}

static HAL_StatusTypeDef SDWaitFlag(SDIO_TypeDef *SDx, uint32_t flag)
{
	volatile uint32_t count = 0x7FFFF;
	
	do{
		if (count-- == 0)
		{
			return HAL_TIMEOUT;
		}
	}while (!__HAL_SD_GET_FLAG(SDx, flag));
	
	__HAL_SD_CLEAR_FLAG(SDx, flag);
	
	return HAL_OK;
}

static void SDGetRsp(SDIO_TypeDef *SDx, uint32_t *rspbuf, uint32_t len)
{
	int i = 0;
	
	for(i = 0; i < len; i++)
	{
		rspbuf[i] = (SDx->CMDBUF[i * 4 + 3] << 24) | (SDx->CMDBUF[i * 4 + 2] << 16) | (SDx->CMDBUF[i * 4 + 1] << 8) | (SDx->CMDBUF[i * 4]);
	}
}

static int SDCmdAndRsp(SDIO_TypeDef *SDx, uint32_t cmd, uint32_t arg, uint32_t *rspbuf, uint32_t rsp_len)
{
	int ret;
	
	SDSendCmd(SDx, cmd, arg, 1);
	ret = SDWaitFlag(SDx, SDIO_IF_CMDDONE);
	if (ret != HAL_OK)
	{
		return ret;
	}
	SDGetRsp(SDx, rspbuf, rsp_len);
//	DumpBuffer(cmd, (uint8_t *)rspbuf, (rsp_len * 4));
	
	return HAL_OK;
}


