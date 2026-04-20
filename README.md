# LED
<<<<<<< HEAD
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#define LED_PIN GET_PIN(B, 5)  // PB5引脚

static struct rt_semaphore led_sem;
=======
/* 函数声明（解决未定义报错） */
static void led_init(void);
static int led_thread1_init(void);
static int led_thread2_init(void);
static void led_thread1_entry(void *parameter);
static void led_thread2_entry(void *parameter);

