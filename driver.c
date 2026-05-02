#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

/*硬件引脚定义  */
#define LED_PIN     GET_PIN(G, 7)   // PG7 - LED引脚
#define KEY_PIN     GET_PIN(A, 0)   // PA0 - 按键引脚（假设）
#define BUZZER_PIN  GET_PIN(G, 2)   // PG2 - 蜂鸣器引脚

/* 全局变量  */
static struct rt_semaphore key_sem;      // 信号量：按键触发
static struct rt_mailbox msg_mb;         // 邮箱：线程间数据传输
static char mb_pool[128];                // 邮箱存储池

/* 硬件初始化函数 */
static void led_init(void)
{
    rt_pin_mode(LED_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(LED_PIN, PIN_HIGH);  // 初始熄灭
    rt_kprintf("[驱动] LED初始化完成 (PG7)\n");
}

static void key_init(void)
{
    rt_pin_mode(KEY_PIN, PIN_MODE_INPUT_PULLUP);  // 上拉输入
    rt_kprintf("[驱动] 按键初始化完成 (PA0)\n");
}

static void buzzer_init(void)
{
    rt_pin_mode(BUZZER_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(BUZZER_PIN, PIN_HIGH);  // 初始关闭（假设高电平关闭）
    rt_kprintf("[驱动] 蜂鸣器初始化完成 (PG2)\n");
}
/* 线程1：按键检测线程 */
static void key_detect_thread_entry(void *parameter)
{
    rt_uint8_t last_state = PIN_HIGH;  // 上次按键状态
    rt_uint8_t current_state;
    
    while (1)
    {
        current_state = rt_pin_read(KEY_PIN);
        
        // 检测下降沿（按键按下）
        if (last_state == PIN_HIGH && current_state == PIN_LOW)
        {
            rt_kprintf("[线程1] 检测到按键按下\n");
            rt_sem_release(&key_sem);  // 释放信号量通知线程2
            
            // 通过邮箱发送消息给线程3
            char *msg = "key_pressed";
            rt_mb_send(&msg_mb, (rt_ubase_t)msg);
        }
        last_state = current_state;
        rt_thread_mdelay(20);  // 消抖延时
    }
}
