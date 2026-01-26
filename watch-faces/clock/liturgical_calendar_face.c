#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "liturgical_calendar_face.h"
#include "watch.h"
#include "watch_utility.h"
#include "movement.h"
#include "liturgical_calendar.h"
#include "liturgical_calendar_alt.h"

#define LITURGICAL_CALENDAR_FACE_MANUAL_SCROLL false
#define LITURGICAL_CALENDAR_FACE_SCROLL_PAUSE_TICKS 2
#define LITURGICAL_CALENDAR_FACE_SCROLL_TICK_DIVISOR 1

typedef struct {
    int16_t scroll_step;
    int16_t manual_offset;
    uint8_t tick_count;
    bool use_alt_calendar;
    uint8_t mode_display_ticks;
} liturgical_calendar_state_t;

static void lookup_day(bool isActivated, liturgical_calendar_state_t *state, bool advance);
static int days_since_start(uint16_t year, uint8_t month, uint8_t day, bool use_alt);

static void clock_indicate(watch_indicator_t indicator, bool on) {
    if (on) {
        watch_set_indicator(indicator);
    } else {
        watch_clear_indicator(indicator);
    }
}

static int days_since_start(uint16_t year, uint8_t month, uint8_t day, bool use_alt) {
    uint16_t start_year = use_alt ? LITURGICAL_CALENDAR_ALT_START_DATE_YEAR : LITURGICAL_CALENDAR_NORMAL_START_DATE_YEAR;
    uint8_t start_month = use_alt ? LITURGICAL_CALENDAR_ALT_START_DATE_MONTH : LITURGICAL_CALENDAR_NORMAL_START_DATE_MONTH;
    uint8_t start_day = use_alt ? LITURGICAL_CALENDAR_ALT_START_DATE_DAY : LITURGICAL_CALENDAR_NORMAL_START_DATE_DAY;

    if (year < start_year) return -1;
    if (year == start_year && month < start_month) return -1;
    if (year == start_year && month == start_month && day < start_day) return -1;

    int days = 0;
    for (uint16_t y = start_year; y < year; y++) {
        days += 365 + is_leap(y);
    }
    days += watch_utility_days_since_new_year(year, month, day);
    days -= watch_utility_days_since_new_year(start_year, start_month, start_day);

    return days;
}

static const LiturgicalDay DUMMY_DAY = { UNKNOWN, 0, false, false, NULL };

static const LiturgicalDay* get_liturgical_day(int days, bool use_alt) {
    if (use_alt) {
        if (days >= 0 && days < (int)(sizeof(liturgical_calendar_alt) / sizeof(liturgical_calendar_alt[0]))) {
            return &liturgical_calendar_alt[days];
        }
    } else {
        if (days >= 0 && days < (int)(sizeof(liturgical_calendar_normal) / sizeof(liturgical_calendar_normal[0]))) {
            return &liturgical_calendar_normal[days];
        }
    }
    return &DUMMY_DAY;
}

void liturgical_calendar_face_setup(uint8_t watch_face_index, void ** context_ptr) {
    (void) watch_face_index;
    if (*context_ptr == NULL) {
        *context_ptr = malloc(sizeof(liturgical_calendar_state_t));
        liturgical_calendar_state_t *state = (liturgical_calendar_state_t *)*context_ptr;
        state->manual_offset = 0;
        state->tick_count = 0;
        state->use_alt_calendar = false; // Default to Normal
        state->mode_display_ticks = 0;
    }
}

void liturgical_calendar_face_activate(void *context) {
    liturgical_calendar_state_t *state = (liturgical_calendar_state_t *)context;
    state->scroll_step = LITURGICAL_CALENDAR_FACE_MANUAL_SCROLL ? 0 : -LITURGICAL_CALENDAR_FACE_SCROLL_PAUSE_TICKS; 
    state->manual_offset = 0;
    state->tick_count = 0;
    state->mode_display_ticks = 0;
}

