#include "wm_psram.h"

/**
  * @brief  Initializes the PSRAM according to the specified parameters
  *         in the PSRAM_InitTypeDef and create the associated handle.
  * @param  hpsram pointer to a PSRAM_HandleTypeDef structure that contains
  *         the configuration information for PSRAM module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PSRAM_Init(PSRAM_HandleTypeDef *hpsram)
{
	uint32_t value = 0x605;
	
	if(hpsram == NULL)
	{
		return HAL_ERROR;
	}
	assert_param(IS_PSRAM_INSTANCE(hpsram->Instance));
	assert_param(IS_PSRAM_DIV(hpsram->Init.Div));
	assert_param(IS_PSRAM_MODE(hpsram->Init.Mode));
	
	HAL_PSRAM_MspInit(hpsram);
	
	value |= ((hpsram->Init.Div << PSRAM_CR_DIV_Pos) | hpsram->Init.Mode);
	WRITE_REG(hpsram->Instance->CR, value);
	do{
		value = READ_REG(hpsram->Instance->CR);
	} while(value & 0x01);
	
	return HAL_OK;
}

/**
  * @brief  DeInitializes the PSRAM peripheral
  * @param  hpsram pointer to a PSRAM_HandleTypeDef structure that contains
  *         the configuration information for PSRAM module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PSRAM_DeInit(PSRAM_HandleTypeDef *hpsram)
{
	if (hpsram == NULL)
	{
		return HAL_ERROR;
	}
	
	assert_param(IS_PSRAM_INSTANCE(hpsram->Instance));
	HAL_PSRAM_MspDeInit(hpsram);
	
	return HAL_OK;
}

/**
  * @brief PSRAM MSP Init
  * @param  hpsram pointer to a PSRAM_HandleTypeDef structure that contains
  *         the configuration information for PSRAM module
  * @retval None
  */
__attribute__((weak)) void HAL_PSRAM_MspInit(PSRAM_HandleTypeDef *hpsram)
{
	UNUSED(hpsram);
}

/**
  * @brief PSRAM MSP DeInit
  * @param  hpsram pointer to a PSRAM_HandleTypeDef structure that contains
  *         the configuration information for PSRAM module
  * @retval None
  */
__attribute__((weak)) void HAL_PSRAM_MspDeInit(PSRAM_HandleTypeDef *hpsram)
{
	UNUSED(hpsram);
}