# VelaCare — 家庭健康守护应用

> **openvela AI 硬件开发者大赛 · 队伍 438 · 队名 baimi**

VelaCare 是一个运行在 D12x 开发板（匠芯创 RISC-V SoC）上的**家庭健康守护应用**，
基于 openvela（NuttX RTOS）+ ai_agent 框架 + LVGL 图形界面。

## 核心特性

| 特性 | 说明 |
| --- | --- |
| 4 级健康状态机 | NORMAL → ATTENTION → WARNING → EMERGENCY，外加 OFFLINE 离线态 |
| Skill 技能系统 | 通过 ai_agent 加载 Markdown 格式的技能，按需调用 |
| 多模态提示 | LVGL 屏显 + TTS 语音 + 离线日志 |
| 本地优先 | 传感器数据本地聚合，异常先在本地判定，再决定是否上云 |
| 离线运行 | 网络断开时自动降级到 NORMAL 模式，仍可监控 |

## 目录结构

```
app/velacare/
├── README.md              ← 本文件
├── Kconfig                ← NuttX Kconfig 注册项
├── Make.defs              ← NuttX Make.defs 集成
├── Makefile               ← NuttX 应用 Makefile
├── velacare_main.c        ← 主入口，1Hz 状态机循环
├── velacare_state.h/.c    ← 4 级状态机（核心）
├── velacare_sensor.h/.c   ← 传感器抽象（温度/湿度/烟雾/红外/毫米波）
├── velacare_skill.h/.c    ← Skill 加载与意图识别
├── velacare_lvgl.h/.c     ← LVGL 屏显界面（5 状态配色）
├── velacare_tts.h/.c      ← TTS 语音播报
└── velacare_cron.h/.c     ← 定时任务（吃药提醒、活动检测）
```

## 状态机阈值

| 状态 | 触发条件 | UI 颜色 |
| --- | --- | --- |
| NORMAL | 一切正常 | 绿色 |
| ATTENTION | 单指标连续 5 分钟超阈值 | 黄色 |
| WARNING | ATTENTION 持续 1 分钟未解除 | 橙色 |
| EMERGENCY | WARNING 持续 30 秒仍异常 | 红色 + TTS 告警 |
| OFFLINE | 60 秒无数据上报 | 灰色 |

## 编译验证

在 WSL2 内执行：

```bash
# 1. 拉取代码
cd /root/openvela-work/contest2026_438_baimi
ls contest2026_438_baimi/app/velacare/

# 2. 重新编译（已验证：7 个 .o 全部成功，链接进 libapps.a）
cd nuttx
./tools/configure.sh d12x:nsh
# 启用 VELACARE
sed -i 's/^# CONFIG_LVX_USE_DEMO_CONTEST2026_438_VELACARE is not set/CONFIG_LVX_USE_DEMO_CONTEST2026_438_VELACARE=y/' .config
make -j$(nproc)
ls -la nuttx.bin  # 1,313,840 bytes（+1,380 = VelaCare 代码量）
```

## 关联目录

- `../data/agent/skills/` — ai_agent 加载的 4 个 Skill Markdown
- `../board/contest_board/` — D12x 板卡配置（待 PR）
- `../quickapp/hello_quickapp/` — 极简 QuickApp 演示

## 团队

| 角色 | 同学 | 任务 |
| --- | --- | --- |
| 队长 | baimi | 架构 + 状态机 + Skill 系统 |
| 队员 | 队友 | LVGL UI + 传感器 + TTS + 演示 |

## 提交日志

- **2026-08-27**：VelaCare 7 个 C 文件 + 4 个 Skill 入仓，编译验证通过
