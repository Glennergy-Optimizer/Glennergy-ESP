#ifndef LEOP_FETCHER_H
#define LEOP_FETCHER_H
#include "Recommendation.h"
#include "Weather.h"
#include "Price.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <stdint.h>
#include <stdbool.h>


extern QueueHandle_t leop_queue;

/*
This should be a pointer to app_state_t->config_data_t->fetch_interval_minutes
*/
typedef struct{
    uint32_t* time_interval;
}LEOPConfig;


typedef struct{
    bool recommendation_fetched;
    bool weather_fetched;
    bool electricity_fetched;
}LEOPStatus;

typedef struct{
    RecommendationList recommendations;
    WeatherList weather;
    PriceList price_list;
    LEOPStatus leop_status;
    LEOPConfig leop_conf;
}LEOPData;

int LEOPFetcher_Initialize(LEOPData *leop_data, uint32_t interval);

void LEOPFetcher_Work(void *arg);

#endif