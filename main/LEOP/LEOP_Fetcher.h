#ifndef LEOP_FETCHER_H
#define LEOP_FETCHER_H
#include "Recommendation.h"
#include "Weather.h"
#include "Price.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <stdint.h>
#include <stdbool.h>


extern QueueHandle_t recommendation_queue;
extern QueueHandle_t weather_queue;
extern QueueHandle_t price_queue;

typedef struct{
    uint32_t time_interval;
}LEOPConfig;

typedef struct{
    RecommendationList recommendations;
    WeatherList weather;
    PriceList price_list;
    LEOPConfig leop_conf;
}LEOPData;

int LEOPFetcher_Initialize(LEOPData *leop_data, uint32_t interval);

void LEOPFetcher_Work(void *arg);

#endif