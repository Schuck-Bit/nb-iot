#include "config.h"
#include "bsp.h"
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "cola_init.h"
#include "cola_os.h"
void rcc_configuration(void)
{
  /* 使能APB2时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_GPIOB  | 
                         RCC_APB2Periph_GPIOC  | RCC_APB2Periph_GPIOD  |  
                         RCC_APB2Periph_AFIO   | RCC_APB2Periph_USART1 | 
                         RCC_APB2Periph_SPI1, ENABLE);

  /* 使能APB1时钟 */
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR    | RCC_APB1Periph_BKP  , ENABLE);


  /* 使能APB时钟 */
}
void SysTickInit()
{
    RCC_ClocksTypeDef RCC_Clocks;
    RCC_GetClocksFreq(&RCC_Clocks);
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);  
    SysTick_Config(RCC_Clocks.HCLK_Frequency /(configHZ));
    NVIC_SetPriority (SysTick_IRQn, 0);  
}
void bsp_rcc(void)
{  /* Start with the clocks in their expected state. */
    RCC_DeInit();

    /* Enable HSE (high speed external clock). */
    RCC_HSEConfig(RCC_HSE_ON);

    /* Wait till HSE is ready. */
    while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);

    /* 2 wait states required on the flash. */
    *((unsigned long *) 0x40022000) = 0x02;

    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* PLLCLK = 4MHz * 9 = 36 MHz. */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL. */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready. */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source. */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source. */
    while (RCC_GetSYSCLKSource() != 0x08)
    {
    }

    /* Set the Vector Table base address at 0x08000000 */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0); //???Bootloader???
	//NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x800); //??Bootloader???
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    /* Configure HCLK clock as SysTick clock source. */
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);

	rcc_configuration();
    SysTickInit();
}
pure_initcall(bsp_rcc);

void SysTick_Handler(void)
{
    cola_timer_ticker();
}
void bsp_init(void)
{
    do_init_call();
}

void enable_irq(void)
{
    __enable_irq();
}

void disable_irq(void)
{
    __disable_irq();
}
	
uint32_t get_pri(void )
{
    return __get_PRIMASK();
}

void set_pri(uint32_t x)
{
    __set_PRIMASK(x);
}

