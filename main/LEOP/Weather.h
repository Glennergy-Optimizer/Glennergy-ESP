#ifndef WEATHER_H
#define WEATHER_H

#include <stddef.h>

typedef struct
{
    float temp;
    int uv_index;
    int weather_code;
    char timestamp[20];
} Weather;

typedef struct
{
    Weather weather[96];
    size_t count;
} WeatherList;

int Weather_Initialize(WeatherList *r_list);

int Weather_Fetch(const char *url, WeatherList *r_list);

void Weather_Dispose(WeatherList *r_list);

#endif
