#include "Price.h"
#include "../HTTP.h"
#include "../JSONParser/DataParser.h"
#include "esp_log.h"

static const char *TAG = "Price";

int Price_Initialize(PriceList *p_list)
{
    ESP_LOGI(TAG, "CRASH 1");
    p_list->count = 0;
    for (int i = 0; i < 96; i++)
    {
        p_list->price[i] = (Price){0};
    }
    ESP_LOGI(TAG, "CRASH 2");
    return 0;
}

int Price_Fetch(const char *url, PriceList *p_list)
{
    HTTPResponse http_response = {0};

    HTTPClient_GET(url, &http_response);

    if (http_response.data == NULL)
    {
        ESP_LOGW(TAG, "HTTP response is invalid");
        return 1;
    }

    int ret = DataParser_ParsePrice(http_response.data, p_list);

    if(ret != 0)
    {
        return 2;
    }

    for (int i = 0; i < p_list->count; i++)
    {
        ESP_LOGI(TAG, "%lf", p_list->price[i].current_prices);
    }

    HTTPClient_Dispose(&http_response);

    return 0;
}

void Price_Dispose(PriceList *p_list)
{
    p_list->count = 0;

    for (int i = 0; i < 96; i++)
    {
        p_list->price[i] = (Price){0};
    }
}