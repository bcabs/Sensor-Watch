/*
 * SPDX-License-Identifier: MIT
 */
#ifndef LITURGICAL_CALENDAR_FACE_H_
#define LITURGICAL_CALENDAR_FACE_H_
#include "movement.h"

typedef enum {
    ADVENT,
    CHRISTMAS,
    ORDINARY_TIME,
    LENT,
    EASTER_TRIDUUM,
    EASTER,
    SEPTUAGESIMA,
    PASSIONTIDE,
    AFTER_EPIPHANY,
    AFTER_PENTECOST,
    UNKNOWN // for initialization or error
} LiturgicalSeason;

typedef struct {
    uint8_t season;
    uint8_t week_of_season;
    bool alarm;
    bool fast;
    const char *text;
} LiturgicalDay;

void liturgical_calendar_face_setup(uint8_t watch_face_index, void ** context_ptr);
void liturgical_calendar_face_activate(void *context);
bool liturgical_calendar_face_loop(movement_event_t event, void *context);
void liturgical_calendar_face_resign(void *context);
movement_watch_face_advisory_t liturgical_calendar_face_advise(void *context);
#define liturgical_calendar_face ((const watch_face_t) { \
    liturgical_calendar_face_setup, \
    liturgical_calendar_face_activate, \
    liturgical_calendar_face_loop, \
    liturgical_calendar_face_resign, \
    liturgical_calendar_face_advise, \
})
#endif // LITURGICAL_CALENDAR_FACE_H_
