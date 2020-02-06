#include <string.h>
#include "app.h"
#include "config.h"
#include "cola_device.h"
#include "cola_os.h"
#include "app_uart.h"



void app_init(void)
{
    uart_task_init();
}
