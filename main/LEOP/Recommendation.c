#include "Recommendation.h"
#include "../HTTP.h"
#include "../JSONParser/DataParser.h"
#include "esp_log.h"

static const char *TAG = "Recommendation";

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

int Recommendation_Fetch(const char *url, RecommendationList *r_list)
{
    HTTPResponse http_response = {0};

    HTTPClient_GET(url, &http_response);

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
        return 2;
    }

    for (int i = 0; i < r_list->count; i++)
    {
        //ESP_LOGI(TAG, "%lf", r_list->rec[i].recommendation);
    }

    HTTPClient_Dispose(&http_response);

    return 0;
}

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

void Recommendation_Dispose(RecommendationList *r_list)
{
    r_list->count = 0;

    for (int i = 0; i < 96; i++)
    {
        r_list->rec[i] = (Recommendation){0};
    }
}