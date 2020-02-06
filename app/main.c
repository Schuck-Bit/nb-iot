  /********************************  cola os  *********************************
  * @file   : main.c
  * @author : sunny.Bit(QQ 1506564994)
  * @brief  : cola os		

  ******************************************************************************/
#include "bsp.h"
#include "cola_os.h"
#include "app.h"
int main(void)
{
    disable_irq();
    bsp_init();
#if defined(__CC_ARM)
    app_init();
#elif defined (__GNUC__)
    extern void uint_test(void);
    uint_test();
#endif
    enable_irq();
    while(1)
    {
#if defined(__CC_ARM)
        cola_task_loop();
#elif defined (__GNUC__)
        extern void gcc_tick_loop(void);
        gcc_tick_loop();
        cola_task_loop();
#endif

    }
}

