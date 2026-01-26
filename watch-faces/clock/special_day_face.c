#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "special_day_face.h"
#include "special_days.h"
#include "watch.h"
#include "watch_utility.h"

#define SPECIAL_DAY_FACE_MANUAL_SCROLL false
#define SPECIAL_DAY_FACE_SCROLL_PAUSE_TICKS 2
#define SPECIAL_DAY_FACE_SCROLL_TICK_DIVISOR 1

typedef struct {
    uint16_t current_view_year;
    uint8_t current_view_month;
    uint8_t current_view_day;
    int16_t scroll_step;
} special_day_state_t;

static const char* get_special_day_text(uint16_t year, uint8_t month, uint8_t day) {
    // Check dynamic holidays: Mother's Day (2nd Sunday in May)
    if (month == 5 && day >= 8 && day <= 14) {
        if (watch_utility_get_iso8601_weekday_number(year, month, day) == 7) {
            return "Mothers Day";
        }
    }

    // Check dynamic holidays: Father's Day (3rd Sunday in June)
    if (month == 6 && day >= 15 && day <= 21) {
        if (watch_utility_get_iso8601_weekday_number(year, month, day) == 7) {
            return "Fathers Day";
        }
    }

    for (size_t i = 0; i < SPECIAL_DAYS_COUNT; i++) {
        if (special_days_list[i].month == month && special_days_list[i].day == day) {
            return special_days_list[i].text;
        }
    }
    return NULL;
}

static void update_indicators(special_day_state_t *state) {
    (void) state;
    watch_date_time_t now = movement_get_local_date_time();
    uint16_t now_y = now.unit.year + WATCH_RTC_REFERENCE_YEAR;
    uint8_t now_m = now.unit.month;
    uint8_t now_d = now.unit.day;

    // Signal for Today
    movement_set_time_signal_enabled(get_special_day_text(now_y, now_m, now_d) != NULL);

    // 24H for Tomorrow
    uint8_t tom_d = now_d + 1;
    uint8_t tom_m = now_m;
    uint16_t tom_y = now_y;
    if (tom_d > watch_utility_days_in_month(now_m, now_y)) {
        tom_d = 1;
        tom_m++;
        if (tom_m > 12) {
            tom_m = 1;
            tom_y++;
        }
    }
    movement_set_special_days_indicator_24h(get_special_day_text(tom_y, tom_m, tom_d) != NULL);
}

static void find_next_special_day(uint16_t start_year, uint8_t start_month, uint8_t start_day, 
                                uint16_t *out_year, uint8_t *out_month, uint8_t *out_day) {
    uint16_t y = start_year;
    uint8_t m = start_month;
    uint8_t d = start_day;

    for (int i = 0; i < 730; i++) {
        d++;
        uint8_t days_in_month = watch_utility_days_in_month(m, y);
        if (d > days_in_month) {
            d = 1;
            m++;
            if (m > 12) {
                m = 1;
                y++;
            }
        }

        if (get_special_day_text(y, m, d) != NULL) {
            *out_year = y;
            *out_month = m;
            *out_day = d;
            return;
        }
    }
    *out_year = start_year;
    *out_month = start_month;
    *out_day = start_day;
}

