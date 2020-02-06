
#ifndef _BSP_H
#define _BSP_H

/* 包含的头文件 --------------------------------------------------------------*/

#include <stdint.h>

void bsp_rcc(void);
void enable_irq(void);

void disable_irq(void);
	
uint32_t get_pri(void );

void set_pri(uint32_t x);
uint8_t get_run_mode(void);

#define OS_CPU_SR   uint32_t
#define enter_critical()        \
    do { cpu_sr = get_pri(); disable_irq();} while (0)
#define exit_critical()         \
    do { set_pri(cpu_sr);} while (0)





///* 宏定义 --------------------------------------------------------------------*/


///* 函数申明 ------------------------------------------------------------------*/

void bsp_init(void);


#endif /* _BSP_H */

/**** Copyright (C)2016 sun. All Rights Reserved **** END OF FILE ****/
