# VelaCare — 评委指南

> **队伍 438 · 队名 baimi · 队长林明强 · 队员苟中飞**

## 作品简介

VelaCare 是一个运行在 D12x 开发板（匠芯创 RISC-V SoC）上的**家庭健康守护应用**，基于 openvela（NuttX RTOS）+ ai_agent 框架 + LVGL 图形界面。

## 核心特性

| 特性 | 说明 |
| --- | --- |
| 5 级状态机 | NORMAL → ATTENTION → WARNING → EMERGENCY → OFFLINE |
| Skill 技能系统 | 通过 ai_agent 加载 Markdown 格式的技能，按需调用 |
| 多模态提示 | LVGL 屏显 + TTS 语音 + 离线日志 |
| 本地优先 | 传感器数据本地聚合，异常先在本地判定 |
| 离线运行 | WiFi 断开超过 60 秒自动降级到 OFFLINE 模式 |

## 快速验证

### 1. 编译验证

```bash
# 在 WSL2 内执行
cd /root/openvela-work/contest2026_438_baimi
source vendor/artinchip/tools/env.sh
./build.sh vendor/artinchip/boards/d12x/demo68-nor/configs/nsh_lvgl -j4

# 手动启用 VELACARE
cd nuttx
sed -i 's/^# CONFIG_LVX_USE_DEMO_CONTEST2026_438_VELACARE is not set/CONFIG_LVX_USE_DEMO_CONTEST2026_438_VELACARE=y/' .config
make -j4 EXTRAFLAGS='-Wno-cpp -Wno-deprecated-declarations'

# 验证产物
ls -la nuttx.bin  # 应为 1,314,952 字节
grep VELACARE .config  # 应显示 CONFIG_LVX_USE_DEMO_CONTEST2026_438_VELACARE=y
```

### 2. 代码结构

```
app/velacare/
├── velacare_main.c        ← 主入口，1Hz 状态机循环
├── velacare_state.h/.c    ← 5 级状态机（核心）
├── velacare_sensor.h/.c   ← 传感器抽象（烟感/水浸/门磁/跌倒检测）
├── velacare_skill.h/.c    ← Skill 加载与意图识别
├── velacare_lvgl.h/.c     ← LVGL 屏显界面（5 状态配色）
├── velacare_tts.h/.c      ← TTS 语音播报
├── velacare_cron.h/.c     ← 定时任务（吃药提醒、活动检测）
├── Kconfig                ← NuttX Kconfig 注册项
├── Make.defs              ← NuttX Make.defs 集成
└── Makefile               ← NuttX 应用 Makefile
```

### 3. Skill 文件

```
data/agent/skills/
├── home-safety-guard.md      ← 家庭安全守护（烟雾/燃气/门窗/水浸）
├── elder-care-reminder.md    ← 老人关怀提醒（吃药/活动/跌倒）
├── emergency-broadcast.md    ← 紧急广播（喊话 + 联系家属）
├── routine-scheduler.md      ← 日常作息管理（起床/午休/熄灯）
└── README.md                 ← Skill 加载机制说明
```

## 技术亮点

1. **5 级状态机**：比常见的 3 级更精细，支持离线降级
2. **WiFi 断开时间戳**：用 `wifi_down_ms` 而非 `state_enter_ms`，避免状态误判
3. **传感器抽象层**：烟感/水浸/门磁/跌倒 4 类传感器，真机可替换 HAL
4. **NuttX 标准**：所有日志用 `syslog()`，符合 RTOS 规范
5. **编译验证通过**：7 个 .o 文件全部成功，nuttx.bin 1,314,952 字节

## AI Coding 日志

```
logs/gouzhongfei/
├── manifest.json
└── 2026-08-26/
    ├── claude_code__2026-08-26-002-velacare-skill.jsonl
    ├── claude_code__2026-08-26-004-velacare-cron.jsonl
    ├── claude_code__2026-08-26-006-velacare-wifi.jsonl
    ├── codex_cli__2026-08-26-001-velacare-state.jsonl
    ├── codex_cli__2026-08-26-003-velacare-lvgl.jsonl
    ├── codex_cli__2026-08-26-005-velacare-tts.jsonl
    └── codex_cli__2026-08-26-007-velacare-docs.jsonl
```

## 演示视频

3 分钟演示视频脚本见：`.notes/demo-video-script.md`

## 团队

| 角色 | 同学 | 任务 |
| --- | --- | --- |
| 队长 | 林明强 | 架构 + 状态机 + Skill 系统 |
| 队员 | 苟中飞 | LVGL UI + 传感器 + TTS + 演示 |

## 联系方式

- GitHub: https://github.com/gouzhongfei/contest2026_438_baimi
- PR: https://github.com/open-vela/contest2026_438_baimi/pull/1
