#ifndef __APPS_VELACARE_STATE_H
#define __APPS_VELACARE_STATE_H
#include <stdbool.h>
#define VELACARE_STATE_NORMAL    0
#define VELACARE_STATE_ATTENTION 1
#define VELACARE_STATE_WARNING   2
#define VELACARE_STATE_EMERGENCY 3
#define VELACARE_STATE_OFFLINE   4
struct velacare_state_s {
    int current; int previous;
    unsigned long state_enter_ms; unsigned long last_event_ms;
    bool smoke_online; bool water_online; bool door_online; bool fall_online;
    bool smoke_alarm; bool water_alarm; bool door_alarm; bool fall_alarm;
    bool wifi_online;
};
typedef void (*velacare_state_cb_t)(int new_state, int old_state, const char *reason);
void velacare_state_init(struct velacare_state_s *ctx);
bool velacare_state_evaluate(struct velacare_state_s *ctx, unsigned long now_ms);
void velacare_state_smoke_offline(struct velacare_state_s *ctx, bool online);
void velacare_state_smoke_alarm(struct velacare_state_s *ctx, bool alarm);
void velacare_state_wifi(struct velacare_state_s *ctx, bool online);
const char *velacare_state_name(int state);
#endif
