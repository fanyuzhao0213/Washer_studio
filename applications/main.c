/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-31     RT-Thread    first version
 */

#include <rtthread.h>
#include "main.h"
#include "iwdg.h"

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

int main(void)
{
    int MainLoopCnt =0;
    Sys_init();
    TaskInit();
    while(1)
    {
      if(MainLoopCnt == 0)
         {
           Start_IWDG();
         }
      if(MainLoopCnt % 10 == 0)      // 1s FeedDog
         {
           Feed_Dog();
           LOG_D("1s喂狗\r\n");
         }
          rt_thread_delay(100);   /* 延时500个tick */
          MainLoopCnt++;
    }
}

    /* 设置空闲线程回调函数 */
//    rt_thread_idle_sethook(idle_hook);

