#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "special_day_face.h"
#include "watch.h"
#include "watch_utility.h"
#include "special_days.h"

static void lookup_day(bool isActivated);
static int days_since_start(uint16_t year, uint8_t month, uint8_t day);

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

static void clock_indicate_lap() {
    clock_indicate(WATCH_INDICATOR_LAP, movement_lap_enabled());
}

static void clock_indicate_24h() {
    clock_indicate(WATCH_INDICATOR_24H, movement_24h_indicator_enabled());
}

static int days_since_start(uint16_t year, uint8_t month, uint8_t day) {
    if (year < SPECIAL_DAYS_START_DATE_YEAR) return -1;
    if (year == SPECIAL_DAYS_START_DATE_YEAR && month < SPECIAL_DAYS_START_DATE_MONTH) return -1;
    if (year == SPECIAL_DAYS_START_DATE_YEAR && month == SPECIAL_DAYS_START_DATE_MONTH && day < SPECIAL_DAYS_START_DATE_DAY) return -1;

    int days = 0;
    for (uint16_t y = SPECIAL_DAYS_START_DATE_YEAR; y < year; y++) {
        days += 365 + is_leap(y);
    }
    days += watch_utility_days_since_new_year(year, month, day);
    days -= watch_utility_days_since_new_year(SPECIAL_DAYS_START_DATE_YEAR, SPECIAL_DAYS_START_DATE_MONTH, SPECIAL_DAYS_START_DATE_DAY);

    // Debug output
    printf("SpecialDayFace: Date %04d-%02d-%02d, Start %04d-%02d-%02d, Index %d\n", 
           year, month, day, 
           SPECIAL_DAYS_START_DATE_YEAR, SPECIAL_DAYS_START_DATE_MONTH, SPECIAL_DAYS_START_DATE_DAY, 
           days);
    
    return days;
}

static const SpecialDay DUMMY_DAY = { UNKNOWN, 0, false, false, NULL };

static const SpecialDay* get_special_day(int days) {
    if (days >= 0 && days < (int)(sizeof(special_days) / sizeof(special_days[0]))) {
        return &special_days[days];
    }
    return &DUMMY_DAY;
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
    clock_indicate_lap();
    clock_indicate_24h();
}

void lookup_day(bool isActivated) {
    watch_date_time_t date_time = movement_get_local_date_time();
    int days = days_since_start(date_time.unit.year + WATCH_RTC_REFERENCE_YEAR, 
        date_time.unit.month, date_time.unit.day);
    const SpecialDay* special_day = get_special_day(days);
    const SpecialDay* next_day = get_special_day(days + 1);

    if (!isActivated) {
        // do background task and quit
        movement_set_alarm_enabled(special_day->alarm);
        movement_set_lap_enabled(special_day->fast);
        movement_set_24h_indicator_enabled(next_day->alarm);
        clock_indicate_alarm();
        clock_indicate_lap();
        clock_indicate_24h();
        return;
    }

    movement_set_alarm_enabled(special_day->alarm);
    movement_set_lap_enabled(special_day->fast);
    movement_set_24h_indicator_enabled(next_day->alarm);
    clock_indicate_alarm();
    clock_indicate_lap();
    clock_indicate_24h();

    if (special_day->season == UNKNOWN) {
        if (isActivated) {
            watch_display_text(WATCH_POSITION_FULL, "----");
        }
        return;
    }
   
    char buf[5];
    switch (special_day->season) {
        case ADVENT:
            buf[0] = 'A';
            buf[1] = 'D';
            break;
        case CHRISTMAS:
            buf[0] = 'C';
            buf[1] = 'H';
            break;
        case ORDINARY_TIME:
            buf[0] = 'O';
            buf[1] = 'T';
            break;
        case LENT:
            buf[0] = 'L';
            buf[1] = 'E';
            break;
        case EASTER_TRIDUUM:
            buf[0] = 'T';
            buf[1] = 'R';
            break;
        case EASTER:
            buf[0] = 'E';
            buf[1] = 'A';
            break;
        default:
            buf[0] = '?';
            buf[1] = '?';
            break;
    }
    
    sprintf(&buf[2], "%02d", special_day->week_of_season);
    watch_display_text_with_fallback(WATCH_POSITION_FULL, buf, buf);

    if (special_day->text != NULL && special_day->text[0] != '\0') {
        watch_display_text_with_fallback(WATCH_POSITION_BOTTOM, special_day->text, special_day->text);
    } else {
        watch_display_text_with_fallback(WATCH_POSITION_BOTTOM, "      ", "      ");
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
