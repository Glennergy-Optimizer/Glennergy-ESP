/**
 * @file Recommendation.c
 * @brief Implementation of the recommendation list module.
 *
 * @ingroup Recommendation
 */

#include "Recommendation.h"
#include "../HTTP.h"
#include "../JSONParser/DataParser.h"
#include "esp_log.h"

#ifdef RECOMMENDATION_TEST
static const char* http_mock = "[
    {
        "id": 2,
        "type": 0.0,
        "timestamp": "2026-06-10T14:45:00+02:00",
        "temp": 16.200000762939453
    },
    {
        "id": 2,
        "type": 0.0010183617495829668,
        "timestamp": "2026-06-10T15:00:00+02:00",
        "temp": 16.299999237060547
}]";
#endif

static const char *TAG = "Recommendation";

/**
 * @brief Initializes the recommendation list state.
 *
 * Clears the list, resets fetch status, and initializes the backing cache.
 *
 * @param r_list Pointer to the recommendation list instance.
 *
 * @return Always returns 0.
 */
int Recommendation_Initialize(RecommendationList *r_list)
{
    r_list->count = 0;
    for (int i = 0; i < 96; i++)
    {
        r_list->rec[i] = (Recommendation){0};
    }

    r_list->status.recommendation_fetched = false;

    Cache_Initialize(&r_list->cache);

    return 0;
}

/**
 * @brief Fetches recommendation data from the network and parses it.
 *
 * Uses the HTTP client in normal builds or the built-in test payload when
 * `RECOMMENDATION_TEST` is defined.
 *
 * @param url Request URL.
 * @param r_list Pointer to the recommendation list instance.
 *
 * @return
 * - `0` on success
 * - `1` if the HTTP response is invalid
 * - `2` if parsing fails
 */
int Recommendation_Fetch(const char *url, RecommendationList *r_list)
{
    HTTPResponse http_response = {0};

    #ifdef RECOMMENDATION_TEST
    http_response.data = http_mock;
    #else
    HTTPClient_GET(url, &http_response);
    #endif
    
    if (http_response.data == NULL)
    {
        ESP_LOGW(TAG, "HTTP response is invalid");
        return 1;
    }

    int res = Cache_WriteFileJSON(&r_list->cache, http_response.data, "Recommendations.json");

    if (res != 0)
    {
        ESP_LOGW(TAG, "Failed to cache data");
    }


    int ret = DataParser_ParseRecommendation(http_response.data, r_list);

    if (ret != 0)
    {
        HTTPClient_Dispose(&http_response);
        return 2;
    }

    for (int i = 0; i < r_list->count; i++)
    {
        //ESP_LOGI(TAG, "%lf", r_list->rec[i].recommendation);
    }

    HTTPClient_Dispose(&http_response);

    return 0;
}

/**
 * @brief Loads cached recommendation data and parses it.
 *
 * @param r_list Pointer to the recommendation list instance.
 *
 * @return
 * - `0` on success
 * - `1` if the cache file cannot be loaded
 * - `2` if parsing cached data fails
 */
int Recommendation_FetchCache(RecommendationList *r_list)
{
    int res = Cache_LoadFileJSON(&r_list->cache, "Recommendations.json");

    if (res != 0)
    {
        ESP_LOGE(TAG, "Failed to load cache");
        return 1;
    }

    res = DataParser_ParseRecommendation(r_list->cache.data, r_list);

    if (res != 0)
    {
        ESP_LOGE(TAG, "Failed to parse cached data");
        return 2;
    }

    for (int i = 0; i < r_list->count; i++)
    {
        //ESP_LOGI(TAG, "%lf", r_list->rec[i].recommendation);
    }

    Cache_Dispose(&r_list->cache);

    return 0;
}

/**
 * @brief Releases recommendation list state.
 *
 * Resets the stored entries and count. Cache ownership is not modified here.
 *
 * @param r_list Pointer to the recommendation list instance.
 */
void Recommendation_Dispose(RecommendationList *r_list)
{
    r_list->count = 0;

    for (int i = 0; i < 96; i++)
    {
        r_list->rec[i] = (Recommendation){0};
    }
}
