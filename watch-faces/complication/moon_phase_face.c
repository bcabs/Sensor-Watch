/*
 * MIT License
 *
 * Copyright (c) 2022 Joey Castillo
 *
 * Based on Phase of Moon App for Tidbyt
 * https://github.com/tidbyt/community/blob/main/apps/phaseofmoon/phase_of_moon.star
 * Copyright (c) 2022 Alan Fleming
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "moon_phase_face.h"
#include "watch_utility.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define LUNAR_DAYS 29.53058770576
#define LUNAR_SECONDS (LUNAR_DAYS * (24 * 60 * 60))
#define FIRST_MOON 947182440 // Saturday, 6 January 2000 18:14:00 in unix epoch time
#define NUM_PHASES 8

static const float phase_changes[] = {0, 1, 6.38264692644, 8.38264692644, 13.76529385288, 15.76529385288, 21.14794077932, 23.14794077932, 28.53058770576, 29.53058770576};

void moon_phase_face_setup(uint8_t watch_face_index, void ** context_ptr) {
    (void) watch_face_index;
    if (*context_ptr == NULL) {
        *context_ptr = malloc(sizeof(moon_phase_state_t));
        memset(*context_ptr, 0, sizeof(moon_phase_state_t));
    }
}

void moon_phase_face_activate(void *context) {
    (void) context;
}

static void _update(moon_phase_state_t *state, uint32_t offset) {
    (void)state;
    char buf[4];
    watch_date_time_t date_time = watch_rtc_get_date_time();
    uint32_t now = watch_utility_date_time_to_unix_time(date_time, movement_get_current_timezone_offset()) + offset;
    date_time = watch_utility_date_time_from_unix_time(now, movement_get_current_timezone_offset());
    double currentfrac = fmod(now - FIRST_MOON, LUNAR_SECONDS) / LUNAR_SECONDS;
    double currentday = currentfrac * LUNAR_DAYS;
    uint8_t phase_index = 0;

    for(phase_index = 0; phase_index <= NUM_PHASES; phase_index++) {
        if (currentday > phase_changes[phase_index] && currentday <= phase_changes[phase_index + 1]) break;
    }

    sprintf(buf, "%2d", date_time.unit.day);
    watch_display_text(WATCH_POSITION_TOP_RIGHT, buf);

    uint8_t percent = (uint8_t)((1.0 - cos(currentfrac * 2.0 * M_PI)) * 50.0 + 0.5);
    char bottom_buf[7] = "      ";
    if (percent == 100) {
        bottom_buf[0] = '1';
        bottom_buf[1] = '0';
        bottom_buf[3] = '0';
    } else {
        bottom_buf[1] = (percent / 10) + '0';
        bottom_buf[3] = (percent % 10) + '0';
    }
    watch_display_text(WATCH_POSITION_BOTTOM, bottom_buf);

    char top_buf[3] = "  ";
    if (currentfrac < 0.5) top_buf[0] = '+';
    else top_buf[0] = '-';
    watch_display_text(WATCH_POSITION_TOP_LEFT, top_buf);
}

bool moon_phase_face_loop(movement_event_t event, void *context) {
    moon_phase_state_t *state = (moon_phase_state_t *)context;
    watch_date_time_t date_time;

    switch (event.event_type) {
        case EVENT_ACTIVATE:
            if (watch_sleep_animation_is_running()) watch_stop_sleep_animation();
            _update(state, state->offset);
            break;
        case EVENT_TICK:
            // only update once an hour
            date_time = watch_rtc_get_date_time();
            if ((date_time.unit.minute == 0) && (date_time.unit.second == 0)) _update(state, state->offset);
            break;
        case EVENT_LOW_ENERGY_UPDATE:
            // update at the top of the hour OR if we're entering sleep mode with an offset.
            // also, in sleep mode, always show the current moon phase (offset = 0).
            if (state->offset || (watch_rtc_get_date_time().unit.minute == 0)) _update(state, 0);
            // and kill the offset so when the wearer wakes up, it matches what's on screen.
            state->offset = 0;
            if (watch_get_lcd_type() == WATCH_LCD_TYPE_CLASSIC) {
                // clear out the last two digits and replace them with the sleep mode indicator
                watch_display_text(WATCH_POSITION_SECONDS, "  ");
            }
            if (!watch_sleep_animation_is_running()) watch_start_sleep_animation(1000);
            break;
        case EVENT_ALARM_BUTTON_UP:
            // Pressing the alarm adds an offset of one day to the displayed value,
            // so you can see moon phases in the future.
            state->offset += 86400;
            _update(state, state->offset);
            break;
	case EVENT_ALARM_LONG_PRESS:
	    state->offset = 0;
            _update(state, state->offset);
	    break;
        case EVENT_TIMEOUT:
            movement_move_to_face(0);
            break;
        default:
            return movement_default_loop_handler(event);
    }

    return true;
}

void moon_phase_face_resign(void *context) {
    moon_phase_state_t *state = (moon_phase_state_t *)context;
    state->offset = 0;
}
