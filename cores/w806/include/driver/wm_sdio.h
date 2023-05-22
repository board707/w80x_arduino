#ifndef __WM_SDIO_H__
#define __WM_SDIO_H__

#include "wm_hal.h"

#define SDIO	(SDIO_TypeDef *)SDIO_BASE


typedef struct
{
	uint32_t BusWide;			/* Specifies the SD bus width. This parameter can be a value of @SD_Bus_Wide */
	uint32_t ClockMode;			/* Specifies the clock speed mode.This parameter can be a value of @SD_Clock_Mode */
	uint32_t ClockDiv;			/* Specifies the clock frequency.This parameter can be a value of @SD_Clock_Div*/
} SD_InitTypeDef;

typedef enum
{
	HAL_SD_STATE_RESET,			/* SD not initialized or disabled */
	HAL_SD_STATE_READY,			/* SD initialized and ready for use */
	HAL_SD_STATE_TIMEOUT,		/* SD Timeout state */
	HAL_SD_STATE_BUSY,			/* SD process ongoing */
	HAL_SD_STATE_PROGRAMMING,	/* SD Programming State */
	HAL_SD_STATE_RECEIVING,		/* SD Receiving State */
	HAL_SD_STATE_TRANSFER,		/* SD Transfer State */
	HAL_SD_STATE_ERROR			/* SD is in error state */
}HAL_SD_StateTypeDef;

typedef enum
{
	HAL_SD_CARD_READY			= 0x00000001,
	HAL_SD_CARD_IDENTIFICATION,
	HAL_SD_CARD_STANDBY,
	HAL_SD_CARD_TRANSFER,
	HAL_SD_CARD_SENDING,
	HAL_SD_CARD_RECEIVING,
	HAL_SD_CARD_PROGRAMMING,
	HAL_SD_CARD_DISCONNECTED,
	HAL_SD_CARD_ERROR			= 0x000000FF,
}HAL_SD_CardStateTypeDef;

typedef struct
{
	uint32_t CardType;			/* Specifies the card Type */
	
	uint32_t CardVersion;		/* Specifies the card version */
	
	uint32_t Class;				/* Specifies the class of the card class */
	
	uint32_t RelCardAdd;		/* Specifies the Relative Card Address */
	
	uint32_t BlockNbr;			/* Specifies the Card Capacity in blocks */
	
	uint32_t BlockSize;			/* Specifies one block size in bytes */
	
	uint32_t LogBlockNbr;		/* Specifies the Card logical Capacity in blocks */
	
	uint32_t LogBlockSize;		/* Specifies logical block size in bytes */
	
}HAL_SD_CardInfoTypeDef;

#define SD_TypeDef		SDIO_TypeDef

typedef struct
{
	SD_TypeDef				*Instance;		/* SD registers base address */
	
	SD_InitTypeDef			Init;			/* SD required parameters */
	
	__IOM HAL_SD_StateTypeDef	State;		/* Sd Card State */
	
	DMA_HandleTypeDef		*hdmarx;		/* SD Rx DMA handle parameters */
	
	DMA_HandleTypeDef		*hdmatx;		/* SD Tx DMA handle parameters */
	
	HAL_SD_CardInfoTypeDef	SdCard;			/* SD Card information */
	
} SD_HandleTypeDef;



#define BLOCKSIZE 512


// SD_Bus_Wide
#define SD_BUS_WIDE_1B			((uint32_t)0x00000000U)
#define SD_BUS_WIDE_4B			SDIO_CR_BUSWIDE

// SD_Clock_Mode
#define SD_CLOCK_MODE_LOW_SPEED	((uint32_t)0x00000000U)
#define SD_CLOCK_MODE_HIGH_SPEED	SDIO_CR_HSPEED

// SD_Clock_Div
#define SD_CLOCK_DIV_1_2			SDIO_CR_CLKSEL_1_2
#define SD_CLOCK_DIV_1_4			SDIO_CR_CLKSEL_1_4
#define SD_CLOCK_DIV_1_6			SDIO_CR_CLKSEL_1_6
#define SD_CLOCK_DIV_1_8			SDIO_CR_CLKSEL_1_8
#define SD_CLOCK_DIV_1_10			SDIO_CR_CLKSEL_1_10
#define SD_CLOCK_DIV_1_12			SDIO_CR_CLKSEL_1_12
#define SD_CLOCK_DIV_1_14			SDIO_CR_CLKSEL_1_14
#define SD_CLOCK_DIV_1_16			SDIO_CR_CLKSEL_1_16