void lookup_day(bool isActivated, liturgical_calendar_state_t *state, bool advance) {
    watch_date_time_t date_time = movement_get_local_date_time();
    int actual_days = days_since_start(date_time.unit.year + WATCH_RTC_REFERENCE_YEAR, 
        date_time.unit.month, date_time.unit.day, state->use_alt_calendar);
    
    // Always update global state based on ACTUAL day and CURRENT mode
    const LiturgicalDay* actual_day = get_liturgical_day(actual_days, state->use_alt_calendar);
    const LiturgicalDay* actual_next_day = get_liturgical_day(actual_days + 1, state->use_alt_calendar);
    
    movement_set_alarm_enabled(actual_day->alarm);
    movement_set_lap_enabled(actual_day->fast);
    movement_set_liturgical_calendar_indicator_24h(actual_next_day->alarm);

    if (!isActivated) {
        return;
    }
    
    // Handle mode switch display
    if (state->mode_display_ticks > 0) {
        watch_clear_display();
        if (state->use_alt_calendar) {
            watch_display_text(WATCH_POSITION_BOTTOM, "Trad  ");
        } else {
            watch_display_text(WATCH_POSITION_BOTTOM, "Normal");
        }
        if (advance) { // Count down ticks
             state->mode_display_ticks--;
        }
        return;
    }

    // Foreground: Use manual offset
    int target_days = actual_days + state->manual_offset;
    const LiturgicalDay* display_day = get_liturgical_day(target_days, state->use_alt_calendar);
    const LiturgicalDay* display_next_day = get_liturgical_day(target_days + 1, state->use_alt_calendar);

    // Update indicators locally to reflect the DISPLAYED day
    clock_indicate(WATCH_INDICATOR_BELL, display_day->alarm);
    clock_indicate(WATCH_INDICATOR_LAP, display_day->fast);
    clock_indicate(WATCH_INDICATOR_24H, display_next_day->alarm);

    if (display_day->season == UNKNOWN) {
        watch_display_text(WATCH_POSITION_FULL, "---- ");
        return;
    }
   
    char buf[12]; // Increased buffer size for safety
    switch (display_day->season) {
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
        case SEPTUAGESIMA:
            buf[0] = 'S';
            buf[1] = 'E';
            break;
        case PASSIONTIDE:
            buf[0] = 'P';
            buf[1] = 'A';
            break;
        case AFTER_EPIPHANY:
            buf[0] = 'A';
            buf[1] = 'E';
            break;
        case AFTER_PENTECOST:
            buf[0] = 'A';
            buf[1] = 'P';
            break;
        default:
            buf[0] = '?';
            buf[1] = '?';
            break;
    }
    
    sprintf(&buf[2], "%2d", display_day->week_of_season);
    watch_display_text_with_fallback(WATCH_POSITION_FULL, buf, buf);

    if (display_day->text != NULL && display_day->text[0] != '\0') {
        char text_buf[48];
        int j = 0;
        for (int i = 0; display_day->text[i] != '\0' && j < (int)sizeof(text_buf) - 1; i++) {
            char c = display_day->text[i];
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
                    state->scroll_step = LITURGICAL_CALENDAR_FACE_MANUAL_SCROLL ? 0 : -LITURGICAL_CALENDAR_FACE_SCROLL_PAUSE_TICKS;
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

bool liturgical_calendar_face_loop(movement_event_t event, void *context) {
    liturgical_calendar_state_t *state = (liturgical_calendar_state_t *)context;
    switch (event.event_type) {
        case EVENT_ALARM_BUTTON_UP:
            state->manual_offset++;
            state->scroll_step = LITURGICAL_CALENDAR_FACE_MANUAL_SCROLL ? 0 : -LITURGICAL_CALENDAR_FACE_SCROLL_PAUSE_TICKS; // Reset scrolling when changing day
            lookup_day(true, state, false);
            break;
        case EVENT_LIGHT_BUTTON_UP:
            if (LITURGICAL_CALENDAR_FACE_MANUAL_SCROLL) {
                lookup_day(true, state, true);
                movement_illuminate_led();
                return true;
            }
            break;
        case EVENT_MODE_LONG_PRESS:
            // Toggle calendar mode
            state->use_alt_calendar = !state->use_alt_calendar;
            // Set display timer (e.g. 2 seconds = 2 ticks at 1Hz)
            state->mode_display_ticks = 2; // Adjusted for 1Hz
            state->scroll_step = LITURGICAL_CALENDAR_FACE_MANUAL_SCROLL ? 0 : -LITURGICAL_CALENDAR_FACE_SCROLL_PAUSE_TICKS;
            lookup_day(true, state, false);
            // Inhibit default mode switch behavior
            break;
        case EVENT_TICK:
            if (!LITURGICAL_CALENDAR_FACE_MANUAL_SCROLL) {
                state->tick_count++;
                if (state->tick_count >= LITURGICAL_CALENDAR_FACE_SCROLL_TICK_DIVISOR) {
                    state->tick_count = 0;
                    lookup_day(true, state, true);
                }
            } else {
                lookup_day(true, state, false);
            }
            break;
        case EVENT_ACTIVATE:
            // Advance on tick only if NOT manual scroll (auto mode) AND event is TICK.
            // On ACTIVATE, we want to display initial state (advance=false).
            lookup_day(true, state, !LITURGICAL_CALENDAR_FACE_MANUAL_SCROLL && event.event_type == EVENT_TICK);
            break;
        case EVENT_LOW_ENERGY_UPDATE:
            lookup_day(true, state, false);
            break;
        case EVENT_BACKGROUND_TASK:
            lookup_day(false, state, false);
            break;
        default:
            return movement_default_loop_handler(event);
    }

    return true;
}

void liturgical_calendar_face_resign(void *context) {
    (void) context;
}

movement_watch_face_advisory_t liturgical_calendar_face_advise(void *context) {
    (void) context;
    movement_watch_face_advisory_t retval = { 0 };

    // only run the background task at midnight to update the day's status
    watch_date_time_t date_time = movement_get_local_date_time();
    retval.wants_background_task = (date_time.unit.hour == 0 && date_time.unit.minute == 0);

    return retval;
}