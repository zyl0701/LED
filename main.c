int main(void)
{
    rt_kprintf("\n========== 系统启动 ==========\n");
    
    // 1. 初始化所有硬件驱动
    led_init();
    key_init();
    buzzer_init();
    
    // 2. 初始化信号量（初始值为0）
    rt_sem_init(&key_sem, "key_sem", 0, RT_IPC_FLAG_PRIO);
    rt_kprintf("[IPC] 信号量创建完成\n");
    
    // 3. 初始化邮箱
    rt_mb_init(&msg_mb, "msg_mb", &mb_pool[0], sizeof(mb_pool) / 4, RT_IPC_FLAG_PRIO);
    rt_kprintf("[IPC] 邮箱创建完成\n");
    
    // 4. 创建并启动所有线程
    key_detect_thread_init();   // 线程1：按键检测
    led_control_thread_init();  // 线程2：LED控制
    buzzer_control_thread_init();// 线程3：蜂鸣器控制
    
    rt_kprintf("========== 系统就绪，等待按键 ==========\n");
    
    return 0;
}
