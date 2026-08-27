# VelaCare Skills — ai_agent 技能目录

> openvela `ai_agent` 框架启动时扫描 `/data/agent/skills/*.md`，
> 把每个 Markdown 解析成一个可调用的 Skill 技能。

## Skill 列表

| 文件 | 技能名 | 触发场景 |
| --- | --- | --- |
| `home-safety-guard.md` | 家庭安全守护 | 检测到烟雾/燃气泄漏/陌生人入侵 |
| `elder-care-reminder.md` | 老人关怀提醒 | 定时提醒吃药、活动检测、跌倒预警 |
| `emergency-broadcast.md` | 紧急广播 | EMERGENCY 状态触发，喊话 + 联系家属 |
| `routine-scheduler.md` | 日常安排 | 起床/午休/熄灯等日程任务 |

## Skill 文件格式（4 段式）

每个 Skill Markdown 必须包含以下 4 个二级标题：

```markdown
# 技能名

## When to use
（什么时候调用这个技能，描述触发条件）

## How to use
（怎么调用这个技能，列出参数和步骤）

## Example
（典型调用示例 + 预期响应）
```

## 加载机制

ai_agent 启动后：

1. 遍历 `/data/agent/skills/*.md`
2. 用 `## ` 切分，提取 4 段元数据
3. 解析文件名（去掉 `.md`）作为 `skill_id`
4. 把 `When to use` 段落转成自然语言描述，注册到意图识别表
5. 收到用户 query 时，先做意图匹配 → 命中则调用对应 `How to use`

## 验证方法

在 PC 端用 Python 模拟器验证（无需烧板）：

```bash
cd .notes/
python3 mock_llm_client.py --skill data/agent/skills/home-safety-guard.md
```

## 扩展指南

新增一个 Skill：

1. 在 `data/agent/skills/` 新建 `xxx-skill.md`
2. 按 4 段式编写
3. 重新烧录或重启 ai_agent 进程，自动加载
4. 用 mock_llm_client 跑一遍意图识别
