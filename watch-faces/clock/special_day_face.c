#include <stdlib.h>
#include <string.h>
#include "special_day_face.h"
#include "watch.h"
#include "watch_utility.h"

typedef struct {
    uint8_t month;
    uint8_t day;
    const char *text;
} SpecialDay;

static const SpecialDay special_days[] = {
    {1, 1, "New Year's"},
    {7, 4, "July 4th"},
    {12, 25, "Christmas"},
    {4, 1, "April Fools"},
};

void special_day_face_setup(uint8_t watch_face_index, void ** context_ptr) {
    (void) watch_face_index;
    if (*context_ptr == NULL) {
        *context_ptr = malloc(sizeof(uint32_t));
    }
}

void special_day_face_activate(void *context) {
    (void) context;
}

bool special_day_face_loop(movement_event_t event, void *context) {
    (void) context;
    watch_date_time_t date_time;
    const char *display_text = "Just a day";

    switch (event.event_type) {
        case EVENT_TICK:
        case EVENT_ACTIVATE:
            date_time = movement_get_local_date_time();
            for (size_t i = 0; i < sizeof(special_days) / sizeof(special_days[0]); i++) {
                if (date_time.unit.month == special_days[i].month && date_time.unit.day == special_days[i].day) {
                    display_text = special_days[i].text;
                    break;
                }
            }
            watch_display_text((watch_position_t)WATCH_POSITION_FULL, display_text);
            break;
        case EVENT_LOW_ENERGY_UPDATE:
            // do nothing
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
    movement_watch_face_advisory_t advisory = {0};
    return advisory;
}
