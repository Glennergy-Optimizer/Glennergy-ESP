#ifndef LEOP_FETCHER_H
#define LEOP_FETCHER_H
#include "Recommendation.h"
#include "Weather.h"
#include "Price.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @file LEOP_Fetcher.h
 * @brief Public API for the LEOP fetcher module.
 *
 * Provides the data containers, shared queues, and worker entry points used to
 * fetch LEOP recommendation, weather, and price data.
 *
 * @defgroup LEOP_FETCHER LEOP_FETCHER
 * @brief Fetcher module for LEOP data.
 *
 * The module initializes local storage for fetched data, updates it from the
 * network or cached sources, and publishes snapshots through FreeRTOS queues.
 * @{
 */

extern QueueHandle_t recommendation_queue;
extern QueueHandle_t weather_queue;
extern QueueHandle_t price_queue;

/**
 * @brief Configuration for LEOP fetch timing.
 *
 * The interval is expected to reference the fetch interval in minutes owned by
 * the application state.
 */
typedef struct{
    uint32_t* time_interval;
}LEOPConfig;

/**
 * @brief Aggregated LEOP data and fetch status.
 *
 * Stores the current recommendation, weather, and price payloads together with
 * the fetch configuration used by the worker.
 */
typedef struct{
    RecommendationList recommendations;
    WeatherList weather;
    PriceList price_list;
    LEOPConfig leop_conf;
}LEOPData;

/**
 * @brief Initializes the LEOP fetcher state and queues.
 *
 * @param[in,out] leop_data Pointer to the LEOP state to initialize.
 * @param[in] interval Fetch interval value passed to the module.
 *
 * @return `0` on success or a negative value on failure.
 *
 * @note Creates single-element FreeRTOS queues used to publish fetched data.
 */
int LEOPFetcher_Initialize(LEOPData *leop_data, uint32_t interval);

/**
 * @brief Worker task entry point for LEOP data fetching.
 *
 * @param[in] arg Pointer to LEOPData passed to the task.
 *
 * @note Runs in task context and blocks while fetching data and delaying
 * between retries or scheduled updates.
 */
void LEOPFetcher_Work(void *arg);

/** @} */

#endif
