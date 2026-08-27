#include <nuttx/config.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <syslog.h>
#include <time.h>
#include "velacare_state.h"
int main(int argc, char *argv[]) {
    struct velacare_state_s st;
    velacare_state_init(&st);
    syslog(LOG_NOTICE,"[VelaCare] starting, state=%s\n",velacare_state_name(st.current));
    velacare_lvgl_init();
    velacare_skill_init();
    velacare_cron_init();
    unsigned long tick=0;
    while(1) {
        velacare_state_smoke_offline(&st,true);
        velacare_state_smoke_alarm(&st,false);
        velacare_state_wifi(&st,true);
        if(velacare_state_evaluate(&st,tick))
            syslog(LOG_NOTICE,"[VelaCare] state->%s\n",velacare_state_name(st.current));
        tick+=1000; sleep(1);
    }
    return 0;
}
