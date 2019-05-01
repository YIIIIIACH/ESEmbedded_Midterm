#include <stdint.h>
#include "reg.h"
#include "blink.h"

void op_sysclk(unsigned int div);
volatile uint32_t array[6]={0xA, 0xB, 0xC, 0xD, 0xE, 0xF};
int main(void)
{
	blink(LED_GREEN);   //CHANGE	
	op_sysclk(4);
}

/**
 * 
 * output sysclk (PC9)
 * 
 */
void op_sysclk(unsigned int div)
{
	//RCC
	SET_BIT(RCC_BASE + RCC_CFGR_OFFSET, MCO2_1_BIT);
	SET_BIT(RCC_BASE + RCC_CFGR_OFFSET, MCO2_0_BIT);  //change

	if (div == 1)
		CLEAR_BIT(RCC_BASE + RCC_CFGR_OFFSET, MCO2PRE_2_BIT);

	else if (div == 4)
	{
		SET_BIT(RCC_BASE + RCC_CFGR_OFFSET, MCO2PRE_2_BIT);
		SET_BIT(RCC_BASE + RCC_CFGR_OFFSET, 28);
		CLEAR_BIT(RCC_BASE + RCC_CFGR_OFFSET, MCO2PRE_0_BIT);
	}
	else
		while(1)
			;

	SET_BIT(RCC_BASE + RCC_AHB1ENR_OFFSET, GPIO_EN_BIT(GPIO_PORTC));

	//GPIO MODER
	CLEAR_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_MODER_OFFSET, MODERy_0_BIT(9));
	SET_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_MODER_OFFSET, MODERy_1_BIT(9));
	//Output push-pull
	CLEAR_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_OTYPER_OFFSET, OTy_BIT(9));

	//OSPEEDR9 = 11 => Very high speed
	CLEAR_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_1_BIT(9));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_OSPEEDR_OFFSET, OSPEEDRy_0_BIT(9));

	//PUPDR9 = 00 => No pull-up, pull-down
	CLEAR_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(9));
	CLEAR_BIT(GPIO_BASE(GPIO_PORTC) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(9));
}