#define	CARD_V1_X		((uint32_t)0x00000000U)
#define CARD_V2_X		((uint32_t)0x00000001U)
#define CARD_V1_SDSC	((uint32_t)0x00000002U)
#define CARD_V2_SDSC	((uint32_t)0x00000003U)
#define CARD_V2_SDHC	((uint32_t)0x00000004U)

#define SDCMD_GO_IDLE_STATE				((uint8_t)0)
#define SDCMD_ALL_SEND_CID				((uint8_t)2)
#define SDCMD_SET_REL_ADDR				((uint8_t)3)
#define SDCMD_HS_SWITCH					((uint8_t)6)
#define SDCMD_SEL_DESEL_CARD			((uint8_t)7)
#define SDCMD_HS_SEND_EXT_CSD			((uint8_t)8)
#define SDCMD_SEND_CSD					((uint8_t)9)
#define SDCMD_STOP_TRANS				((uint8_t)12)
#define SDCMD_SEND_STATUS				((uint8_t)13)
#define SDCMD_SET_BLOCKLEN				((uint8_t)16)
#define SDCMD_RD_SINGLE_BLOCK			((uint8_t)17)
#define SDCMD_RD_MULT_BLOCK				((uint8_t)18)
#define SDCMD_WRITE_SINGLE_BLOCK		((uint8_t)24)
#define SDCMD_WRITE_MULT_BLOCK			((uint8_t)25)
#define SDCMD_APP_OP_COND				((uint8_t)41)
#define SDCMD_APP_CMD					((uint8_t)55)



#define IS_SD_ALL_INSTANCE(INSTANCE)	((INSTANCE) == SDIO)
#define IS_SD_BUS_WIDE(WIDE)			(((WIDE) == SD_BUS_WIDE_1B) || ((WIDE) == SD_BUS_WIDE_4B))
#define IS_SD_CLOCK_MODE(MODE)			(((MODE) == SD_CLOCK_MODE_LOW_SPEED) || ((MODE) == SD_CLOCK_MODE_HIGH_SPEED))
#define IS_SD_CLOCK_DIV(DIV)			(((DIV) == SD_CLOCK_DIV_1_2) || ((DIV) == SD_CLOCK_DIV_1_4) || \
										 ((DIV) == SD_CLOCK_DIV_1_6) || ((DIV) == SD_CLOCK_DIV_1_8) || \
										 ((DIV) == SD_CLOCK_DIV_1_10) || ((DIV) == SD_CLOCK_DIV_1_12) || \
										 ((DIV) == SD_CLOCK_DIV_1_14) || ((DIV) == SD_CLOCK_DIV_1_16))

#define __HAL_SD_GET_FLAG(__INSTANCE__, __FLAG__)	(((__INSTANCE__)->IF & (__FLAG__)))
#define __HAL_SD_CLEAR_FLAG(__INSTANCE__, __FLAG__) ((__INSTANCE__)->IF |= (__FLAG__))		

HAL_StatusTypeDef HAL_SD_Init(SD_HandleTypeDef *hsd);
HAL_StatusTypeDef HAL_SD_DeInit(SD_HandleTypeDef *hsd);
void HAL_SD_MspInit(SD_HandleTypeDef *hsd);
void HAL_SD_MspDeInit(SD_HandleTypeDef *hsd);

HAL_StatusTypeDef HAL_SD_ReadBlocks(SD_HandleTypeDef *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks, uint32_t Timeout);
HAL_StatusTypeDef HAL_SD_WriteBlocks(SD_HandleTypeDef *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks, uint32_t Timeout);
HAL_SD_CardStateTypeDef HAL_SD_GetCardState(SD_HandleTypeDef *hsd);
HAL_StatusTypeDef HAL_SD_GetCardInfo(SD_HandleTypeDef *hsd, HAL_SD_CardInfoTypeDef *pCardInfo);

#endif
