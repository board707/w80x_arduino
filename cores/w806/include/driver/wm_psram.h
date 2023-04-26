#ifndef __WM_PSRAM_H__
#define __WM_PSRAM_H__
#include "wm_hal.h"
#define PSRAM_ADDR_START          0x30000000
#define PSRAM_SIZE_BYTE           0x00800000

typedef enum{
	PSRAM_SPI = 0,
	PSRAM_QPI,
} psram_mode_t;

/**
 * @brief          This function is used to init the psram .
 * @param[in]      mode   is work mode, PSRAM_SPI or PSRAM_QPI
 * @retval        none
 * @note           None
 */
void psram_init(psram_mode_t mode);

#endif