static void lookup_day(special_day_state_t *state, bool advance) {
    char buf[14];
    
    sprintf(buf, "%2d", state->current_view_month);
    watch_display_text(WATCH_POSITION_TOP_LEFT, buf);
    sprintf(buf, "%2d", state->current_view_day);
    watch_display_text(WATCH_POSITION_TOP_RIGHT, buf);

    const char *text = get_special_day_text(state->current_view_year, state->current_view_month, state->current_view_day);
    if (text != NULL && text[0] != '\0') {
        char text_buf[48];
        int j = 0;
        for (int i = 0; text[i] != '\0' && j < (int)sizeof(text_buf) - 1; i++) {
            char c = text[i];
            if (i > 0 && c >= 'A' && c <= 'Z') {
                text_buf[j++] = ' ';
                if (j >= (int)sizeof(text_buf) - 1) break;
            }
            if (c >= 'a' && c <= 'z') {
                c -= 32; // to upper
            }
            text_buf[j++] = c;
        }
        text_buf[j] = '\0';

        int len = strlen(text_buf);
        char display_buf[7];
        memset(display_buf, ' ', 6);
        display_buf[6] = '\0';

        if (len > 6) {
            int period = len + 2;
            if (advance) {
                state->scroll_step++;
                if (state->scroll_step >= period) {
                    state->scroll_step = SPECIAL_DAY_FACE_MANUAL_SCROLL ? 0 : -SPECIAL_DAY_FACE_SCROLL_PAUSE_TICKS;
                }
            }

            int offset = (state->scroll_step < 0) ? 0 : state->scroll_step;
            for (int i = 0; i < 6; i++) {
                int pos = (offset + i) % period;
                if (pos < len) display_buf[i] = text_buf[pos];
                else display_buf[i] = ' ';
            }
        } else {
            memcpy(display_buf, text_buf, len);
        }
        
        watch_display_text_with_fallback(WATCH_POSITION_BOTTOM, display_buf, display_buf);
    } else {
        watch_display_text_with_fallback(WATCH_POSITION_BOTTOM, "      ", "      ");
    }
}

void special_day_face_setup(uint8_t watch_face_index, void ** context_ptr) {
    (void) watch_face_index;
    if (*context_ptr == NULL) {
        *context_ptr = malloc(sizeof(special_day_state_t));
        special_day_state_t *state = (special_day_state_t *)*context_ptr;
        watch_date_time_t now = movement_get_local_date_time();
        state->current_view_year = now.unit.year + WATCH_RTC_REFERENCE_YEAR;
        state->current_view_month = now.unit.month;
        state->current_view_day = now.unit.day;
        state->scroll_step = SPECIAL_DAY_FACE_MANUAL_SCROLL ? 0 : -SPECIAL_DAY_FACE_SCROLL_PAUSE_TICKS;
    }
}

void special_day_face_activate(void *context) {
    special_day_state_t *state = (special_day_state_t *)context;
    watch_date_time_t now = movement_get_local_date_time();
    state->current_view_year = now.unit.year + WATCH_RTC_REFERENCE_YEAR;
    state->current_view_month = now.unit.month;
    state->current_view_day = now.unit.day;
    state->scroll_step = SPECIAL_DAY_FACE_MANUAL_SCROLL ? 0 : -SPECIAL_DAY_FACE_SCROLL_PAUSE_TICKS;
    update_indicators(state);
}

bool special_day_face_loop(movement_event_t event, void *context) {
    special_day_state_t *state = (special_day_state_t *)context;

    switch (event.event_type) {
        case EVENT_ACTIVATE:
            lookup_day(state, false);
            break;
        case EVENT_TICK:
        case EVENT_LOW_ENERGY_UPDATE:
            lookup_day(state, !SPECIAL_DAY_FACE_MANUAL_SCROLL);
            break;

        case EVENT_ALARM_BUTTON_UP:
            find_next_special_day(state->current_view_year, state->current_view_month, state->current_view_day,
                                &state->current_view_year, &state->current_view_month, &state->current_view_day);
            state->scroll_step = SPECIAL_DAY_FACE_MANUAL_SCROLL ? 0 : -SPECIAL_DAY_FACE_SCROLL_PAUSE_TICKS;
            lookup_day(state, false);
            break;
            
        case EVENT_BACKGROUND_TASK:
            update_indicators(state);
            break;

        default:
            return movement_default_loop_handler(event);
    }

    return true;
}

void special_day_face_resign(void *context) {
    (void) context;
}

movement_watch_face_advisory_t special_day_face_advise(void *context) {
    (void) context;
    watch_date_time_t date_time = movement_get_local_date_time();
    return (movement_watch_face_advisory_t) {
        .wants_background_task = (date_time.unit.hour == 0 && date_time.unit.minute == 0)
    };
}