#include <nuttx/config.h>
#include <stdio.h>
#include "velacare_tts.h"
void velacare_tts_speak(const char *t) { printf("[VelaCare] TTS: %s stub\n",t?t:"null"); }
