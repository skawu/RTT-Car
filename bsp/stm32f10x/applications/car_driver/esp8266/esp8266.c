#include "esp8266.h"

/*
    CH_PD	MP3_DREQ	PG7
    URXD	UTXD		USART2_TXD
    UTXD	URXD		USART2_RXD
    GPIO0	MP3_XDCS	PG6
    GPIO2	MP3_XREST	PG8
    GPIO16	MP3_CS		PE6
 */

static rt_device_t _wifi_device = RT_NULL;
rt_uint8_t wifi_read[64] = "";

void esp8266_init(void)
{
	// CH_PD需拉高
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_SetBits(GPIOG, GPIO_Pin_7);
	rt_esp8266_set_device("uart2");
}

void esp8266_test(void)
{
	char ch;
	rt_uint16_t old_flag = _wifi_device->open_flag;
	_wifi_device->open_flag |= RT_DEVICE_FLAG_STREAM;
	rt_device_write(_wifi_device, 0, "AT\r", 3);
	_wifi_device->open_flag = old_flag;

	while (rt_device_read(_wifi_device, 0, &ch, 1) == 1)
	{
		rt_kprintf("%c", ch);
	}
}


rt_device_t rt_esp8266_set_device(const char *name)
{
	rt_device_t new, old;
	old = _wifi_device;
	/* find new console device */
	new = rt_device_find(name);

	if (new != RT_NULL)
	{
		if (old != RT_NULL)
		{
			/* 如果有wifi设备，则先关闭再打开新设备 */
			rt_device_close(_wifi_device);
		}

		/* set  wifi device */
		rt_device_open(new, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_STREAM);
		_wifi_device = new;
	}

	return old;
}
RTM_EXPORT(rt_esp8266_set_device);

