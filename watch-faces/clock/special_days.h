#ifndef SPECIAL_DAYS_H_
#define SPECIAL_DAYS_H_

#include <stdint.h>

typedef struct {
    uint8_t month;
    uint8_t day;
    const char *text;
} SpecialDayDefinition;

static const SpecialDayDefinition special_days_list[] = {
    {1, 1, "New Year"},
    {2, 1, "first"},
    {4, 1, "second"},
    {11, 1, "third"},
    {12, 1, "fourth"},
    // Add your special days here
    // {1, 11, "My Birthday"},
};

#define SPECIAL_DAYS_COUNT (sizeof(special_days_list) / sizeof(SpecialDayDefinition))

#endif // SPECIAL_DAYS_H_
