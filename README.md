# LED
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#define LED_PIN GET_PIN(B, 5)  // PB5引脚

static struct rt_semaphore led_sem;
