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
    return 0;
}

void Recommendation_Fetch(const char *url, RecommendationList *r_list)
{
    HTTPResponse http_response = {0};

    HTTPClient_GET(url, &http_response);

    if (http_response.data == NULL)
    {
        ESP_LOGW(TAG, "HTTP response is invalid");
        return;
    }

    DataParser_Parse(http_response.data, r_list);

    for (int i = 0; i < r_list->count; i++)
    {
        ESP_LOGI(TAG, "%lf", r_list->rec[i].recommendation);
    }

    HTTPClient_Dispose(&http_response);
}

void Recommendation_Dispose(RecommendationList *r_list)
{
    r_list->count = 0;

    for (int i = 0; i < 96; i++)
    {
        r_list->rec[i] = (Recommendation){0};
    }
}