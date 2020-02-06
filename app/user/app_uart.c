  /********************************  cola os  *********************************
  * @file   : app_uart.c
  * @author : sunny.Bit(QQ 1506564994)
              Email:1506564994@qq.com
  * @brief  : cola os		
  ******************************************************************************/
#include "config.h"
#include <string.h>
#include "app.h"

#include "cola_device.h"
#include "cola_os.h"


static task_t uart_task;
static cola_device_t *uart_dev;


static uint8_t tmp[128] = {0};
static void uart_task_cb(uint32_t event)
{
    int err = 0;
    if(event&SIG_DATA)
    {
        err = cola_device_read(uart_dev,0,tmp,sizeof(tmp));
        if(err)
        {
            os_log("UART-RX:%d:",err);
            cola_device_write(uart_dev,0,tmp,err);
            os_log("\r\n");
        }
    }
}

static void uart_dev_init(void)
{
    uart_dev = cola_device_find("uart1");
    assert(uart_dev);
    cola_device_set_owner(uart_dev,&uart_task);
    struct serial_configure cfg;
    cfg.baud_rate = 115200;
    cola_device_cfg(uart_dev,&cfg,0);
}

void uart_task_init(void)
{
    uart_dev_init();
    cola_task_create(&uart_task,uart_task_cb);
}
