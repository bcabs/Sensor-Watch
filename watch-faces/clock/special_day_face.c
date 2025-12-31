#include <stdlib.h>
#include <string.h>
#include "special_day_face.h"
#include "watch.h"
#include "watch_utility.h"

typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    bool alarm;
    const char *text;
    
} SpecialDay;

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

static const SpecialDay special_days[] = {
{2025,1,1,true,"New Year's Day"},
{2025,1,2,false,"Science Fiction Day"},
{2025,1,3,false,"JRR Tolkien Day"},
{2026,1,4,false,"World Braille Day"},
{2026,1,5,false,"Bird Day"},
{2026,1,6,false,"Epiphany"},
{2026,1,7,false,"Old Rock Day"},
{2026,1,8,false,"Bubble Bath Day"},
{2026,1,9,false,"Law Enforcement Day"},
{2026,1,10,false,"Houseplant Day"},
{2026,1,11,false,"Human Trafficking Day"},
{2026,1,12,false,"Youth Day"},
{2026,1,13,false,"Sticker Day"},
{2026,1,14,false,"Logic Day"},
{2026,1,15,false,"Wikipedia Day"},
{2026,1,16,false,"Religious Freedom Day"},
{2026,1,17,false,"Benjamin Franklin Day"},
{2026,1,18,false,"Martin Luther King Jr"},
{2026,1,19,false,"Popcorn Day"},
{2026,1,20,false,"Penguin Awareness"},
{2026,1,21,false,"Hugging Day"},
{2026,1,22,false,"Chinese New Year"},
{2026,1,23,false,"Handwriting Day"},
{2026,1,24,false,"Education Day"},
{2026,1,25,false,"Opposite Day"},
{2026,1,26,false,"Australia Day"},
{2026,1,27,false,"Holocaust Remembrance"},
{2026,1,28,false,"Data Privacy Day"},
{2026,1,29,false,"Puzzle Day"},
{2026,1,30,false,"Croissant Day"},
{2026,1,31,false,"Backward Day"},
{2026,2,1,false,"Dark Chocolate Day"},
{2026,2,2,false,"Groundhog Day"},
{2026,2,3,false,"Golden Retriever Day"},
{2026,2,4,false,"World Cancer Day"},
{2026,2,5,false,"Nutella Day"},
{2026,2,6,false,"Waitangi Day"},
{2026,2,7,false,"Send a Card Day"},
{2026,2,8,false,"Safer Internet Day"},
{2026,2,9,false,"Pizza Day"},
{2026,2,10,false,"Umbrella Day"},
{2026,2,11,false,"Women in Science Day"},
{2026,2,12,false,"Darwin Day"},
{2026,2,13,false,"Radio Day"},
{2026,2,14,false,"Valentine's Day"},
{2026,2,15,false,"Angelman Day"},
{2026,2,16,false,"Presidents Day"},
{2026,2,17,false,"Random Acts Day"},
{2026,2,18,false,"Pluto Day"},
{2026,2,19,false,"Chocolate Mint Day"},
{2026,2,20,false,"Social Justice Day"},
{2026,2,21,false,"Mother Language Day"},
{2026,2,22,false,"Thinking Day"},
{2026,2,23,false,"Banana Bread Day"},
{2026,2,24,false,"Flag Day Mexico"},
{2026,2,25,false,"World Spay Day"},
{2026,2,26,false,"Pistachio Day"},
{2026,2,27,false,"Polar Bear Day"},
{2026,2,28,false,"Rare Disease Day"},
};

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
    const char *display_text = "Just a day";

    for (size_t i = 0; i < sizeof(special_days) / sizeof(special_days[0]); i++) {
        if (date_time.unit.month == special_days[i].month && 
            date_time.unit.day == special_days[i].day) {
        
            if (special_days[i].year == 0 || 
                special_days[i].year == (date_time.unit.year + WATCH_RTC_REFERENCE_YEAR)) {

                if (isActivated) {
                    display_text = special_days[i].text;
                    watch_display_text((watch_position_t)WATCH_POSITION_FULL, display_text);
                }
                
                movement_set_alarm_enabled(special_days[i].alarm);
                clock_indicate_alarm();

                printf("foo");
                
                break;
            }
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
            lookup_day(false); // TODO : test
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
