#include "timer_reg.h"

#define TIMER_TICK_MILLIS_ 17

void timer_set(timer_reg_t *timer, uint8_t value) {
    timer->reg = value;
    timer->dirty = true;
}

void timer_update_time(timer_reg_t *timer, uint32_t current_millis) {
    if (timer->reg > 0) {
        if (timer->dirty) {
            timer->last_update_millis = current_millis;
            timer->dirty = false;
        } else {
            if (current_millis - timer->last_update_millis > TIMER_TICK_MILLIS_) {
                timer->last_update_millis = current_millis;
                --timer->reg;
            }
        }
    }
    
}
