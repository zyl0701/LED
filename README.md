#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

/* 硬件引脚定义 */
#define LED_PIN     GET_PIN(G, 7)   // PG7 - LED引脚
#define KEY_PIN     GET_PIN(A, 0)   // PA0 - 按键引脚（假设）
#define BUZZER_PIN  GET_PIN(G, 2)   // PG2 - 蜂鸣器引脚

/* 全局变量 */
static struct rt_semaphore key_sem;      // 信号量：按键触发
static struct rt_mailbox msg_mb;         // 邮箱：线程间数据传输
static char mb_pool[128];                // 邮箱存储池








git checkout -b driver-main
# 负责：硬件初始化、LED线程、main函数、按键线程
