/*
 * SPDX-License-Identifier: MIT
 */
#ifndef SPECIAL_DAY_FACE_H_
#define SPECIAL_DAY_FACE_H_
#include "movement.h"

typedef enum {
    ADVENT,
    CHRISTMAS,
    ORDINARY_TIME,
    LENT,
    EASTER_TRIDUUM,
    EASTER,
    UNKNOWN // for initialization or error
} LiturgicalSeason;

typedef struct {
    LiturgicalSeason season;
    uint8_t week_of_season;
    bool alarm;
    const char *text;
} SpecialDay;

void special_day_face_setup(uint8_t watch_face_index, void ** context_ptr);
void special_day_face_activate(void *context);
bool special_day_face_loop(movement_event_t event, void *context);
void special_day_face_resign(void *context);
movement_watch_face_advisory_t special_day_face_advise(void *context);
#define special_day_face ((const watch_face_t) { \
    special_day_face_setup, \
    special_day_face_activate, \
    special_day_face_loop, \
    special_day_face_resign, \
    special_day_face_advise, \
})
#endif // SPECIAL_DAY_FACE_H_