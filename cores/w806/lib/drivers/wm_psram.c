#include "wm_psram.h"

void psram_init(psram_mode_t mode)
{
	volatile unsigned int value ;
    value = tls_reg_read32(PSRAM_CTRL_ADDR);

	if(mode == PSRAM_QPI)
	{
		value |= 0x03;
	}

	/*reset psram*/
	value |= 0x01;
	tls_reg_write32(PSRAM_CTRL_ADDR, value);
	do{
		value = tls_reg_read32(PSRAM_CTRL_ADDR);
	}while(value&0x01);	

}
