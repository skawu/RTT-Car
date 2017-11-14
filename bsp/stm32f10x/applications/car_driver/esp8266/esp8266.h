#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "stm32f10x.h"
#include <rtthread.h>


#define RT_SERIAL_WIFI_DEVICE_NAME  "uart2"


extern void esp8266_init(void);
extern rt_device_t rt_esp8266_set_device(const char *name);
extern void esp8266_test(void);


#endif
