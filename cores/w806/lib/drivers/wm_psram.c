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

/**
 * Added AnatolSher
 * Tested PIN configurations for Air103,W806,W801
 */
 void HAL_PSRAM_ConfigPin(uint8_t mode)
 {
	 switch(mode) {
	 case 0:	// for Air103
		  __HAL_AFIO_REMAP_PSRAM_CS(GPIOB, GPIO_PIN_27);
		  __HAL_AFIO_REMAP_PSRAM_CLK(GPIOA, GPIO_PIN_15);
		  __HAL_AFIO_REMAP_PSRAM_D0(GPIOB, GPIO_PIN_2);
		  __HAL_AFIO_REMAP_PSRAM_D1(GPIOB, GPIO_PIN_3);
		  __HAL_AFIO_REMAP_PSRAM_D2(GPIOB, GPIO_PIN_4);
		  __HAL_AFIO_REMAP_PSRAM_D3(GPIOB, GPIO_PIN_5);
		  __HAL_RCC_PSRAM_CLK_ENABLE();
		  break;
	 case 1:	// for W801,W806
		  __HAL_AFIO_REMAP_PSRAM_CS(GPIOB, GPIO_PIN_1);
		  __HAL_AFIO_REMAP_PSRAM_CLK(GPIOB, GPIO_PIN_0);
		  __HAL_AFIO_REMAP_PSRAM_D0(GPIOB, GPIO_PIN_2);
		  __HAL_AFIO_REMAP_PSRAM_D1(GPIOB, GPIO_PIN_3);
		  __HAL_AFIO_REMAP_PSRAM_D2(GPIOB, GPIO_PIN_4);
		  __HAL_AFIO_REMAP_PSRAM_D3(GPIOB, GPIO_PIN_5);
		  __HAL_RCC_PSRAM_CLK_ENABLE();
		  break;
	 default:
		 printf("psram config error!");
		 break;
	 }
 }
 /**
  * This code is provided by WinnerMicro
  * Dynamic memory allocation
  */
 /* Heap should align to DRAM_HEAP_ALIGNMENT */
 #define DRAM_HEAP_BASE              PSRAM_ADDR_START
 /* Size should be smaller than ~DRAM_HEAP_MAGIC */
 #define DRAM_HEAP_SIZE              (2*1024 * 1024)

 #define ALIGN(x,a)                  (((x) + (a) - 1) & ~((a) - 1))
 #define DRAM_HEAP_ALIGNMENT         4

 /* At the beginning of allocated memory */
 #define DRAM_HEAP_BLK_HEAD_SIZE     ALIGN(sizeof(ih_blk_head_t), DRAM_HEAP_ALIGNMENT)
 /* Block sizes must not get too small. */
 #define DRAM_HEAP_BLK_MIN	        ((DRAM_HEAP_BLK_HEAD_SIZE) << 1)

 /* blk belong to APP, magic set
    blk belong to free list, magic clear */
 #define DRAM_HEAP_MAGIC             (0xF0000000)
 /* block head, before each blocks */
 typedef struct ih_blklist
 {
     /* free memory block list, order by address*/
 	struct ih_blklist *next;
 	/* when blk in APP: DRAM_HEAP_MAGIC & size
 	   when blk in freelist: only size
 	   size include ih_blk_head_t self */
 	size_t magic_size;
 } ih_blk_head_t;

 /* free block list: order by address, from low to high */
 /* first free block. */
 static ih_blk_head_t s_freelist_head;
 /* point to last free block. */
 static ih_blk_head_t *s_freelist_tail;

 /* For statistic. */
 static size_t s_heap_free_size      = DRAM_HEAP_SIZE;
 static size_t s_heap_free_size_min  = DRAM_HEAP_SIZE;
 /* Init heap, with one free block */
 static void dram_heap_init(void)
 {
     ih_blk_head_t *free_blk;

     if (  DRAM_HEAP_BASE%DRAM_HEAP_ALIGNMENT != 0
        || DRAM_HEAP_SIZE > (~DRAM_HEAP_MAGIC)
        || DRAM_HEAP_SIZE < DRAM_HEAP_BLK_MIN*2 )
     {
         printf("[dram_heap] fatal error: heap parameter invalid!\n");
         return;
     }

 	s_freelist_head.next = (void *) DRAM_HEAP_BASE;
 	s_freelist_head.magic_size = (size_t) 0;

 	s_freelist_tail = (void *)(DRAM_HEAP_BASE + DRAM_HEAP_SIZE - DRAM_HEAP_BLK_HEAD_SIZE);
 	s_freelist_tail->next = NULL;
 	s_freelist_tail->magic_size = 0;

     /* Only one block when init. */
 	free_blk = (void *) DRAM_HEAP_BASE;
 	free_blk->next = s_freelist_tail;
 	free_blk->magic_size = (size_t)s_freelist_tail - (size_t) free_blk;

 	s_heap_free_size     = free_blk->magic_size;
 	s_heap_free_size_min = free_blk->magic_size;
 }

 static void dram_heap_freeblk_insert(ih_blk_head_t *blk_insert)
 {
     ih_blk_head_t *blk_before; /* before the blk_insert */
     ih_blk_head_t *blk_after;  /* after  the blk_insert */

     /* freelist is ordered by address, find blk_before */
     for (blk_before = &s_freelist_head; blk_before->next < blk_insert; blk_before = blk_before->next)
     {
         if (blk_before == s_freelist_tail)
         {
             return;
         }
     }
     blk_after = blk_before->next;

     /* now: blk_before < blk_insert < blk_after */

     /* try to merge blk_before and blk_insert */
 	if ((char *)blk_before + blk_before->magic_size == (char *)blk_insert)
     {
     	blk_before->magic_size += blk_insert->magic_size;
     	blk_insert = blk_before;
     }
     else
     {
         /* do not merge, just insert new node to freelist */
     	blk_before->next = blk_insert;
     }

     /* try to merge blk_insert and blk_after */
 	if (blk_after != s_freelist_tail &&
 	    (char *)blk_insert + blk_insert->magic_size == (char *)blk_after)
     {
     	blk_insert->magic_size += blk_after->magic_size;
     	blk_insert->next = blk_after->next;
     }
 	else
     {
         /* do not merge, just insert new node to freelist */
     	blk_insert->next = blk_after;
     }
 }

 void *dram_heap_malloc(size_t alloc_size)
 {
     ih_blk_head_t *blk_alloc;
     ih_blk_head_t *blk_prev;
     ih_blk_head_t *blk_left;

     /* first call to malloc, init is needed */
 	if (s_freelist_tail == NULL)
     {
     	// printf("heap init\r\n");
     	dram_heap_init();
     }

 	if  ((alloc_size == 0)
       || (alloc_size > s_heap_free_size))
     {
         /* not enough memory */
         return NULL;
     }

 	alloc_size += DRAM_HEAP_BLK_HEAD_SIZE;
     alloc_size  = ALIGN(alloc_size, DRAM_HEAP_ALIGNMENT);

     /* find a free block bigger than alloc_size */
 	blk_prev = &s_freelist_head;
 	blk_alloc = s_freelist_head.next;
 	while ((blk_alloc->magic_size < alloc_size) && (blk_alloc->next != NULL))
     {
     	blk_prev = blk_alloc;
     	blk_alloc = blk_alloc->next;
     }
 	if (blk_alloc->next == NULL)
     {
         /* this means "blk_alloc == s_freelist_tail" */
         return NULL;
     }

     /* delete blk_alloc from freelist */
 	blk_prev->next = blk_alloc->next;

     /* check whether blk_alloc can split */
 	if (blk_alloc->magic_size - alloc_size > DRAM_HEAP_BLK_MIN)
     {
         /* split */
         blk_left = (void *)((char *)blk_alloc + alloc_size);
     	blk_left->magic_size = blk_alloc->magic_size - alloc_size;

     	blk_alloc->magic_size = alloc_size;

         /* Insert the new block into the list of free blocks. */
     	dram_heap_freeblk_insert(blk_left);
     }

     /* update statistic */
 	s_heap_free_size -= blk_alloc->magic_size;
 	if (s_heap_free_size < s_heap_free_size_min)
     {
     	s_heap_free_size_min = s_heap_free_size;
     }

     /* blk belong to APP, magic set */
 	blk_alloc->magic_size |= DRAM_HEAP_MAGIC;
 	blk_alloc->next = NULL;

     /* app used addr is after ih_blk_head_t */
 	return (void *)((char *)blk_alloc + DRAM_HEAP_BLK_HEAD_SIZE);
 }

 void dram_heap_free(void *pfree)
 {
     ih_blk_head_t *free_blk;

 	if (pfree == NULL)
     {
         return;
     }

     /* app used addr is after ih_blk_head_t */
 	free_blk = (ih_blk_head_t *)((char *)pfree - DRAM_HEAP_BLK_HEAD_SIZE);

     /* blk damaged check */
     if (free_blk->next != NULL
       ||(free_blk->magic_size & DRAM_HEAP_MAGIC) != DRAM_HEAP_MAGIC)
     {
         printf("[dram_heap] fatal error: block has been destroyed!\n");
         return;
     }

     /* blk belong to free list, magic clear */
 	free_blk->magic_size &= ~DRAM_HEAP_MAGIC;
     /* update statistic */
 	s_heap_free_size += free_blk->magic_size;
 	dram_heap_freeblk_insert(free_blk);
 }

 size_t dram_heap_free_size(void)
 {
 	return s_heap_free_size;
 }

 size_t dram_heap_free_size_min(void)
 {
 	return s_heap_free_size_min;
 }

 /* if addr is in dram_heap, return 1
    else return 0*/
 int dram_heap_check_addr(void* addr)
 {
     if ((size_t)addr - DRAM_HEAP_BASE < DRAM_HEAP_SIZE)
     {
         return 1;
     }
     else
     {
         return 0;
     }
 }
