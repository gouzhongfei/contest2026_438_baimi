/****************************************************************************
 * apps/velacare/velacare_main.c — VelaCare 主进程入口
 *
 * 队伍 438 (baimi) · 2026 openvela AI 硬件开发者大赛
 *
 * 职责:
 *   1. 初始化 4 级状态机
 *   2. 装载 4 个 Skill 到 ai_agent
 *   3. 注册 cron 定时任务
 *   4. 启动 LVGL 显示线程
 *   5. 进入主循环, 1Hz 轮询传感器 + 评估状态机
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <syslog.h>
#include <time.h>

#include "velacare_state.h"
#include "velacare_sensor.h"
#include "velacare_skill.h"
#include "velacare_lvgl.h"
#include "velacare_tts.h"
#include "velacare_cron.h"

/****************************************************************************
 * 私有宏
 ****************************************************************************/

#define VELACARE_TAG       "[VelaCare]"
#define VELACARE_LOG(fmt, ...) \
    syslog(LOG_NOTICE, VELACARE_TAG " " fmt "\n", ##__VA_ARGS__)

/****************************************************************************
 * 私有函数
 ****************************************************************************/

/**
 * velacare_on_state_change - 状态变更回调
 *
 * 当状态机发生切换时被调用, 负责:
 *   - 打印状态变更日志
 *   - 触发对应的 Skill 执行
 *   - 更新 LVGL 屏幕显示
 *
 * @param new_state  新状态
 * @param old_state  旧状态
 * @param reason     变更原因描述
 */

static void velacare_on_state_change(int new_state, int old_state,
                                     const char *reason)
{
    VELACARE_LOG("State: %s -> %s (%s)",
                 velacare_state_name(old_state),
                 velacare_state_name(new_state),
                 reason ? reason : "N/A");

    /* 更新 LVGL 屏幕显示 */

    velacare_lvgl_update_state(new_state);

    /* 紧急状态触发 TTS 告警 */

    if (new_state == VELACARE_STATE_EMERGENCY)
        {
            velacare_tts_speak("紧急情况，请注意安全");
        }
}

/**
 * velacare_sensor_poll - 模拟传感器轮询
 *
 * 在真机上替换为 HAL 层传感器读取。
 * 当前使用随机数模拟传感器状态变化。
 *
 * @param ctx  状态机上下文
 */

static void velacare_sensor_poll(struct velacare_state_s *ctx)
{
    /* 读取传感器状态 */

    ctx->smoke_online = velacare_sensor_smoke_online();
    ctx->smoke_alarm  = velacare_sensor_smoke_alarm();
    ctx->water_online = velacare_sensor_water_online();
    ctx->water_alarm  = velacare_sensor_water_alarm();
    ctx->door_online  = velacare_sensor_door_online();
    ctx->door_alarm   = velacare_sensor_door_alarm();
    ctx->fall_online  = velacare_sensor_fall_online();
    ctx->fall_alarm   = velacare_sensor_fall_alarm();
}

/**
 * velacare_sleep_ms - 毫秒级睡眠
 *
 * @param ms  睡眠毫秒数
 */

static void velacare_sleep_ms(unsigned int ms)
{
    struct timespec ts;
    ts.tv_sec  = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
}

/****************************************************************************
 * 公共函数
 ****************************************************************************/

/**
 * main - VelaCare 主入口
 *
 * @param argc  参数个数
 * @param argv  参数数组
 * @return      0=正常退出, <0=错误
 */

int main(int argc, char *argv[])
{
    struct velacare_state_s state;
    unsigned long tick_ms = 0;

    VELACARE_LOG("=== VelaCare v0.1 starting ===");
    VELACARE_LOG("Family Health Guardian - Team 438");

    /* 1. 初始化状态机 */

    velacare_state_init(&state);
    state.on_state_change = velacare_on_state_change;

    VELACARE_LOG("State machine init: %s", velacare_state_name(state.current));

    /* 2. 初始化子系统 */

    velacare_skill_init();
    velacare_lvgl_init();
    velacare_cron_init();

    VELACARE_LOG("Entering main loop (1Hz)");

    /* 5. 主循环: 1Hz 传感器轮询 + 状态评估 */

    while (1)
        {
            /* 轮询传感器 */

            velacare_sensor_poll(&state);

            /* 评估状态机 */

            if (velacare_state_evaluate(&state, tick_ms))
                {
                    /* 状态已变更, 回调中处理后续动作 */

                    if (state.on_state_change)
                        {
                            state.on_state_change(
                                state.current,
                                state.previous,
                                "sensor_poll");
                        }
                }

            /* 1Hz 节拍 */

            tick_ms += 1000;
            velacare_sleep_ms(1000);
        }

    return 0;
}
