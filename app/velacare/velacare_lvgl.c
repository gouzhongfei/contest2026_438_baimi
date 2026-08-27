/****************************************************************************
 * apps/velacare/velacare_lvgl.c — LVGL 屏幕渲染 (占位实现)
 ****************************************************************************/

#include <nuttx/config.h>
#include <syslog.h>
#include "velacare_lvgl.h"

void velacare_lvgl_init(void)
{
    /* TODO: 初始化 LVGL + D12x 480x272 RGB 屏 */
    syslog(LOG_NOTICE, "[VelaCare] LVGL init (stub)\n");
}

void velacare_lvgl_update_state(int state)
{
    /* TODO: 根据状态切换屏幕颜色和文字 */
    syslog(LOG_NOTICE, "[VelaCare] LVGL update state=%d (stub)\n", state);
}
