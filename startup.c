#include <stdint.h>
#include "reg.h"
#include "blink.h"

extern int main(void);

void set_sysclk_pll(void);

void reset_handler(void)
{
	//symbols defined in linker script
	extern uint32_t _mydata_lma_start;
	extern uint32_t _mydata_vma_start;
	extern uint32_t _mydata_vma_end;
	extern uint32_t _mybss_vma_start;
	extern uint32_t _mybss_vma_end;
	//note that for the variables above, "symbol value" is equivalent to the address we want
	//use "&" operator to get symbol values

	uint32_t *mydata_lstart_ptr = &_mydata_lma_start;
	uint32_t *mydata_vstart_ptr = &_mydata_vma_start;
	uint32_t *mydata_vend_ptr = &_mydata_vma_end;

	uint32_t *mybss_vstart_ptr = &_mybss_vma_start;
	uint32_t *mybss_vend_ptr = &_mybss_vma_end;

	uint32_t *src_ptr, *dst_ptr;

	src_ptr = mydata_lstart_ptr;
	dst_ptr = mydata_vstart_ptr;

	while (dst_ptr < mydata_vend_ptr)
		*dst_ptr++ = *src_ptr++;

	dst_ptr = mybss_vstart_ptr;

	while (dst_ptr < mybss_vend_ptr)
		*dst_ptr++ = 0;

	set_sysclk_pll();
	
	main();
}

/**
 * 
 * set sysclk pll (168 MHz)
 * 
 */
void set_sysclk_pll(void)
{
	//enable HSI
	//SET_BIT(RCC_BASE + RCC_CR_OFFSET , HSION_BIT );

	//wait
	//while (READ_BIT(RCC_BASE + RCC_CR_OFFSET, HSIRDY_BIT) != 1)
		;
	//
	//set pll
	//use HSI for PLL source
	CLEAR_BIT( RCC_BASE + RCC_PLLCFGR_OFFSET , PLLSRC_BIT);
	//f_HSI = 16 MHz
	//
	//N = 168
	//M = 4
	//
	//
	//
	//P = 4
	//
	//f_PLL_out = 168
	//
	WRITE_BITS(RCC_BASE + RCC_PLLCFGR_OFFSET, PLLP_1_BIT, PLLP_0_BIT, 0b01);
	WRITE_BITS(RCC_BASE + RCC_PLLCFGR_OFFSET, PLLN_8_BIT, PLLN_0_BIT, 168);
	WRITE_BITS(RCC_BASE + RCC_PLLCFGR_OFFSET, PLLM_5_BIT, PLLM_0_BIT, 0b100);
	

	//enable pll
	SET_BIT(RCC_BASE + RCC_CR_OFFSET , PLLON_BIT );
	
	//wait
	while (READ_BIT(RCC_BASE + RCC_CR_OFFSET, PLLRDY_BIT) != 1)
		;
	
	//enable flash prefetch buffer
	SET_BIT( FLASH_BASE + FLASH_ACR_OFFSET , PRFTEN_BIT );
	
	//set flash wait state = 5
	WRITE_BITS( FLASH_BASE + FLASH_ACR_OFFSET , LATENCY_2_BIT , LATENCY_0_BIT , 5);
	//use pll
	WRITE_BITS( RCC_BASE + RCC_CFGR_OFFSET ,SW_1_BIT,SW_0_BIT, 2);

	//wait
	while( (READ_BIT( RCC_BASE + RCC_CFGR_OFFSET ,SWS_1_BIT) == 0x0) && (READ_BIT( RCC_BASE + RCC_CFGR_OFFSET ,SWS_1_BIT) == 0x1))
		;
}
