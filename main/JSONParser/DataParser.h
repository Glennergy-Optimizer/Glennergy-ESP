#ifndef DATAPARSER_H
#define DATAPARSER_H

#include "../HTTP.h"
#include "../LEOP/Recommendation.h"
#include "../LEOP/Weather.h"
#include "../LEOP/Price.h"

/**
 * @file DataParser.h
 * @brief Public API for parsing JSON responses into LEOP data lists.
 *
 * Provides helpers for converting HTTP response payloads into the
 * recommendation, weather, and price structures used by the application.
 *
 * @defgroup DATAPARSER DataParser
 * @brief JSON parsing helpers for LEOP data models.
 *
 * Parses response payloads into application lists. The caller must provide
 * valid output buffers large enough for the parsed array contents.
 * @{
 */

/**
 * @brief Parses a recommendation JSON array into a recommendation list.
 *
 * @param[in] raw_data JSON payload string.
 * @param[out] r_list Destination list for parsed recommendation entries.
 *
 * @return Parsing status code used by the module.
 */
int DataParser_ParseRecommendation(const char* raw_data, RecommendationList* r_list);

/**
 * @brief Parses a weather JSON array into a weather list.
 *
 * @param[in] raw_data JSON payload string.
 * @param[out] w_list Destination list for parsed weather entries.
 *
 * @return Parsing status code used by the module.
 */
int DataParser_ParseWeather(const char *raw_data, WeatherList *w_list);

/**
 * @brief Parses a price JSON array into a price list.
 *
 * @param[in] raw_data JSON payload string.
 * @param[out] p_list Destination list for parsed price entries.
 *
 * @return Parsing status code used by the module.
 */
int DataParser_ParsePrice(const char *raw_data, PriceList *p_list);

/** @} */

#endif
