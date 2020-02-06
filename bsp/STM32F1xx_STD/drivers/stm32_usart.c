  /********************************  cola os  *********************************
  * @file   : stm32_uart.c
  * @author : sunny.Bit(QQ 1506564994)
              Email:1506564994@qq.com
  * @brief  : cola os		
  ******************************************************************************/

#include "config.h"
#include "stm32f10x_conf.h"
#include "cola_os.h"
#include "cola_device.h"
#include "cola_fifo.h"

#ifdef USING_UART1 


#ifdef USING_DEBUG
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0); 
    USART1->DR = (u8) ch;      
	return ch;
}
#endif

static cola_device_t uart1_dev;
static cola_fifo_t uart_fifo;
static task_t      uart_timer;
static uint8_t     uart_buf[256] = {0};

static void uart1_gpio_init(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
    /* 定义 USART-TX 引脚为复用输出 */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;                        //IO口的第9脚
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                  //IO口速度
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;                   //IO口复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);                             //USART输出IO口

    /* 定义 USART-Rx 引脚为悬空输入 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                         //IO口的第10脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;              //IO口悬空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);                             //USART输入IO口
}


static void uart1_init(uint32_t bund)
{
    USART_InitTypeDef USART_InitStructure;

    /******************************************************************
    USART参数初始化:  波特率     传输位数   停止位数  校验位数
        115200         8          1      0(NO)
    *******************************************************************/
    USART_InitStructure.USART_BaudRate = bund;                       //设定传输速率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;        //设定传输数据位数
    USART_InitStructure.USART_StopBits = USART_StopBits_1;             //设定停止位个数
    USART_InitStructure.USART_Parity = USART_Parity_No ;               //不用校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//不用流量控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    //使用接收和发送功能
    USART_Init(USART1, &USART_InitStructure);                          //初始化USART1

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                     //使能USART1接收中断

    USART_Cmd(USART1, ENABLE);                                         //使能USART1

    USART_ClearFlag(USART1,USART_FLAG_RXNE);
}
static void uart1_nvic_init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
   
    /* 外部中断 */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);                  
}
static void uart1_configuration(uint32_t bund)
{
    uart1_gpio_init();
    uart1_init(bund);
    uart1_nvic_init();
}
void USART1_SendByte(uint8_t Data)
{
  while((USART1->SR & USART_FLAG_TXE) == RESET);
  USART1->DR = (Data & (uint16_t)0x01FF);
}

static void uart1_sendnbyte(uint8_t *pData, uint16_t Length)
{
  while(Length--)
  {
    USART1_SendByte(*pData);
    pData++;
  }
}

static int uart1_write(cola_device_t *dev, int pos, const void *buffer, int size)
{
	uart1_sendnbyte((uint8_t *)buffer,size);
	return size;
}
static int uart1_read(cola_device_t *dev, int pos, void *buffer, int size)
{
	return cola_fifo_read(&uart_fifo,buffer,size);
}
static int  uart1_write_slot(uint8_t in[], uint8_t len)
{
	return  cola_fifo_write(&uart_fifo, in, len);
}

void USART1_IRQHandler(void)
{
  uint8_t c;

  if((USART1->SR & USART_FLAG_RXNE) == USART_FLAG_RXNE)
  {                                              
    c = (uint8_t)(USART1->DR);
    if(uart1_dev.owner)
    {
        if(uart1_write_slot( &c, 1))
        {
            cola_timer_start(&uart_timer,TIMER_ONE_SHOT,pdMS_TO_TICKS(20));
        }
    }	
  }
}
static int uart1_config(cola_device_t *dev, void *args, void *var)
{
	struct serial_configure *cfg = (struct serial_configure *)args;
	if(cfg)
	{
		uart1_init(cfg->baud_rate);
	}
	return 0;
}
static struct cola_device_ops uart1_ops =
{
	.write  = uart1_write,
	.read   = uart1_read,
	.config = uart1_config,
};

static void uart_timer_cb(uint32_t event)
{
    if(uart1_dev.owner)
    {
        cola_set_event(uart1_dev.owner,SIG_DATA);
    }
}

#endif
void board_setup_usart(void)
{
#ifdef USING_UART1
    uart1_configuration(115200);
    cola_fifo_init(&uart_fifo,uart_buf,sizeof(uart_buf));
    cola_timer_create(&uart_timer,uart_timer_cb);
    uart1_dev.name = "uart1";
    uart1_dev.dops = &uart1_ops;
    cola_device_register(&uart1_dev);
#endif
}
device_initcall(board_setup_usart);

