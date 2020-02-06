  /********************************  cola os  *********************************
  * @file   : cola_init.c
  * @author : sunny.Bit(QQ 1506564994)
              Email:1506564994@qq.com
  * @brief  : cola os		
  ******************************************************************************/
#include "cola_init.h"

void do_init_call(void)
{
#if defined(__CC_ARM)                         /* ARM Compiler */
    extern initcall_t initcall0init$$Base[];
    extern initcall_t initcall0init$$Limit[];
    extern initcall_t initcall1init$$Base[];
    extern initcall_t initcall1init$$Limit[];
    extern initcall_t initcall2init$$Base[];
    extern initcall_t initcall2init$$Limit[];
    extern initcall_t initcall3init$$Base[];
    extern initcall_t initcall3init$$Limit[];

    initcall_t *fn;

    for (fn = initcall0init$$Base;
            fn < initcall0init$$Limit;
            fn++)
    {
        if(fn)
            (*fn)();
    }

    for (fn = initcall1init$$Base;
            fn < initcall1init$$Limit;
            fn++)
    {
        if(fn)
            (*fn)();
    }

    for (fn = initcall2init$$Base;
            fn < initcall2init$$Limit;
            fn++)
    {
        if(fn)
            (*fn)();
    }

    for (fn = initcall3init$$Base;
            fn < initcall3init$$Limit;
            fn++)
    {
        if(fn)
            (*fn)();
    }
 #endif // defined
}
