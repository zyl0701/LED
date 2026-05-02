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


git checkout -b driver-main
# 负责：硬件初始化、LED线程、main函数、按键线程

/*  线程3：蜂鸣器控制线程 */
static void buzzer_control_thread_entry(void *parameter)
{
    rt_ubase_t msg;
    
    while (1)
    {
        // 等待邮箱消息
        if (rt_mb_recv(&msg_mb, &msg, RT_WAITING_FOREVER) == RT_EOK)
        {
            rt_kprintf("[线程3] 收到邮箱消息: %s, 启动蜂鸣器\n", (char *)msg);
            
            // 蜂鸣器响500ms
            rt_pin_write(BUZZER_PIN, PIN_LOW);
            rt_thread_mdelay(500);
            rt_pin_write(BUZZER_PIN, PIN_HIGH);
        }
    }
}
git checkout -b thread-function
# 负责：蜂鸣器线程、IPC机制

/* ==================== 线程初始化函数 ==================== */
static void key_detect_thread_init(void)
{
    rt_thread_t tid = rt_thread_create("key_detect",
                                        key_detect_thread_entry,
                                        RT_NULL,
                                        1024,  // 栈大小
                                        8,     // 优先级（较高）
                                        10);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
    else
        rt_kprintf("[错误] 按键检测线程创建失败\n");
}

static void led_control_thread_init(void)
{
    rt_thread_t tid = rt_thread_create("led_ctrl",
                                        led_control_thread_entry,
                                        RT_NULL,
                                        512,
                                        10,    // 优先级（较低）
                                        10);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
    else
        rt_kprintf("[错误] LED控制线程创建失败\n");
}
static void buzzer_control_thread_init(void)
{
    rt_thread_t tid = rt_thread_create("buzzer_ctrl",
                                        buzzer_control_thread_entry,
                                        RT_NULL,
                                        512,
                                        9,     // 优先级（中间）
                                        10);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
    else
        rt_kprintf("[错误] 蜂鸣器控制线程创建失败\n");
}