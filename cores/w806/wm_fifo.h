#ifndef _FIF0_H_
#define _FIFO_H_

//#include "wm_hal.h"
#include "Common.h"

typedef struct fifo_t {
    uint8_t *buf;
	uint32_t size;
	volatile uint32_t in;
	volatile uint32_t out;
} _fifo_str;

//#define min(x,y) ((x) < (y)?(x):(y))

int FifoInit(_fifo_str *p, uint8_t *fifo_addr, uint32_t fifo_size);

int FifoDataLen(_fifo_str *p);

int FifoSpaceLen(_fifo_str *p);

int FifoRead(_fifo_str *p, uint8_t *buf, uint32_t len);
 
int FifoReadChar(_fifo_str *p, uint8_t *buf);

int FifoWriteChar(_fifo_str *p, uint8_t b);

int FifoWrite(_fifo_str *p, uint8_t *buf, uint32_t len);

void FifoClear(_fifo_str *p);


#endif

