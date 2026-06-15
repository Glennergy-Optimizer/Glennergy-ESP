/**
 * @file DataParser.c
 * @brief Implementation of the JSON parsing helpers for recommendation, weather, and price data.
 *
 * @ingroup JSONParser
 */

#include "DataParser.h"
#include <string.h>
#include "jansson.h"
#include "esp_log.h"

static const char *TAG = "DataParser";

/**
 * @brief Parses recommendation data from a JSON array.
 *
 * See header for full contract documentation.
 */
int DataParser_ParseRecommendation(const char *raw_data, RecommendationList *r_list)
{
    json_error_t error;
    json_t *root = json_loads(raw_data, 0, &error);

    if (root == NULL)
    {
        ESP_LOGE(TAG, "Failed to load data from http response");
        return 1;
    }

    if (!json_is_array(root))
    {
        ESP_LOGW(TAG, "data is not an array");
        json_decref(root);
        return 2;
    }

    size_t array_size = json_array_size(root);

    if(array_size <= 0)
    {
        ESP_LOGW(TAG, "array is empty");
        json_decref(root);
        return 3;
    }

    r_list->count = array_size;

    for (int i = 0; i < array_size; i++)
    {
        json_t *obj = json_array_get(root, i);

        json_t *id = json_object_get(obj, "id");
        json_t *rec = json_object_get(obj, "type");
        json_t *timestamp = json_object_get(obj, "timestamp");

        r_list->rec[i].id = json_integer_value(id);
        r_list->rec[i].recommendation = json_real_value(rec);
        strncpy(r_list->rec[i].timestamp,
                json_string_value(timestamp),
                sizeof(r_list->rec[i].timestamp) - 1);

        r_list->rec[i].timestamp[sizeof(r_list->rec[i].timestamp) - 1] = '\0';
    }

    json_decref(root);
    return 0;
}

/**
 * @brief Parses weather data from a JSON array.
 *
 * See header for full contract documentation.
 */
int DataParser_ParseWeather(const char *raw_data, WeatherList *w_list)
{
    json_error_t error;
    json_t *root = json_loads(raw_data, 0, &error);

    if (root == NULL)
    {
        ESP_LOGE(TAG, "Failed to load data from http response");
        return 1;
    }

    if (!json_is_array(root))
    {
        ESP_LOGW(TAG, "data is not an array");
        json_decref(root);
        return 2;
    }

    size_t array_size = json_array_size(root);

    if(array_size <= 0)
    {
        ESP_LOGW(TAG, "array is empty");
        json_decref(root);
        return 3;
    }

    w_list->count = array_size;

    for (int i = 0; i < array_size; i++)
    {
        json_t *obj = json_array_get(root, i);

        json_t *temp = json_object_get(obj, "temp");
        json_t *uv_index = json_object_get(obj, "uv_index");
        json_t *weather_code = json_object_get(obj, "weather_code");
        json_t *timestamp = json_object_get(obj, "timestamp");

        w_list->weather[i].temp = json_real_value(temp);
        w_list->weather[i].weather_code = json_integer_value(weather_code);
        w_list->weather[i].uv_index = json_integer_value(uv_index);
        strncpy(w_list->weather[i].timestamp,
                json_string_value(timestamp),
                sizeof(w_list->weather[i].timestamp) - 1);

        w_list->weather[i].timestamp[sizeof(w_list->weather[i].timestamp) - 1] = '\0';
    }

    json_decref(root);
    return 0;
}

/**
 * @brief Parses price data from a JSON array.
 *
 * See header for full contract documentation.
 */
int DataParser_ParsePrice(const char *raw_data, PriceList *p_list)
{
    json_error_t error;
    json_t *root = json_loads(raw_data, 0, &error);

    if (root == NULL)
    {
        ESP_LOGE(TAG, "Failed to load data from http response");
        return 1;
    }

    if (!json_is_array(root))
    {
        ESP_LOGW(TAG, "data is not an array");
        json_decref(root);
        return 2;
    }

    size_t array_size = json_array_size(root);

    if(array_size <= 0)
    {
        ESP_LOGW(TAG, "array is empty");
        json_decref(root);
        return 3;
    }

    p_list->count = array_size;

    for (int i = 0; i < array_size; i++)
    {
        json_t *obj = json_array_get(root, i);

        json_t *price = json_object_get(obj, "price SEK");
        json_t *timestamp = json_object_get(obj, "timestamp");

        p_list->price[i].current_prices = json_real_value(price);
        strncpy(p_list->price[i].timestamp,
                json_string_value(timestamp),
                sizeof(p_list->price[i].timestamp) - 1);

        p_list->price[i].timestamp[sizeof(p_list->price[i].timestamp) - 1] = '\0';
    }

    json_decref(root);
    return 0;
}
