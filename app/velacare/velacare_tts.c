/****************************************************************************
 * apps/velacare/velacare_tts.c — TTS 语音播报 (占位实现)
 ****************************************************************************/

#include <nuttx/config.h>
#include <syslog.h>
#include "velacare_tts.h"

void velacare_tts_speak(const char *text)
{
    /* TODO: 调用 D12x TTS 引擎播报 */
    syslog(LOG_NOTICE, "[VelaCare] TTS: %s (stub)\n", text ? text : "(null)");
}
