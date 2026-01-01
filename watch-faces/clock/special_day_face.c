#include <stdlib.h>
#include <string.h>
#include "special_day_face.h"
#include "watch.h"
#include "watch_utility.h"
#include "special_days.h"

static void lookup_day(bool isActivated);
static int days_since_start(uint16_t year, uint8_t month, uint8_t day);

static const watch_date_time_t HARDCODED_START_DATE = { .unit = { .year = 5, .month = 1, .day = 1 } }; // 2025-01-01

static void clock_indicate(watch_indicator_t indicator, bool on) {
    if (on) {
        watch_set_indicator(indicator);
    } else {
        watch_clear_indicator(indicator);
    }
}

static void clock_indicate_alarm() {
    clock_indicate(WATCH_INDICATOR_BELL, movement_alarm_enabled());
}

static int days_since_start(uint16_t year, uint8_t month, uint8_t day) {
    uint16_t start_year_abs = HARDCODED_START_DATE.unit.year + WATCH_RTC_REFERENCE_YEAR;
    if (year < start_year_abs) return -1;
    
    int days = 0;
    for (uint16_t y = start_year_abs; y < year; y++) {
        days += 365 + is_leap(y);
    }
    days += watch_utility_days_since_new_year(year, month, day) - 1;
    return days;
}

void special_day_face_setup(uint8_t watch_face_index, void ** context_ptr) {
    (void) watch_face_index;
    if (*context_ptr == NULL) {
        *context_ptr = malloc(sizeof(uint32_t));
    }
}

void special_day_face_activate(void *context) {
    (void) context;
    clock_indicate_alarm();
}

void lookup_day(bool isActivated) {
    watch_date_time_t date_time = movement_get_local_date_time();
    int days = days_since_start(date_time.unit.year + WATCH_RTC_REFERENCE_YEAR, date_time.unit.month, date_time.unit.day);

    if (days >= 0 && days < (int)(sizeof(special_days) / sizeof(special_days[0]))) {
        const SpecialDay* special_day = &special_days[days];
        if (special_day->text != NULL) {
            if (isActivated) {
                watch_display_text((watch_position_t)WATCH_POSITION_FULL, special_day->text);
            }
            movement_set_alarm_enabled(special_day->alarm);
            clock_indicate_alarm();
        } else {
            if (isActivated) {
                watch_display_text((watch_position_t)WATCH_POSITION_FULL, "Just a day");
            }
        }
    } else {
        if (isActivated) {
            watch_display_text((watch_position_t)WATCH_POSITION_FULL, "Just a day");
        }
    }
}

bool special_day_face_loop(movement_event_t event, void *context) {
    (void) context;
    switch (event.event_type) {
        case EVENT_TICK:
        case EVENT_ACTIVATE:
            lookup_day(true);
            break;
        case EVENT_BACKGROUND_TASK:
            lookup_day(false);
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
    movement_watch_face_advisory_t retval = { 0 };

    // schedule every hour
    watch_date_time_t date_time = movement_get_local_date_time();
    retval.wants_background_task = date_time.unit.second == 0;

    return retval;
}
