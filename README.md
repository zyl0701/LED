# LED
<<<<<<< HEAD
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#define LED_PIN GET_PIN(B, 5)  // PB5引脚
/*信号量结构体*/
static struct rt_semaphore led_sem;
=======
/* 函数声明（解决未定义报错） */
static void led_init(void);
static int led_thread1_init(void);
static int led_thread2_init(void);
static void led_thread1_entry(void *parameter);
static void led_thread2_entry(void *parameter);

/* LED初始化 */
static void led_init(void)
{
    rt_pin_mode(LED_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(LED_PIN, PIN_HIGH);  // 初始熄灭
    rt_kprintf("LED初始化完成！\n");
}

/* 线程1：慢闪烁（500ms） */
static void led_thread1_entry(void *parameter)
{
    while (1)
    {
        // 慢闪
        rt_pin_write(LED_PIN, PIN_LOW);
        rt_thread_mdelay(500);
        rt_pin_write(LED_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        // 发送信号量触发线程2
        rt_sem_release(&led_sem);
        rt_kprintf("线程1：发送快闪信号\n");
    }
}
/* 主函数 */
int main(void)
{
    // 1. 初始化LED
    led_init();
    
    // 2. 初始化信号量
    rt_sem_init(&led_sem, "led_sem", 0, RT_IPC_FLAG_PRIO);
    rt_kprintf("信号量创建完成，启动多线程\n");
}