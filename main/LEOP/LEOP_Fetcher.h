#ifndef LEOP_FETCHER_H
#define LEOP_FETCHER_H
#include "Recommendation.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <stdint.h>
#include <stdbool.h>


extern QueueHandle_t leop_queue;

typedef struct{
    uint32_t time_interval;
}LEOPConfig;

typedef struct{
    bool recommendation_fetched;
    bool weather_fetched;
    bool electricity_fetched;
}LEOPStatus;

typedef struct{
    RecommendationList recommendations;
    LEOPStatus leop_status;
    LEOPConfig leop_conf;
}LEOPData;

int LEOPFetcher_Initialize(LEOPData *leop_data, uint32_t interval);

void LEOPFetcher_Work(void *arg);

#endif