#ifndef __TIMER_REG_H__
#define __TIMER_REG_H__

#include <stdint.h>
#include <stdbool.h>

typedef struct TIMER {
    uint8_t reg;
    // Timestamp of when the current second started
    uint32_t last_update_millis;
    // A new value was set, so the next time update should be treated as first
    bool dirty;
} timer_reg_t;

void timer_set(timer_reg_t *timer, uint8_t value);
void timer_update_time(timer_reg_t *timer, uint32_t current_millis);

#endif //__TIMER_REG_H__