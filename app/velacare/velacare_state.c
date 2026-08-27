#include <nuttx/config.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "velacare_state.h"
static const char *g_names[] = {"NORMAL","ATTENTION","WARNING","EMERGENCY","OFFLINE"};
const char *velacare_state_name(int s) { return (s>=0&&s<=4)?g_names[s]:"UNK"; }
void velacare_state_init(struct velacare_state_s *ctx) {
    memset(ctx,0,sizeof(*ctx)); ctx->current=0; ctx->smoke_online=ctx->water_online=ctx->door_online=ctx->fall_online=ctx->wifi_online=true;
}
bool velacare_state_evaluate(struct velacare_state_s *ctx, unsigned long now) {
    bool ch=false;
    if(!ctx->wifi_online && (now-ctx->state_enter_ms)>60000) {
        if(ctx->current!=4){ctx->previous=ctx->current;ctx->current=4;ctx->state_enter_ms=now;ch=true;} return ch;
    }
    if(ctx->current==4 && ctx->wifi_online){ctx->previous=4;ctx->current=0;ctx->state_enter_ms=now;return true;}
    if(ctx->smoke_alarm||ctx->water_alarm||ctx->door_alarm||ctx->fall_alarm) {
        if(ctx->current==0){ctx->previous=0;ctx->current=2;ctx->state_enter_ms=now;ch=true;}
        if(ctx->current==2&&(now-ctx->state_enter_ms)>30000){ctx->previous=2;ctx->current=3;ctx->state_enter_ms=now;ch=true;}
        return ch;
    }
    if(!ctx->smoke_online||!ctx->water_online||!ctx->door_online||!ctx->fall_online) {
        if(ctx->current==0){ctx->previous=0;ctx->current=1;ctx->state_enter_ms=now;ch=true;} return ch;
    }
    if(ctx->current!=0&&ctx->current!=4&&(now-ctx->state_enter_ms)>60000){ctx->previous=ctx->current;ctx->current=0;ctx->state_enter_ms=now;ch=true;}
    return ch;
}
void velacare_state_smoke_offline(struct velacare_state_s *ctx, bool on) { ctx->smoke_online=on; }
void velacare_state_smoke_alarm(struct velacare_state_s *ctx, bool a) { ctx->smoke_alarm=a; }
void velacare_state_wifi(struct velacare_state_s *ctx, bool on) { ctx->wifi_online=on; }
