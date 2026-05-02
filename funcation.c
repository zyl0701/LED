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